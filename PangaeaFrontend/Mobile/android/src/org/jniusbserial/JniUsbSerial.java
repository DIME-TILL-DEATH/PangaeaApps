package org.jniusbserial;

import java.util.HashMap;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.io.IOException;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.pm.PackageManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.*;
import android.widget.Toast;
import java.nio.charset.StandardCharsets;

import com.hoho.android.usbserial.driver.*;
import org.jniusbserial.SerialInputOutputManager;

public class JniUsbSerial
{
    private static UsbManager usbManager;
    private static HashMap<String, UsbSerialPort> m_usbSerialPort;
    private static HashMap<String, SerialInputOutputManager> m_usbIoManager;

    private static Activity m_activity = null;

    private static native void nativeDeviceException(long classPoint, String messageA);
    private static native void nativeDeviceNewData(long classPoint, byte[] dataA);

    private static final String ACTION_USB_PERMISSION = "pangaea-mobile.USB_PERMISSION";
    private static String m_currentPortName;
    private static long m_currentClassPoint;

    private static final BroadcastReceiver usbReceiver = new BroadcastReceiver() {

        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (ACTION_USB_PERMISSION.equals(action)) {
                synchronized (this) {
                    UsbDevice device = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

                    if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                       if(device != null){
                            //open(m_currentPortName, m_currentClassPoint);
                       }
                    }
                    else {
                        //Log.d(TAG, "permission denied for device " + device);
                    }
                }
            }
        }
    };

    public JniUsbSerial()
    {
        //m_instance = this;
        m_usbIoManager = new HashMap<String, SerialInputOutputManager>();
        m_usbSerialPort = new HashMap<String, UsbSerialPort>();
    }

    public static boolean hasCapable(Activity activity)
    {
        PackageManager packageManager = activity.getPackageManager();
        return packageManager.hasSystemFeature(PackageManager.FEATURE_USB_HOST);
    }

    public static void setActivity(Activity activity)
    {
        m_activity = activity;

        IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
        m_activity.registerReceiver(usbReceiver, filter);
    }

    private static boolean getCurrentDevices()
    {
        if (m_activity == null)
            return false;

        if (usbManager == null)
            usbManager = (UsbManager)m_activity.getSystemService(Context.USB_SERVICE);
        return true;
    }

    public static String[] availableDevicesInfo()
    {
        //  GET THE LIST OF CURRENT DEVICES
        if (!getCurrentDevices())
            return null;

        if (usbManager.getDeviceList().size() < 1)
            return null;

        String[] listL = new String[usbManager.getDeviceList().size()];
        String tempL;

        UsbSerialProber usbDefaultProber = UsbSerialProber.getDefaultProber();
        int countL = 0;
        for(UsbDevice deviceL : usbManager.getDeviceList().values()) {

            UsbSerialDriver driverL = usbDefaultProber.probeDevice(deviceL);
            tempL = deviceL.getDeviceName() + ":";

            if (driverL == null) {
                tempL = tempL + "Unknown:";
            }
            else if (driverL instanceof CdcAcmSerialDriver)
            {
                tempL = tempL + "Cdc Acm:";
            }
            else if (driverL instanceof Ch34xSerialDriver)
            {
                tempL = tempL + "Ch34x:";
            }
            else if (driverL instanceof CommonUsbSerialPort)
            {
                tempL = tempL + "CommonUsb:";
            }
            else if (driverL instanceof Cp21xxSerialDriver)
            {
                tempL = tempL + "Cp21xx:";
            }
            else if (driverL instanceof FtdiSerialDriver)
            {
                tempL = tempL + "Ftdi:";
            }
            else if (driverL instanceof ProlificSerialDriver)
            {
                tempL = tempL + "Prolific:";
            }

            tempL = tempL + deviceL.getManufacturerName() + ":";

            tempL = tempL + Integer.toString(deviceL.getProductId()) + ":";
            tempL = tempL + Integer.toString(deviceL.getVendorId()) + ":";

            listL[countL] = tempL;
            countL++;
        }

        return listL;
    }

    public static boolean setParameters(String portNameA, int baudRateA, int dataBitsA, int stopBitsA, int parityA)
    {
        if (m_usbSerialPort.size() <= 0)
            return false;

        if (m_usbSerialPort.get(portNameA) == null)
            return false;

        try
        {
            m_usbSerialPort.get(portNameA).setParameters(baudRateA, dataBitsA, stopBitsA, parityA);
            return true;
        }
        catch(IOException eA)
        {
            return false;
        }
    }

    public static void stopIoManager(String portNameA)
    {
        if (m_usbIoManager.get(portNameA) == null)
            return;

        m_usbIoManager.get(portNameA).stop();
        m_usbIoManager.remove(portNameA);
    }

    public static void startIoManager(String portNameA, long classPoint)
    {
        if (m_usbSerialPort.get(portNameA) == null)
            return;

        SerialInputOutputManager usbIoManager = new SerialInputOutputManager(m_usbSerialPort.get(portNameA), m_Listener, classPoint);

        m_usbIoManager.put(portNameA, usbIoManager);
        m_usbIoManager.get(portNameA).start();
    }

    public static boolean close(String portNameA)
    {
        if (m_usbSerialPort.get(portNameA) == null)
            return false;

        try
        {
            stopIoManager(portNameA);
            m_usbSerialPort.get(portNameA).close();
            m_usbSerialPort.remove(portNameA);

            return true;
        }
        catch (IOException eA)
        {
            return false;
        }
    }

    public static int open(String portNameA, long classPoint)
    {
        m_currentPortName = portNameA;
        m_currentClassPoint = classPoint;

        if (!getCurrentDevices())
            return 0;

        if (usbManager.getDeviceList().size() < 1)
            return 0;

        if (m_usbSerialPort.get(portNameA) != null)
            return 0;

        UsbSerialProber usbDefaultProber = UsbSerialProber.getDefaultProber();
        for(UsbDevice deviceL : usbManager.getDeviceList().values()) {

            if (portNameA.equals(deviceL.getDeviceName()))
            {
            }
            else
            {
                continue;
            }

            UsbSerialDriver driverL = usbDefaultProber.probeDevice(deviceL);
            if (driverL == null)
            {
                return 0;
            }

            UsbDevice serialDevice = driverL.getDevice();

            if(usbManager.hasPermission(serialDevice))
            {
                UsbDeviceConnection connectionL = usbManager.openDevice(serialDevice);
                if (connectionL == null) {
                    return 5;
                }

                UsbSerialPort usbSerialPort = driverL.getPorts().get(0);

                try{
                    usbSerialPort.open(connectionL);
                    m_usbSerialPort.put(portNameA, usbSerialPort);

                    startIoManager(portNameA, classPoint);
                    return 127;
                }
                catch (Exception e) {
                    m_usbSerialPort.remove(portNameA);
                    stopIoManager(portNameA);
                    return 0;
                }
            }
            else
            {
                PendingIntent pi = PendingIntent.getBroadcast(m_activity, 0, new Intent(ACTION_USB_PERMISSION), PendingIntent.FLAG_IMMUTABLE);
                usbManager.requestPermission(serialDevice, pi);
                return 7;
            }
        }

        return 0;
    }

    public static int write(String portNameA, byte[] sourceA, int timeoutMSecA)
    {
        if (m_usbSerialPort.get(portNameA) == null)
            return 0;

        try
        {
            System.out.println("Serial write:" + new String(sourceA, StandardCharsets.UTF_8));
            m_usbSerialPort.get(portNameA).write(sourceA, timeoutMSecA);
        }
        catch (IOException eA)
        {
            return 0;
        }

        return 1;
    }

    // SerialInputOutputManager.Listener

    private final static SerialInputOutputManager.Listener m_Listener =
    new SerialInputOutputManager.Listener()
    {
        @Override
        public void onNewData(byte[] data, long classPoint) {
            nativeDeviceNewData(classPoint, data);
            System.out.println("Serial read:" + new String(data, StandardCharsets.UTF_8));
        };

        @Override
        public void onRunError(Exception e, long classPoint) {
            nativeDeviceException(classPoint, e.getMessage());
        }
    };
    //
}
