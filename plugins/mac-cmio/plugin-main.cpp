#include <obs-module.h>
#include <media-io/video-io.h>
#include <obs-frontend-api.h>
#include <util/threading.h>
#include <util/base.h>

#include <dispatch/dispatch.h>
#include <servers/bootstrap.h>
#include <mach/mach.h>

#include "CMIODPASampleServer.h"
#include "CMIO_DPA_Sample_Server_VCamAssistant.h"
#include "CMIO_DPA_Sample_Server_VCamDevice.h"

#define PLUGIN_USE_QT 0

#if PLUGIN_USE_QT
#include <QMainWindow>
#include <QAction>
#include <QDialog>
#endif

#define do_log(level, format, ...)                  \
	blog(level, "[CMIO] " format, ##__VA_ARGS__)

// 200 and 300 are from base.h but those values get
// redefined from syslog.h, so hard code them.
#define warn(format, ...) do_log(200, format, ##__VA_ARGS__)
#define info(format, ...) do_log(300, format, ##__VA_ARGS__)

struct virtual_out_data {
	obs_output_t *output = nullptr;
	pthread_mutex_t mutex;
	int width = 0;
	int height = 0;
};

obs_output_t *virtual_out;
bool output_running = false;
CMIO::DPA::Sample::Server::VCamDevice *device;
pthread_t mach_msg_thread;
mach_port_t portSet;
obs_output *cmio_out;
bool cmio_started;

#if PLUGIN_USE_QT
QAction *start_action;
QAction *stop_action;
#endif

std::function<void()> start_cb = [] {
	if (!output_running) {
		info("%s", "start menu item pressed");
		obs_output_start(cmio_out);
		info("%s", "obs_output_start called");
#if PLUGIN_USE_QT
		start_action->setVisible(false);
		stop_action->setVisible(true);
#endif
	}
};

std::function<void()> stop_cb = [] {
	if (output_running) {
		info("%s", "stop menu item pressed");
		obs_output_stop(cmio_out);
		info("%s", "obs_output_start called");
#if PLUGIN_USE_QT
		start_action->setVisible(true);
		stop_action->setVisible(false);
#endif
	}
};

static void nv12_to_y422(uint8_t *in, int in_width, int in_height,
			 int out_width, int out_height, uint8_t **out,
			 int *out_length)
{
	*out_length = out_width * out_height * 2;
	uint8_t *frame = (uint8_t *)bzalloc(*out_length);

	uint8_t *y = in;
	uint8_t *uv = y + (in_width * in_height);
	for (int r = 0; r < out_height; r += 1) {
		for (int c = 0; c < out_width; c += 2) {
			int out_i = r * out_width + c;

			if (r >= in_height || c >= in_width) {
				// U0
				frame[out_i * 2] = 127;
				// V0
				frame[out_i * 2 + 2] = 127;
				// Y0
				frame[out_i * 2 + 1] = 0;
				// Y1
				frame[out_i * 2 + 3] = 0;
				continue;
			}

			int i = r * in_width + c;
			int uv_i = ((r / 2) * in_width) + c;

			// U0
			frame[out_i * 2] = uv[uv_i];
			// V0
			frame[out_i * 2 + 2] = uv[uv_i + 1];
			// Y0
			frame[out_i * 2 + 1] = y[i];
			// Y1
			frame[out_i * 2 + 3] = y[i + 1];
		}
	}

	*out = frame;
}

static void virtual_output_destroy(void *data)
{
	output_running = false;
	virtual_out_data *out_data = (virtual_out_data *)data;
	if (out_data) {
		pthread_mutex_destroy(&out_data->mutex);
		bfree(data);
	}
}

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("mac-cmio", "en-US")
MODULE_EXPORT const char *obs_module_description(void)
{
	return "macOS CoreMedioIO virtual camera output";
}

extern struct obs_output_info cmio_output_info;

static const char *cmio_output_getname(void *unused)
{
	info("cmio_output_getname");
	UNUSED_PARAMETER(unused);
	return "CMIO Virtual Camera";
}

static void cmio_output_stop(void *data, uint64_t ts)
{
	info("cmio_output_stop");
	virtual_out_data *out_data = (virtual_out_data *)data;
	obs_output_end_data_capture(out_data->output);
	output_running = false;
}

static void cmio_output_destroy(void *data)
{
	info("cmio_output_destroy");
	virtual_output_destroy(data);
}

boolean_t MessagesAndNotifications(mach_msg_header_t *request,
				   mach_msg_header_t *reply)
{
	// Invoke the MIG created CMIODPASampleServer() to see if this is one of the client messages it handles
	boolean_t processed = CMIODPASampleServer(request, reply);

	// If CMIODPASampleServer() did not process the message see if it is a MACH_NOTIFY_NO_SENDERS notification
	if (not processed and MACH_NOTIFY_NO_SENDERS == request->msgh_id) {
		/* TODO: This seems to crash things, should try to figure out why
		dispatch_async(dispatch_get_main_queue(), ^{
			CMIO::DPA::Sample::Server::VCamAssistant::Instance()
				->ClientDied(request->msgh_local_port);
		});
		processed = true;
		*/
	}

	return processed;
}

void *runloop(void *vargp)
{
	// Check in with the bootstrap port under the agreed upon name to get the servicePort with receive rights
	mach_port_t servicePort;
	name_t serviceName = "com.apple.cmio.DPA.SampleVCam";
	kern_return_t err =
		bootstrap_check_in(bootstrap_port, serviceName, &servicePort);
	if (BOOTSTRAP_SUCCESS != err) {
		warn("bootstrap_check_in() failed: 0x%x", err);
		exit(43);
	}
	info("bootstrap_check_in() succeeded!");

	// Create a port set to hold the service port, and each client's port
	portSet = CMIO::DPA::Sample::Server::VCamAssistant::Instance()
			  ->GetPortSet();
	err = mach_port_move_member(mach_task_self(), servicePort, portSet);
	if (KERN_SUCCESS != err) {
		info("Unable to add service port to port set: 0x%x", err);
		exit(2);
	}

	info("LOG_INFO=%d, LOG_WARN=%d, LOG_ZZZ=%d", LOG_INFO, LOG_WARNING, LOG_ZZZ);
	info("Successfully added service port to port set");

	device = (CMIO::DPA::Sample::Server::VCamDevice *)
			 CMIO::DPA::Sample::Server::VCamAssistant::Instance()
				 ->GetDevice();
	CMIO::DPA::Sample::Server::VCamAssistant::Instance()
		->SetStartStopHandlers(start_cb, stop_cb);
	info("Created VCamDevice");

	// Service incoming messages from the clients and notifications which were signed up for
	while (true) {
		(void)mach_msg_server(MessagesAndNotifications, 8192, portSet,
				      MACH_MSG_OPTION_NONE);
	}
}

static void *cmio_output_create(obs_data_t *settings, obs_output_t *output)
{
	info("cmio_output_create");
	virtual_out_data *data =
		(virtual_out_data *)bzalloc(sizeof(struct virtual_out_data));

	data->output = output;
	pthread_mutex_init_value(&data->mutex);
	if (pthread_mutex_init(&data->mutex, NULL) != 0) {
		virtual_output_destroy(data);
		return NULL;
	}

	pthread_create(&mach_msg_thread, NULL, runloop, NULL);

	UNUSED_PARAMETER(settings);
	return data;
}

static bool cmio_output_start(void *data)
{
	info("cmio_output_start");
	virtual_out_data *out_data = (virtual_out_data *)data;
	video_t *video = obs_output_video(out_data->output);
	info("start: Video Format - %s",
	       get_video_format_name(video_output_get_format(video)));

	out_data->width = (int32_t)obs_output_get_width(out_data->output);
	out_data->height = (int32_t)obs_output_get_height(out_data->output);
	info("start: Video Size - %d x %d", out_data->width,
	       out_data->height);

	if (out_data) {
		obs_output_begin_data_capture(out_data->output, 0);
		info("obs_output_begin_data_capture called");
		output_running = true;
		return true;
	}

	return false;
}

static obs_properties_t *cmio_output_properties(void *unused)
{
	info("cmio_output_properties");
	UNUSED_PARAMETER(unused);

	obs_properties_t *props = obs_properties_create();
	return props;
}

static void cmio_output_raw_video(void *data, struct video_data *frame)
{
	virtual_out_data *out_data = (virtual_out_data *)data;
	// info("raw_video - timestamp %lld", frame->timestamp);
	uint8_t *converted;
	int converted_length;
	nv12_to_y422(frame->data[0], out_data->width, out_data->height, 1280,
		     720, &converted, &converted_length);
	device->EmitFrame(device, converted);
	bfree(converted);
}

static void cmio_output_raw_audio(void *data, struct audio_data *frames)
{
	// info("%s", "cmio_output_raw_audio");
}

static void cmio_output_update(void *data, obs_data_t *settings) {}

struct obs_output_info cmio_output_info = {
	.id = "cmio_output",
	.flags = OBS_OUTPUT_AUDIO | OBS_OUTPUT_VIDEO,
	.get_name = cmio_output_getname,
	.create = cmio_output_create,
	.destroy = cmio_output_destroy,
	.get_properties = cmio_output_properties,
	.raw_audio = cmio_output_raw_audio,
	.raw_video = cmio_output_raw_video,
	.start = cmio_output_start,
	.stop = cmio_output_stop,
	.update = cmio_output_update,
};

bool obs_module_load(void)
{
	info("obs_module_load");
	obs_register_output(&cmio_output_info);

	obs_data_t *settings = obs_data_create();
	cmio_out =
		obs_output_create("cmio_output", "CMIOOutput", settings, NULL);
	obs_data_release(settings);
	signal_handler_t *handler = obs_output_get_signal_handler(cmio_out);
	signal_handler_add(handler,
			   "void output_stop(string msg, bool opening)");

#if PLUGIN_USE_QT
	start_action = (QAction *)obs_frontend_add_tools_menu_qaction(
		obs_module_text("ToolsMenu_CMIOOutputStart"));
	stop_action = (QAction *)obs_frontend_add_tools_menu_qaction(
		obs_module_text("ToolsMenu_CMIOOutputStop"));
	stop_action->setVisible(false);
	start_action->connect(start_action, &QAction::triggered, start_cb);
	stop_action->connect(stop_action, &QAction::triggered, stop_cb);
#endif

	info("obs_module_load complete");
	return true;
}
