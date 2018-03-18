/*
 * Copyright (C) 2010-2011 Chia-I Wu <olvaffe@gmail.com>
 * Copyright (C) 2010-2011 LunarG Inc.
 * Copyright (C) 2016 Linaro, Ltd., Rob Herring <robh@kernel.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef _GRALLOC_DRM_HANDLE_H_
#define _GRALLOC_DRM_HANDLE_H_

#include <cutils/log.h>
#include <cutils/native_handle.h>
#include <system/graphics.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <gralloc_handle.h>

static inline int gralloc_drm_get_prime_fd(buffer_handle_t _handle)
{
	struct gralloc_handle_t *handle = gralloc_handle(_handle);
	return (handle) ? handle->prime_fd : -1;
}

static inline int gralloc_drm_get_gem_handle(buffer_handle_t handle)
{
	(void)handle;
	return 0; /* Not supported, return invalid handle. */
}

#ifdef __cplusplus
}
#endif
#endif /* _GRALLOC_DRM_HANDLE_H_ */
