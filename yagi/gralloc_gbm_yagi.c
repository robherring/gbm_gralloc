/*
 * Copyright (C) 2018-2022 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* YAGI implementation for gbm_gralloc's buffer_handle. */

#define LOG_TAG "gbm_gralloc_yagi"

#include <errno.h>
#include <hardware/gralloc.h>
#include <android/gralloc_handle.h>
#include <log/log.h>
#include <drm/drm_fourcc.h>

#include "android_yagi.h"

#define EXPORT __attribute__((visibility("default")))

#define ALIGN(A, B) (((A) + (B)-1) & ~((B)-1))
#define DIV_ROUND_UP(n, d) (((n) + (d)-1) / (d))

static int get_drm_format_and_bpp(int hal_format, uint32_t *out_fourcc)
{
	switch (hal_format) {
	case HAL_PIXEL_FORMAT_RGBA_8888:
		*out_fourcc = DRM_FORMAT_ABGR8888;
		break;
	case HAL_PIXEL_FORMAT_RGBX_8888:
		*out_fourcc = DRM_FORMAT_XBGR8888;
		break;
	case HAL_PIXEL_FORMAT_RGB_888:
		*out_fourcc = DRM_FORMAT_RGB888;
		break;
	case HAL_PIXEL_FORMAT_RGB_565:
		*out_fourcc = DRM_FORMAT_RGB565;
		break;
	case HAL_PIXEL_FORMAT_BGRA_8888:
		*out_fourcc = DRM_FORMAT_ARGB8888;
		break;
	case HAL_PIXEL_FORMAT_YV12:
		*out_fourcc = DRM_FORMAT_YVU420;
		break;
	case HAL_PIXEL_FORMAT_RGBA_FP16:
		*out_fourcc = DRM_FORMAT_ABGR16161616F;
		break;
	case HAL_PIXEL_FORMAT_RGBA_1010102:
		*out_fourcc = DRM_FORMAT_ABGR2101010;
		break;
	default:
		ALOGE("Format not supported");
		return -EINVAL;
		break;
	}

	return 0;
}

EXPORT int yagi_bi_get(struct yagi *yagi,
                       buffer_handle_t handle,
                       struct yagi_bi_v1 *out_buf_info, int version,
                       int size) {
  struct gralloc_handle_t *gr_handle = NULL;
  uint32_t drm_format = 0;
  int ret = 0;

  if (yagi == NULL || handle == NULL || out_buf_info == NULL ||
      version != 1 || size != sizeof(struct yagi_bi_v1)) {
    ALOGE("Invalid arguments");
    return -EINVAL;
  }

  gr_handle = gralloc_handle(handle);

  ret = get_drm_format_and_bpp(gr_handle->format, &drm_format);
  if (ret)
    return ret;

  out_buf_info->width = gr_handle->width;
  out_buf_info->height = gr_handle->height;
  out_buf_info->drm_format = drm_format;
  out_buf_info->pitches[0] = gr_handle->stride;
  out_buf_info->offsets[0] = 0;
  out_buf_info->prime_fds[0] = gr_handle->prime_fd;
  out_buf_info->modifiers[0] = gr_handle->modifier;
  out_buf_info->num_planes = 1;

  if (gr_handle->format == HAL_PIXEL_FORMAT_YV12) {
    out_buf_info->pitches[1] = ALIGN(DIV_ROUND_UP(gr_handle->stride, 2), 16);
    out_buf_info->offsets[1] = gr_handle->stride * gr_handle->height;
    out_buf_info->modifiers[1] = gr_handle->modifier;
    out_buf_info->pitches[2] = out_buf_info->pitches[1];
    out_buf_info->offsets[2] = out_buf_info->offsets[1] +
                               DIV_ROUND_UP(out_buf_info->pitches[1] * gr_handle->height, 2);
    out_buf_info->modifiers[2] = gr_handle->modifier;
    out_buf_info->num_planes = 3;
  }

  out_buf_info->yagi_flags = 0;

  return 0;
}

EXPORT struct yagi *yagi_init() {
  // Must return any non-null value
  return (struct yagi *)~0;
}

EXPORT void yagi_destroy(struct yagi *yagi) {
  (void)yagi;
}
