
package android.os;

import android.os.RemoteException;
import android.os.IKdcDmsiManager;
import android.util.Log;

public class KdcDmsiManager {
    private IKdcDmsiManager mService;
    public static final String TAG = "KdcDmsiManager";
    // serialno
    public static final int NV_FACTORY_DATA_1_I = 2497;
    public static final int NV_FACTORY_DATA_1_I_SIZE = 15;
    // wifi mac
    public static final int NV_WIFI_MAC_I = 4678;
    public static final int NV_WIFI_MAC_I_SIZE = 6;
    // bt mac
    public static final int NV_BT_MAC_I = 447;
    public static final int NV_BT_MAC_I_SIZE = 6;
    // factory test result
    public static final int NV_FACTORY_DATA_3_I = 2499;
    public static final int NV_FACTORY_DATA_3_I_SIZE = 30;
    // meid
    public static final int NV_MEID_I = 1943;
    public static final int NV_MEID_I_SIZE = 14;
    // imei
    public static final int NV_IMEI_I = 550;
    public static final int NV_IMEI_I_SIZE = 15;

    public static final int RESULT_FAILED = 0;
    public static final int RESULT_OK = 1;
    public static final int RESULT_AREADY_BEFORE = 2;
    public static final int RESULT_READ_NEXT_TIME = 3;
    public static final int RESULT_FAILED_FINAL = 4;

    public KdcDmsiManager(IKdcDmsiManager server) {
        mService = server;
    }

    public String getSnNumber() {
        byte mSnNumber[] = new byte[NV_FACTORY_DATA_1_I_SIZE];
        try {
            if (mService == null) {
                return null;
            }
            if(0 == mService.readSerialno(NV_FACTORY_DATA_1_I_SIZE, mSnNumber)){
                return new String(mSnNumber);
            }
        } catch (RemoteException e) {
        }
        return null;
    }

    public String getImei(int slotId) {
        byte mImei[] = new byte[NV_IMEI_I_SIZE];
        try {
            if (mService == null) {
                return null;
            }
            if(0 == mService.readImei(slotId, NV_FACTORY_DATA_1_I_SIZE, mImei)){
                return new String(mImei);
            }
        } catch (RemoteException e) {
        }
        return null;
    }

    public String getMeid() {
        byte mMeid[] = new byte[NV_MEID_I_SIZE];
        try {
            if (mService == null) {
                return null;
            }
            if(0 == mService.readMeid(NV_MEID_I_SIZE, mMeid)){
                return new String(mMeid);
            }
        } catch (RemoteException e) {
        }
        return null;
    }

    public String getFactoryTestResult() {
        byte mTestResult[] = new byte[NV_FACTORY_DATA_3_I_SIZE];
        try {
            if (mService == null) {
                return null;
            }
            if(0 == mService.readCalResult(NV_FACTORY_DATA_3_I_SIZE, mTestResult)){
                return new String(mTestResult);
            }
        } catch (RemoteException e) {
        }
        return null;
    }

    public int setFactoryTestResult(String testResult) {
        try {
            if (mService == null) {
                return -1;
            }
            return mService.writeCalResult(NV_FACTORY_DATA_3_I_SIZE, testResult.getBytes());
        } catch (RemoteException e) {
        }
        return 0;
    }

    public int readAndSaveImei(int tryTime) {
        String imeiStr = SystemProperties.get("persist.sys.imei", "0,0");
        Log.d(TAG, "readAndSaveImei start  imeiStr = " + imeiStr);
        boolean isUpdateImei = SystemProperties.getBoolean("persist.sys.updateimei", false);
        if (imeiStr.equals("0,0") || isUpdateImei) {
            String imei0 = getImei(0);
            String imei1 = getImei(1);
            Log.d(TAG, "readAndSaveImei imei0 = "+ imei0);
            Log.d(TAG, "readAndSaveImei imei1 = "+ imei1);

            if (imei0 != null && imei1 != null
                    && ("000000000000000".equals(imei0)
                            ||"000000000000000".equals(imei1) )) {
                Log.d(TAG, "readAndSaveImei read again when reboot");
                return RESULT_READ_NEXT_TIME;
            }

            if (imei0 != null && imei1 != null && imei0.length() == 15 && imei1.length() == 15) {
                SystemProperties.set("persist.sys.imei", imei0 + ","+ imei1);
                Log.d(TAG, "readAndSaveImei SUCCESS ");
                return RESULT_OK;
            }

        } else if (!imeiStr.equals("0,0")) {
            Log.d(TAG, "readAndSaveImei WAS ready imeiStr =" + imeiStr);
            return RESULT_AREADY_BEFORE;
        }
        Log.d(TAG, "readAndSaveImei failed  tryTime = " + tryTime);
        if(tryTime >= 3) {
            return RESULT_FAILED_FINAL;
        }
        return RESULT_FAILED;
    }
}
