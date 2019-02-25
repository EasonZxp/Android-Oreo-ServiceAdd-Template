#if defined(__BIONIC_FORTIFY)
#include <sys/system_properties.h>
#endif

#define MODEM_BASEBAND_PROPERTY   "ro.baseband"
#if defined(__BIONIC_FORTIFY)
#define MODEM_BASEBAND_PROPERTY_SIZE  PROP_VALUE_MAX
#else
#define MODEM_BASEBAND_PROPERTY_SIZE  10
#endif
#define MODEM_BASEBAND_VALUE_APQ  "apq"

#ifndef WCNSS_QMI_CLIENT_H
#define WCNSS_QMI_CLIENT_H

#define SUCCESS 0
#define FAILED -1

typedef enum {
	READ_NV_IMEI_SUB0 = 0,
	READ_NV_IMEI_SUB1 = 1,
	READ_NV_MEID = 2,
	READ_NV_FACTORY_DATA1 = 3,
	READ_NV_FACTORY_DATA3 = 4,
}nv_cmd_enum;


int check_mdm_and_qmi_init();
void kdcdmsi_qmi_deinit();
int kdc_read_nv(const int nv_cmd, const int length, char* buf);
int kdc_set_cal_result(const int length, char *buf);

#endif
