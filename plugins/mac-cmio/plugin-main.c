#include <obs-module.h>


OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("mac-cmio", "en-US")
MODULE_EXPORT const char *obs_module_description(void)
{
	return "macOS CoreMedioIO virtual camera output";
}

extern struct obs_output_info cmio_output_info;

bool obs_module_load(void)
{
	obs_register_output(&cmio_output_info);
	return true;
}

static const char *cmio_output_getname(void *unused)
{
	UNUSED_PARAMETER(unused);
	return obs_module_text("CMIO Virtual Camera");
}

static void cmio_output_stop(void *data, uint64_t ts) {
	printf("CMIO stop\n");
}

static void cmio_output_destroy(void *data)
{
	printf("CMIO destroy\n");
}

static void *cmio_output_create(obs_data_t *settings, obs_output_t *output)
{
	printf("CMIO create\n");
	UNUSED_PARAMETER(settings);
	return NULL;
}

static bool cmio_output_start(void *data)
{
	printf("CMIO start\n");
	return true;
}

static obs_properties_t *cmio_output_properties(void *unused)
{
	printf("CMIO properties\n");
	UNUSED_PARAMETER(unused);

	obs_properties_t *props = obs_properties_create();
	return props;
}

static void cmio_output_raw_video(void *param, struct video_data *frame) {}

static void cmio_output_raw_audio2(void *param, size_t mix_idx, struct audio_data *frame) {}

struct obs_output_info cmio_output_info = {
	.id = "cmio_output",
	.flags = OBS_OUTPUT_AV,
	.get_name = cmio_output_getname,
	.create = cmio_output_create,
	.destroy = cmio_output_destroy,
	.raw_audio2 = cmio_output_raw_audio2,
	.raw_video = cmio_output_raw_video,
	.start = cmio_output_start,
	.stop = cmio_output_stop,
	.get_properties = cmio_output_properties,
};
