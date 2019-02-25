
#define LOG_TAG "KdcDmsiService"

#include <log/log.h>

#include <hardware/hardware.h>

#include <hardware/kdc_dmsi.h>

#include "KdcDmsi.h"

namespace android {
namespace hardware {
namespace kdcdmsi {
namespace V1_0 {
namespace implementation {

KdcDmsi::KdcDmsi(kdc_dmsi_device_t *device) : mDevice(device) {}

Return<void> KdcDmsi::readNv(int32_t nv_cmd, int32_t length, readNv_cb _hidl_cb) {

	int ret = -1;
	char *sn = (char *) malloc(length);
	KdcDmsiMetadata data;

	ret = mDevice->kdc_read_nv(nv_cmd, length, sn);
	data.setToExternal((unsigned char *)sn, length);
	if(!ret){
		_hidl_cb(data);
	}else{
		_hidl_cb({});
	}
	free(sn);

	return Void();
}

Return<bool> KdcDmsi::writeCalibrateResult(int32_t length, const hidl_vec<uint8_t>& result) {

	int ret = mDevice->kdc_write_calibrate_result(length, (char *)result.data()) == 0;

	return (ret == 0) ? true : false;
}

IKdcDmsi* HIDL_FETCH_IKdcDmsi(const char* /* name */) {
	kdc_dmsi_device_t *kdc_dmsi_device;
	const hw_module_t *hw_module = nullptr;

	int ret = hw_get_module(KDC_DMSI_HARDWARE_MODULE_ID, &hw_module);
	if (ret == 0) {
		ret = hw_module->methods->open(hw_module, KDC_DMSI_HARDWARE_MODULE_ID, (hw_device_t **)&kdc_dmsi_device);
		if (ret != 0) {
			ALOGE("kdc_dmsi_open failed: %d", ret);
		}
	} else {
		ALOGE("hw_get_module %s failed: %d", KDC_DMSI_HARDWARE_MODULE_ID, ret);
	}

	if (ret == 0) {
		return new KdcDmsi(kdc_dmsi_device);
	} else {
		ALOGE("Passthrough failed to open legacy HAL.");
		return nullptr;
	}

}

}  // namespace implementation
}  // namespace V1_0
}  // namespace kdcdmsi
}  // namespace hardware
}  // namespace android
