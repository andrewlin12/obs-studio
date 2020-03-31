#include <obs-module.h>
#include <media-io/video-io.h>
#include <QMainWindow>
#include <QAction>
#include <QDialog>
#include <obs-frontend-api.h>
#include <util/threading.h>

#include <servers/bootstrap.h>
#include <mach/mach.h>

#include "CMIODPASampleServer.h"
#include "CMIO_DPA_Sample_Server_VCamAssistant.h"
#include "CMIO_DPA_Sample_Server_VCamDevice.h"

struct virtual_out_data {
	obs_output_t *output = nullptr;
	pthread_mutex_t mutex;
	int width = 0;
	int height = 0;
	int64_t last_video_ts = 0;
};

obs_output_t *virtual_out;
bool output_running = false;
bool audio_running = false;

static void nv12_to_y422(uint8_t *in, int width, int height, uint8_t **out,
			 int *out_length)
{
	*out_length = width * height * 2;
	uint8_t *frame = (uint8_t *)bzalloc(*out_length);

	uint8_t *y = in;
	uint8_t *uv = y + (width * height);
	for (int r = 0; r < height; r += 1) {
		for (int c = 0; c < width; c += 2) {
			int i = r * width + c;
			int uv_i = ((r / 2) * width) + c;

			// U0
			frame[i * 2] = uv[uv_i];
			// V0ut i
			frame[i * 2 + 2] = uv[uv_i + 1];

			// Y0
			frame[i * 2 + 1] = y[i];
			// Y1
			frame[i * 2 + 3] = y[i + 1];
		}
	}

	*out = frame;
}

static void virtual_output_destroy(void *data)
{
	output_running = false;
	audio_running = false;
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
	printf("%s", "CMIO getname\n");
	UNUSED_PARAMETER(unused);
	return "CMIO Virtual Camera";
}

static void cmio_output_stop(void *data, uint64_t ts)
{
	printf("%s", "CMIO stop\n");
	virtual_out_data *out_data = (virtual_out_data *)data;
	obs_output_end_data_capture(out_data->output);
	output_running = false;
	audio_running = false;
}

static void cmio_output_destroy(void *data)
{
	printf("%s", "CMIO destroy\n");
}

boolean_t MessagesAndNotifications(mach_msg_header_t *request,
				   mach_msg_header_t *reply)
{
	// Invoke the MIG created CMIODPASampleServer() to see if this is one of the client messages it handles
	boolean_t processed = CMIODPASampleServer(request, reply);

	// If CMIODPASampleServer() did not process the message see if it is a MACH_NOTIFY_NO_SENDERS notification
	if (not processed and MACH_NOTIFY_NO_SENDERS == request->msgh_id) {
		/*
		CMIO::DPA::Sample::Server::VCamAssistant::Instance()->ClientDied(
			request->msgh_local_port);
		*/
		processed = true;
	}

	return processed;
}

CMIO::DPA::Sample::Server::VCamDevice *device;
pthread_t mach_msg_thread;
mach_port_t portSet;
void *runloop(void *vargp)
{
	// Check in with the bootstrap port under the agreed upon name to get the servicePort with receive rights
	mach_port_t servicePort;
	name_t serviceName = "com.apple.cmio.DPA.SampleVCam";
	kern_return_t err =
		bootstrap_check_in(bootstrap_port, serviceName, &servicePort);
	if (BOOTSTRAP_SUCCESS != err) {
		printf("CMIO: bootstrap_check_in() failed: 0x%x\n", err);
		exit(43);
	}
	printf("CMIO: bootstrap_check_in() succeeded!\n");

	// Create a port set to hold the service port, and each client's port
	portSet = CMIO::DPA::Sample::Server::VCamAssistant::Instance()
			  ->GetPortSet();
	err = mach_port_move_member(mach_task_self(), servicePort, portSet);
	if (KERN_SUCCESS != err) {
		blog(LOG_INFO,
		     "CMIO: Unable to add service port to port set: 0x%x", err);
		exit(2);
	}
	printf("CMIO: Successfully added service port to port set\n");

	device = (CMIO::DPA::Sample::Server::VCamDevice *)
			 CMIO::DPA::Sample::Server::VCamAssistant::Instance()
				 ->GetDevice();
	printf("CMIO: Created VCamDevice\n");

	// Service incoming messages from the clients and notifications which were signed up for
	while (true) {
		(void)mach_msg_server(MessagesAndNotifications, 8192, portSet,
				      MACH_MSG_OPTION_NONE);
	}
}

static void *cmio_output_create(obs_data_t *settings, obs_output_t *output)
{
	printf("CMIO cmio_output_create\n");
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
	printf("%s", "CMIO start\n");
	virtual_out_data *out_data = (virtual_out_data *)data;
	video_t *video = obs_output_video(out_data->output);
	printf("CMIO start: Video Format - %s\n",
	       get_video_format_name(video_output_get_format(video)));

	out_data->width = (int32_t)obs_output_get_width(out_data->output);
	out_data->height = (int32_t)obs_output_get_height(out_data->output);
	printf("CMIO start: Video Size - %d x %d\n", out_data->width,
	       out_data->height);

	if (out_data) {
		obs_output_begin_data_capture(out_data->output, 0);
		printf("%s", "CMIO output_begin_data_capture called\n");
		return true;
	}

	return false;
}

static obs_properties_t *cmio_output_properties(void *unused)
{
	printf("%s", "CMIO properties\n");
	UNUSED_PARAMETER(unused);

	obs_properties_t *props = obs_properties_create();
	return props;
}

static void cmio_output_raw_video(void *param, struct video_data *frame)
{
	printf("CMIO raw_video - timestamp %lld\n", frame->timestamp);
	uint8_t *converted;
	int converted_length;
	nv12_to_y422(frame->data[0], 1280, 720, &converted, &converted_length);
	device->EmitFrame(device, converted);
	bfree(converted);
}

static void cmio_output_raw_audio(void *data, struct audio_data *frames)
{
	// printf("%s", "CMIO raw_audio");
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

QDialog *properties_dialog;
obs_output *cmio_out;
QAction *action;
bool obs_module_load(void)
{
	printf("%s", "CMIO obj_module_load");
	obs_register_output(&cmio_output_info);

	obs_data_t *settings = obs_data_create();
	cmio_out =
		obs_output_create("cmio_output", "CMIOOutput", settings, NULL);
	obs_data_release(settings);
	signal_handler_t *handler = obs_output_get_signal_handler(cmio_out);
	signal_handler_add(handler,
			   "void output_stop(string msg, bool opening)");

	// QMainWindow *main_window =
	//	(QMainWindow *)obs_frontend_get_main_window();
	action = (QAction *)obs_frontend_add_tools_menu_qaction(
		obs_module_text("ToolsMenu_CMIOOutput"));

	// obs_frontend_push_ui_translation(obs_module_get_string);
	// properties_dialog = new QDialog(main_window);
	// obs_frontend_pop_ui_translation();

	auto menu_cb = [] {
		printf("%s", "CMIO tools menu item pressed");
		// properties_dialog->setVisible(!properties_dialog->isVisible());
		// printf("%s", "CMIO dialog visible");
		obs_output_start(cmio_out);
		printf("%s", "CMIO obs_output_start called");
	};

	action->connect(action, &QAction::triggered, menu_cb);

	printf("%s", "CMIO obj_module_load complete");
	return true;
}
