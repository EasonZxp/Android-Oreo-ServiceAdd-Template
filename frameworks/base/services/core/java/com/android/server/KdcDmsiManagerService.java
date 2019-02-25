/*
 * Copyright (C) 2008 The Android Open Source Project
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

package com.android.server;

import android.content.Context;
import android.os.KdcDmsiManager;
import android.os.IKdcDmsiManager;
import android.util.Slog;

import java.util.HashMap;

public class KdcDmsiManagerService extends IKdcDmsiManager.Stub {
    private static final String TAG = "KdcDmsiManagerService";

    KdcDmsiManagerService() {
        Slog.d(TAG, "KdcDmsiManagerService construct...");
        init_native();
	}

    public int readSerialno(int length, byte[] buf) {
        int result = kdc_read_serialno_native(length, buf);
        if (result != 0) {
            Slog.e(TAG, "readSerialno error...");
        }
        return result;
    }

    public int readImei(int slotId, int length, byte[] buf) {
        int result = kdc_read_imei_native(slotId, length, buf);
        if (result != 0) {
            Slog.e(TAG, "readImei error...");
        }
        return result;
    }

    public int readMeid(int length, byte[] buf) {
        int result = kdc_read_meid_native(length, buf);
        if (result != 0) {
            Slog.e(TAG, "readMeid error...");
        }
        return result;
    }

    public int readCalResult(int length, byte[] buf) {
        int result = kdc_read_calibrate_result_native(length, buf);
        if (result != 0) {
            Slog.e(TAG, "readCalResult error...");
        }
        return result;
    }

    public int writeCalResult(int length, byte[] buf) {
        int result = kdc_write_calibrate_result_native(length, buf);
        if (result != 0) {
            Slog.e(TAG, "writeCalResult error...");
        }
        return result;
    }

    private static native boolean init_native();

    private static native int kdc_read_serialno_native(int length, byte[] nv_buf);

    private static native int kdc_read_imei_native(int sub_id, int length, byte[] nv_buf);

    private static native int kdc_read_meid_native(int length, byte[] nv_buf);

    private static native int kdc_read_calibrate_result_native(int length, byte[] nv_buf);

    private static native int kdc_write_calibrate_result_native(int length, byte[] nv_buf);
};
