#ifndef ANDROID_HARDWARE_KDCDMSI_V1_0_KDCDMSI_H
#define ANDROID_HARDWARE_KDCDMSI_V1_0_KDCDMSI_H

#include <android/hardware/kdcdmsi/1.0/IKdcDmsi.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

#include <hardware/kdc_dmsi.h>

namespace android {
namespace hardware {
namespace kdcdmsi {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

using ::android::hardware::kdcdmsi::V1_0::IKdcDmsi;
using ::android::hardware::Return;

struct KdcDmsi : public IKdcDmsi {

	KdcDmsi(kdc_dmsi_device_t *device);
	Return<void> readNv(int32_t nv_cmd, int32_t length, readNv_cb _hidl_cb) override;
	Return<bool> writeCalibrateResult(int32_t length, const hidl_vec<uint8_t>& result) override;

private:
	kdc_dmsi_device_t    *mDevice;
};

// FIXME: most likely delete, this is only for passthrough implementations
extern "C" IKdcDmsi* HIDL_FETCH_IKdcDmsi(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace kdcdmsi
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_KDCDMSI_V1_0_KDCDMSI_H
