package future_ds.usb_native;

import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.widget.Toast;

public class UsbHelper extends BroadcastReceiver {

    private UsbManager um;
    private static final String ACTION_USB_PERMISSION = "sonicdebris.usb.test.USB_PERMISSION";
    private PendingIntent permissionIntent;

    private MainActivity listener;

    UsbHelper(MainActivity act) {
        listener  = act;
        register(act);
    }

    public void register(Context c) {

        // register for device dis/connection events:
        IntentFilter iFilter = new IntentFilter();
        iFilter.addAction(UsbManager.ACTION_USB_DEVICE_ATTACHED);
        iFilter.addAction(UsbManager.ACTION_USB_DEVICE_DETACHED);
        c.registerReceiver(this,iFilter);

        // register for request permission response:
        um = (UsbManager) c.getSystemService(Context.USB_SERVICE);
        IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
        c.registerReceiver(this, filter);

        // prepare intent to ask for permission:
        permissionIntent = PendingIntent.getBroadcast(c, 0, new Intent(ACTION_USB_PERMISSION), 0);
    }

    private UsbDevice deviceFromIntent(Intent i) {
        return i.getParcelableExtra(UsbManager.EXTRA_DEVICE);

    }

    @Override
    public void onReceive(Context c, Intent intent) {
        String act = intent.getAction();
        if (act.equals(UsbManager.ACTION_USB_DEVICE_ATTACHED)) {
            UsbDevice dev = deviceFromIntent(intent);
            if (null!=dev)
                requestPermission(dev);
        }
        else if (act.equals(UsbManager.ACTION_USB_DEVICE_DETACHED)) {
            Toast.makeText(c,"USB device detached",Toast.LENGTH_SHORT).show();
        }
        else if (act.equals(ACTION_USB_PERMISSION)) {
            UsbDevice dev = deviceFromIntent(intent);

            if (dev != null && intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                if(dev != null){
                    Toast.makeText(c,"Permission granted",Toast.LENGTH_SHORT).show();
                    UsbDeviceConnection connection = um.openDevice(dev);

                    listener.testUsbDevice(connection.getFileDescriptor(),dev.getDeviceName());
                }
            }

        }
    }

    public void requestPermission(UsbDevice dev) {
        um.requestPermission(dev,permissionIntent);
    }

}
