/*
 * Copyright (C) 2022 The Android Open Source Project
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

/* Yet Another Gralloc Interface (YAGI), DRM-compatible buffer-info supplier library.
 * Upstream URL:
 * https://gitlab.freedesktop.org/drm-hwcomposer/drm-hwcomposer/-/tree/main/bufferinfo/yagi
 *
 * Library must expose 3 symbols:
 * 1. yagi_init
 * 2. yagi_destroy
 * 3. yagi_bi_get
 * 4. yagi_get_data is optional for v1
 * Please find type information below.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <cutils/native_handle.h>
#include <stdint.h>

/* Implementation-specific struct, opaque for client */
struct yagi;

/* for mesa3d front buffer rendering feature */
#define YAGI_DATA_MUTABLE_BUFFER_USAGE_MASK (1 << 1)

#define YAGI_PROTECTED_BUFFER_FLAG (1 << 1)
#define YAGI_MUTABLE_BUFFER_FLAG (1 << 2)

/* Do not modify! Create v2 instead if necessary */
#define YAGI_MAX_PLANES_V1 4
struct yagi_bi_v1 {
  uint32_t width;
  uint32_t height;
  uint32_t drm_format; /* DRM_FORMAT_* from drm_fourcc.h */
  uint32_t pitches[YAGI_MAX_PLANES_V1];
  uint32_t offsets[YAGI_MAX_PLANES_V1];
  /* fds should be set for every plane even if it is the same buffer */
  int prime_fds[YAGI_MAX_PLANES_V1];
  uint64_t modifiers[YAGI_MAX_PLANES_V1];
  int num_planes;
  uint32_t yagi_flags;
};

typedef int (*yagi_bi_get_t)(struct yagi *yagi, buffer_handle_t handle,
                             struct yagi_bi_v1 *out_buf_info, int version,
                             int size);

typedef int (*yagi_get_data_t)(struct yagi *yagi, uint32_t yagi_data, int *result);

typedef struct yagi *(*yagi_init_t)(int *out_yagi_api_version);

typedef void (*yagi_destroy_t)(struct yagi *);

#ifdef __cplusplus
}
#endif
