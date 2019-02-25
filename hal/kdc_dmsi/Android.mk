LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := kdc_dmsi.default
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/common/inc/
LOCAL_SRC_FILES := kdc_dmsi.c
LOCAL_SRC_FILES += kdc_dmsi_qmi_client.c
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/qmi-framework/inc
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/qmi/services
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/qmi/platform
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/qmi/inc
LOCAL_SHARED_LIBRARIES := libc libcutils libutils liblog
LOCAL_SHARED_LIBRARIES += libqmiservices libqmi_cci
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/libmdmdetect/inc
LOCAL_SHARED_LIBRARIES += libmdmdetect
LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS += -Wall -Werror
#include $(BUILD_EXECUTABLE)
include $(BUILD_SHARED_LIBRARY)
