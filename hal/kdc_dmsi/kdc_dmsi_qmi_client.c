#define LOG_TAG "kdc_dmsi_qmi"
#include <cutils/log.h>
#include "kdc_dmsi_qmi_client.h"
#include "qmi_client.h"
#include "device_management_service_v01.h"
#include "mdm_detect.h"
#include <cutils/properties.h>
#include <string.h>


#define DMS_QMI_TIMEOUT (2000)

static qmi_client_type dms_qmi_client;
static int dms_init_done = FAILED;

int kdc_read_nv(int nv_cmd, int length, char* buf)
{
	int i = 0;

	qmi_client_error_type qmi_client_err;
	dms_kdc_read_nv_req_msg_v01 sn_req;
	dms_kdc_read_nv_resp_msg_v01 sn_resp;

	if ((dms_init_done == FAILED) || (buf == NULL)) {
		ALOGE("Eason : %s: DMS init failed", __func__);
		return FAILED;
	}

	memset(&sn_req, 0, sizeof(sn_req));

	sn_req.item_cmd = nv_cmd;
	qmi_client_err = qmi_client_send_msg_sync(dms_qmi_client,
		QMI_DMS_KDC_READ_NV_REQ_V01, &sn_req, sizeof(sn_req),
		&sn_resp, sizeof(sn_resp), DMS_QMI_TIMEOUT);

	if (qmi_client_err != QMI_NO_ERR){
		ALOGE("[Eason] %s: Failed to get Rsp from Modem Error:%d",
				__func__, qmi_client_err);
		return FAILED;
	}
#if 0
	ALOGE("[Eason] %s: imei0_valid:%d, imei0:%s, imei1_valid:%d, imei1:%s, meid_valid: %d meid:%s, factory_data1_valid:%d, factory_data1:%s, factory_data3_valid:%d, factory_data3:%s",
				__func__, sn_resp.imei0_valid, sn_resp.imei0, sn_resp.imei1_valid, sn_resp.imei1, sn_resp.meid_valid, sn_resp.meid, \
				sn_resp.factory_data1_valid, sn_resp.factory_data1, \
				sn_resp.factory_data3_valid, sn_resp.factory_data3);
#endif
	switch(nv_cmd){
	case DMS_KDC_READ_NV_IMEI_SUB0:
		for(i=0; i<length; i++){
			memcpy((void *)buf, (void *)(sn_resp.imei0), length);
		}
		break;
	case DMS_KDC_READ_NV_IMEI_SUB1:
		for(i=0; i<length; i++){
			memcpy((void *)buf, (void *)(sn_resp.imei1), length);
		}
		break;
	case DMS_KDC_READ_NV_MEID:
		for(i=0; i<length; i++){
			memcpy((void *)buf, (void *)(sn_resp.meid), length);
		}
		break;
	case DMS_KDC_READ_NV_FACTORY_DATA1:
		for(i=0; i<length; i++){
			memcpy((void *)buf, (void *)(sn_resp.factory_data1), length);
		}
		break;
	case DMS_KDC_READ_NV_FACTORY_DATA3:
		for(i=0; i<length; i++){
			memcpy((void *)buf, (void *)(sn_resp.factory_data3), length);
		}
		break;
	default:
		break;
	}
	return 0;
}

static int kdcdmsi_init_qmi()
{
	qmi_client_error_type qmi_client_err;

	ALOGE("%s: Initialize kdcdmsi QMI Interface", __func__);
        qmi_client_os_params dms_os_params;

	memset(&dms_os_params, 0, sizeof(qmi_client_os_params));
	qmi_client_err = qmi_client_init_instance(dms_get_service_object_v01(),
			QMI_CLIENT_INSTANCE_ANY, NULL, NULL,
			&dms_os_params, 5000, &dms_qmi_client);

	if (qmi_client_err != QMI_NO_ERR){
		ALOGE("%s: Error while Initializing QMI Client: %d",
			__func__, qmi_client_err);
		goto exit;
	}

	dms_init_done = SUCCESS;
	return SUCCESS;

exit:
	return FAILED;
}

int kdc_set_cal_result(const int length, char *buf)
{
	qmi_client_error_type qmi_client_err;
	dms_kdc_set_cal_result_req_msg_v01 sn_req;
	dms_kdc_set_cal_result_resp_msg_v01 sn_resp;

	if (dms_init_done == FAILED) {
		ALOGE("Eason : %s: DMS init fail or pBdAddr is NULL", __func__);
		return FAILED;
	}

	memset(&sn_req, 0, sizeof(sn_req));
	memcpy(sn_req.cal_result, (void *)buf, length);

	qmi_client_err = qmi_client_send_msg_sync(dms_qmi_client,
		QMI_DMS_KDC_SET_CAL_RESULT_REQ_V01, &sn_req, sizeof(sn_req),
		&sn_resp, sizeof(sn_resp), DMS_QMI_TIMEOUT);

	if (qmi_client_err != QMI_NO_ERR){
		ALOGE("[Eason] %s: Failed to get Rsp from Modem Error:%d",
				__func__, qmi_client_err);
		return FAILED;
	}
	return SUCCESS;
}

void kdcdmsi_qmi_deinit()
{
	qmi_client_error_type qmi_client_err;

	ALOGE("%s: Deinitialize kdcdmsi QMI Interface", __func__);

	if (dms_init_done == FAILED) {
		ALOGE("%s: DMS Service was not Initialized", __func__);
		return;
	}

	qmi_client_err = qmi_client_release(dms_qmi_client);

	if (qmi_client_err != QMI_NO_ERR){
		ALOGE("%s: Error while releasing qmi_client: %d",
			__func__, qmi_client_err);
	}

	dms_init_done = FAILED;
}

int check_modem_compatability(struct dev_info *mdm_det_info)
{
	char args[MODEM_BASEBAND_PROPERTY_SIZE] = {0};
	int ret = 0;

	ret = property_get(MODEM_BASEBAND_PROPERTY, args, "");
	if (ret > MODEM_BASEBAND_PROPERTY_SIZE) {
		ALOGE("property [%s] has size [%d] that exceeds max [%d]",
				MODEM_BASEBAND_PROPERTY, ret, MODEM_BASEBAND_PROPERTY_SIZE);
		return SUCCESS;
	}

	if(!strncmp(MODEM_BASEBAND_VALUE_APQ, args, 3)) {

		for (ret = 0; ret < mdm_det_info->num_modems; ret++) {
			if (mdm_det_info->mdm_list[ret].type == MDM_TYPE_EXTERNAL) {
				ALOGE("Hardware supports external modem");
				return 1;
			}
		}
		ALOGE("Hardware does not support external modem");
		return SUCCESS;
	}
	return 1;
}

int check_mdm_and_qmi_init()
{
	int nom = 0;
	struct dev_info mdm_detect_info;

	nom = get_system_info(&mdm_detect_info);
	if (nom > 0){
		ALOGE("Failed to get system info, ret %d", nom);
	}

	if (mdm_detect_info.num_modems == 0) {
		ALOGE("kdc_dmsi: No Modem support for this target"
			  " number of modems is %d", mdm_detect_info.num_modems);
		goto nomodem;
	}

	ALOGE("kdc_dmsi: num_modems = %d", mdm_detect_info.num_modems);

	if(!check_modem_compatability(&mdm_detect_info)) {
		ALOGE("kdc_dmsi: Target does not have external modem");
		goto nomodem;
	}

	if (SUCCESS != kdcdmsi_init_qmi()) {
		ALOGE("kdc_dmsi: Failed to Initialize kdcdmsi QMI Interface");
		return -2;
	}
	return SUCCESS;
nomodem:
	return FAILED;
}

