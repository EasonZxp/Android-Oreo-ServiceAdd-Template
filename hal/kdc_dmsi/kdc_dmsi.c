#include <hardware/kdc_dmsi.h>
#include <hardware/hardware.h>
#include <cutils/log.h>
#include <cutils/properties.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <malloc.h>
#include "kdc_dmsi_qmi_client.h"

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "KdcDmsiHal"
#endif

int kdc_write_calibrate_result(const int length, char* buf)
{
	int ret = 0;

	ret = kdc_set_cal_result(length, buf);
	if(0 != ret)
	{
		ALOGE("%s failed, ret:%d\n", __func__, ret);
		return FAILED;
	}
	return SUCCESS;
}

static int kdc_dmsi_close(hw_device_t *device)
{
	if(device != NULL){
		free(device);
		device = NULL;
	}

	kdcdmsi_qmi_deinit();

	return SUCCESS;
}

#if 1
static int kdc_dmsi_open(const hw_module_t* module, const char* name,
					  hw_device_t** device) {
	int ret = 0;

	if(name != NULL){
		ALOGE("====> open name:%s\n", name);
	}
	kdc_dmsi_device_t *kdc_dmsi_dev = (kdc_dmsi_device_t *)calloc(1, sizeof(kdc_dmsi_device_t));

	if (!kdc_dmsi_dev) {
		ALOGE("Can not allocate memory for the kdc_dmsi device");
		return -ENOMEM;
	}

	ret = check_mdm_and_qmi_init();
	if(ret){
		ALOGE("check mdm invalid or qmi init failed, ret:%d\n", ret);
		return -ENOMEM;
	}

	kdc_dmsi_dev->common.tag = HARDWARE_DEVICE_TAG;
	kdc_dmsi_dev->common.module = (hw_module_t *) module;
	kdc_dmsi_dev->common.version = HARDWARE_DEVICE_API_VERSION(1,0);
	kdc_dmsi_dev->common.close = kdc_dmsi_close;
	kdc_dmsi_dev->kdc_read_nv = kdc_read_nv;
	kdc_dmsi_dev->kdc_write_calibrate_result = kdc_write_calibrate_result;

	*device = (hw_device_t *) kdc_dmsi_dev;

	return SUCCESS;
}

static struct hw_module_methods_t kdc_dmsi_module_methods = {
	.open = kdc_dmsi_open,
};

kdc_dmsi_module_t HAL_MODULE_INFO_SYM = {
	.common = {
		.tag = HARDWARE_MODULE_TAG,
		.module_api_version = 0,
		.hal_api_version = HARDWARE_HAL_API_VERSION,
		.id = KDC_DMSI_HARDWARE_MODULE_ID,
		.name = "KDC DMSI Module",
		.author = "Kedacom Inc",
		.methods = &kdc_dmsi_module_methods,
		.dso	 = NULL,
		.reserved = {0},
	},
};
#else
int main(void)
{
	int ret = 0;
	/*struct kdc_dmsi_device dev;*/
	char serialno[15] = {0x0};
	/*char imei0[15] = {0x0};*/
	/*char imei1[15] = {0x0};*/
	/*char meid[14] = {0x0};*/
	/*char calibrate_result[30] = {0x0};*/
	/*[>char test_result[30] = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";<]*/
	/*char test_result[30] = "UUUUUPPUPFPPUUUUUUFPUPPPPPPPPU";*/
	//check modem and qmi init
	ret = check_mdm_and_qmi_init();
	if(ret){
		ALOGE("check mdm invalid or qmi init failed, ret:%d\n", ret);
		return -ENOMEM;
	}

	kdc_read_serialno(15, serialno);
	ALOGE("===> serialno:%s\n",serialno);
	/*kdc_read_imei(0, 15, imei0);*/
	/*ALOGE("===> imei0:%s\n", imei0);*/
	/*kdc_read_imei(1, 15,  imei1);*/
	/*ALOGE("===> imei1:%s\n", imei1);*/
	/*kdc_read_meid(14,  meid);*/
	/*ALOGE("===> meid:%s\n", meid);*/
	/*kdc_write_calibrate_result(30, test_result);*/
	/*kdc_read_calibrate_result(30,  calibrate_result);*/
	/*ALOGE("===> factory_data1:%s\n", calibrate_result);*/

	wcnss_qmi_deinit();

	return SUCCESS;
}
#endif
