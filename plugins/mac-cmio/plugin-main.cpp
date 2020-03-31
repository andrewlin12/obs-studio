#include <obs-module.h>
#include <media-io/video-io.h>
#include <QMainWindow>
#include <QAction>
#include <QDialog>
#include <obs-frontend-api.h>
#include <util/threading.h>

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
	blog(LOG_INFO, "out_length: %d", *out_length);

	uint8_t *frame = (uint8_t *)bzalloc(*out_length);
	blog(LOG_INFO, "out bzalloc: %p", *frame);

	uint8_t *y = in;
	uint8_t *uv = y + (width * height);
	for (int r = 0; r < height; r += 1) {
		for (int c = 0; c < width; c += 2) {
			int i = r * width + c;
			int uv_i = ((r / 2) * width) + c;
			// U0
			frame[i * 2] = (uv[uv_i] & 0x01) +
				       ((uv[uv_i] >> 1) & 0x02) +
				       ((uv[uv_i] >> 2) & 0x04) +
				       ((uv[uv_i] >> 3) & 0x08) +
				       ((uv[uv_i + 1] << 4) & 0x10) +
				       ((uv[uv_i + 1] << 3) & 0x20) +
				       ((uv[uv_i + 1] << 2) & 0x40) +
				       ((uv[uv_i + 1] << 1) & 0x80);
			// V0
			frame[i * 2 + 2] = ((uv[uv_i] >> 1) & 0x01) +
					   ((uv[uv_i] >> 2) & 0x02) +
					   ((uv[uv_i] >> 3) & 0x04) +
					   ((uv[uv_i] >> 4) & 0x08) +
					   ((uv[uv_i + 1] << 3) & 0x10) +
					   ((uv[uv_i + 1] << 2) & 0x20) +
					   ((uv[uv_i + 1] << 1) & 0x40) +
					   (uv[uv_i + 1] & 0x80);

			frame[i * 2] = uv[uv_i];
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
	blog(LOG_INFO, "%s", "CMIO getname");
	UNUSED_PARAMETER(unused);
	return "CMIO Virtual Camera";
}

static void cmio_output_stop(void *data, uint64_t ts)
{
	blog(LOG_INFO, "%s", "CMIO stop");
	virtual_out_data *out_data = (virtual_out_data *)data;
	obs_output_end_data_capture(out_data->output);
	output_running = false;
	audio_running = false;
}

static void cmio_output_destroy(void *data)
{
	blog(LOG_INFO, "%s", "CMIO destroy");
}

static void *cmio_output_create(obs_data_t *settings, obs_output_t *output)
{
	/*
	FILE *test_file =
		fopen("/Users/andrewlin/Downloads/test_nv12.yuv", "r");
	uint8_t *frame = new uint8_t[1382400];
	fread(frame, 1, 1382400, test_file);

	uint8_t *converted;
	int converted_length;
	nv12_to_y422(frame, 1280, 720, &converted, &converted_length);
	blog(LOG_INFO, "%s", "CMIO create");

	FILE *fp;
	fp = fopen("/Users/andrewlin/Downloads/converted_nv12_to_y422.yuv",
		   "w");
	blog(LOG_INFO, "CMIO raw_video - fp %lld", fp);
	fwrite(converted, 1, converted_length, fp);
	fclose(fp);

	exit(0);
	*/

	virtual_out_data *data =
		(virtual_out_data *)bzalloc(sizeof(struct virtual_out_data));

	data->output = output;
	pthread_mutex_init_value(&data->mutex);
	if (pthread_mutex_init(&data->mutex, NULL) == 0) {
		UNUSED_PARAMETER(settings);
		return data;
	} else {
		virtual_output_destroy(data);
	}

	return NULL;
}

static bool cmio_output_start(void *data)
{
	blog(LOG_INFO, "%s", "CMIO start");
	virtual_out_data *out_data = (virtual_out_data *)data;
	video_t *video = obs_output_video(out_data->output);
	blog(LOG_INFO, "CMIO start: Video Format - %s",
	     get_video_format_name(video_output_get_format(video)));

	out_data->width = (int32_t)obs_output_get_width(out_data->output);
	out_data->height = (int32_t)obs_output_get_height(out_data->output);
	blog(LOG_INFO, "CMIO start: Video Size - %d x %d", out_data->width,
	     out_data->height);

	if (out_data) {
		obs_output_begin_data_capture(out_data->output, 0);
		blog(LOG_INFO, "%s", "CMIO output_begin_data_capture called");
		return true;
	}

	return false;
}

static obs_properties_t *cmio_output_properties(void *unused)
{
	blog(LOG_INFO, "%s", "CMIO properties");
	UNUSED_PARAMETER(unused);

	obs_properties_t *props = obs_properties_create();
	return props;
}

bool wrote_frame = false;

static void cmio_output_raw_video(void *param, struct video_data *frame)
{
	blog(LOG_INFO, "CMIO raw_video - timestamp %lld", frame->timestamp);
	if (!wrote_frame) {
		for (int i = 0; i < MAX_AV_PLANES; i++) {
			blog(LOG_INFO, "frame %d linesize %d", i,
			     frame->linesize[i]);
		}

		uint8_t *converted;
		int converted_length;
		nv12_to_y422(frame->data[0], 1280, 720, &converted,
			     &converted_length);

		FILE *fp;
		fp = fopen(
			"/Users/andrewlin/Downloads/converted_nv12_to_y422.yuv",
			"w");
		blog(LOG_INFO, "CMIO raw_video - fp %lld", fp);
		fwrite(converted, 1, converted_length, fp);
		fclose(fp);

		bfree(converted);
		wrote_frame = true;
	}
}

static void cmio_output_raw_audio(void *data, struct audio_data *frames)
{
	blog(LOG_INFO, "%s", "CMIO raw_audio");
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
bool obs_module_load(void)
{
	blog(LOG_INFO, "%s", "CMIO obj_module_load");
	obs_register_output(&cmio_output_info);

	obs_data_t *settings = obs_data_create();
	cmio_out =
		obs_output_create("cmio_output", "CMIOOutput", settings, NULL);
	obs_data_release(settings);
	signal_handler_t *handler = obs_output_get_signal_handler(cmio_out);
	signal_handler_add(handler,
			   "void output_stop(string msg, bool opening)");

	QMainWindow *main_window =
		(QMainWindow *)obs_frontend_get_main_window();
	QAction *action = (QAction *)obs_frontend_add_tools_menu_qaction(
		obs_module_text("ToolsMenu_CMIOOutput"));

	obs_frontend_push_ui_translation(obs_module_get_string);
	properties_dialog = new QDialog(main_window);
	obs_frontend_pop_ui_translation();

	auto menu_cb = [] {
		blog(LOG_INFO, "%s", "CMIO tools menu item pressed");
		properties_dialog->setVisible(!properties_dialog->isVisible());
		blog(LOG_INFO, "%s", "CMIO dialog visible");
		obs_output_start(cmio_out);
		blog(LOG_INFO, "%s", "CMIO obs_output_start called");
	};

	action->connect(action, &QAction::triggered, menu_cb);

	blog(LOG_INFO, "%s", "CMIO obj_module_load complete");
	return true;
}
