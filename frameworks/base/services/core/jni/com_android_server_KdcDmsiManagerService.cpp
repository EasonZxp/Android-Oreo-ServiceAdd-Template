/*
 * Copyright (C) 2009 The Android Open Source Project
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

#define LOG_TAG "KdcDmsiManagerService"

#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"

#include <stdlib.h>
#include <utils/misc.h>
#include <utils/Log.h>
#include <stdio.h>

#include <android/hardware/kdcdmsi/1.0/IKdcDmsi.h>

using ::android::hardware::kdcdmsi::V1_0::IKdcDmsi;
using ::android::hardware::hidl_vec;

namespace android{

typedef enum {
	READ_NV_IMEI_SUB0 = 0,
	READ_NV_IMEI_SUB1 = 1,
	READ_NV_MEID = 2,
	READ_NV_FACTORY_DATA1 = 3,
	READ_NV_FACTORY_DATA3 = 4,
}nv_cmd_enum;

static sp<IKdcDmsi> mHal;

static jboolean init_native(JNIEnv *env, jobject clazz)
{
    mHal = IKdcDmsi::getService();

    return mHal != nullptr;
}

#if 0
static void dump_data(jbyte* b, int len)
{
	for(int i=0; i<len; i++)
	{
		ALOGD("data[%d] : %d\n", i, *(b+i));
	}
}
#endif

jint kdc_read_serialno_native(JNIEnv *env, jobject clazz, jint length, jbyteArray nv_buf)
{
	int len = 0;
	int ret = 0;
	hidl_vec<uint8_t> data;
    jbyte* b = (jbyte *)env->GetPrimitiveArrayCritical(nv_buf, 0);

	auto cb = [&](hidl_vec<uint8_t> vec) {
		data = vec;
	};

	mHal->readNv(READ_NV_FACTORY_DATA1, length, cb);

    len = data.size();
	if(len > 0){
		memcpy(b, &data[0], len);
	}else{
		ret = -1;
	}

	//dump_data(b, len);
    env->ReleasePrimitiveArrayCritical(nv_buf, b, JNI_ABORT);

    return  ret;
}

jint kdc_read_imei_native(JNIEnv *env, jobject clazz, jint sub_id, jint length, jbyteArray nv_buf)
{
	int len = 0;
	int ret = 0;
	hidl_vec<uint8_t> data;
    jbyte* b = (jbyte *)env->GetPrimitiveArrayCritical(nv_buf, 0);

	auto cb = [&](hidl_vec<uint8_t> vec) {
		data = vec;
	};

	if(sub_id == 1)
		mHal->readNv(READ_NV_IMEI_SUB1, length, cb);
	else
		mHal->readNv(READ_NV_IMEI_SUB0, length, cb);

    len = data.size();
	if(len > 0){
		memcpy(b, &data[0], len);
	}else{
		ret = -1;
	}

	//dump_data(b, len);
    env->ReleasePrimitiveArrayCritical(nv_buf, b, JNI_ABORT);

	return  ret;
}

jint kdc_read_meid_native(JNIEnv *env, jobject clazz, jint length, jbyteArray nv_buf)
{
	int len = 0;
	int ret = 0;
	hidl_vec<uint8_t> data;
    jbyte* b = (jbyte *)env->GetPrimitiveArrayCritical(nv_buf, 0);

	auto cb = [&](hidl_vec<uint8_t> vec) {
		data = vec;
	};

	mHal->readNv(READ_NV_MEID, length, cb);

    len = data.size();
	if(len > 0){
		memcpy(b, &data[0], len);
	}else{
		ret = -1;
	}

	//dump_data(b, len);
    env->ReleasePrimitiveArrayCritical(nv_buf, b, JNI_ABORT);

    return  ret;
}

jint kdc_read_calibrate_result_native(JNIEnv *env, jobject clazz, jint length, jbyteArray nv_buf)
{
	int len = 0;
	int ret = 0;
	hidl_vec<uint8_t> data;
    jbyte* b = (jbyte *)env->GetPrimitiveArrayCritical(nv_buf, 0);

	auto cb = [&](hidl_vec<uint8_t> vec) {
		data = vec;
	};
	mHal->readNv(READ_NV_FACTORY_DATA3, length, cb);

    len = data.size();
	if(len > 0){
		memcpy(b, &data[0], len);
	}else{
		ret = -1;
	}

	//dump_data(b, len);
    env->ReleasePrimitiveArrayCritical(nv_buf, b, JNI_ABORT);

    return ret;
}

jint kdc_write_calibrate_result_native(JNIEnv *env, jobject clazz, jint length, jbyteArray nv_buf)
{
	bool success = false;
    hidl_vec<uint8_t> result;

    jbyte *data = env->GetByteArrayElements(nv_buf, 0);
    result.setToExternal((unsigned char *)data, length);

	success = mHal->writeCalibrateResult(length, result);

    return success ? 0 : -1;
}

static const JNINativeMethod method_table[] = {
    { "init_native", "()Z", (void*)init_native },
    { "kdc_read_serialno_native", "(I[B)I", (void*)kdc_read_serialno_native },
    { "kdc_read_imei_native", "(II[B)I", (void*)kdc_read_imei_native },
    { "kdc_read_meid_native", "(I[B)I", (void*)kdc_read_meid_native },
    { "kdc_read_calibrate_result_native", "(I[B)I", (void*)kdc_read_calibrate_result_native },
    { "kdc_write_calibrate_result_native", "(I[B)I", (void*)kdc_write_calibrate_result_native },
};

int register_android_server_KdcDmsiManagerService(JNIEnv *env)
{
    return jniRegisterNativeMethods(env, "com/android/server/KdcDmsiManagerService",
            method_table, NELEM(method_table));
}

};
