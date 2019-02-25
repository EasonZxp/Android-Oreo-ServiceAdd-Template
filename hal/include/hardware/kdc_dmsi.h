/*
 * Copyright (C) 2013 The Android Open Source Project
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

#ifndef _HARDWARE_kdc_dmsi_H
#define _HARDWARE_kdc_dmsi_H

#include <hardware/hardware.h>

__BEGIN_DECLS

#define KDC_DMSI_API_VERSION HARDWARE_MODULE_API_VERSION(1,0)

/**
 * The id of this module
 */
#define KDC_DMSI_HARDWARE_MODULE_ID "kdc_dmsi"

#define KDC_DMSI_DEFAULT_DEVICE "default"

struct kdc_dmsi_device;
typedef struct kdc_dmsi_device {

	struct hw_device_t common;
	int (*kdc_read_nv)(const int nv_cmd, const int length, char* buf);
	int (*kdc_write_calibrate_result)(const int length, char* buf);
} kdc_dmsi_device_t;

typedef struct kdc_dmsi_module {
    struct hw_module_t common;
}kdc_dmsi_module_t;

__END_DECLS

#endif
