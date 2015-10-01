package com.unity.test;
import com.unity3d.player.UnityPlayerActivity;

import java.io.OutputStream;

import com.unity3d.player.UnityPlayer;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Vibrator;
import android.view.ContextThemeWrapper;
import android.widget.ArrayAdapter;
import android.widget.Toast;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;

public class UnityPlayerExtendActivity extends UnityPlayerActivity {

	// Intent request codes	
    private static final int REQUEST_ENABLE_BT = 3;
	
	
	public static String EXTRA_DEVICE_ADDRESS = "device_address";

	private BluetoothAdapter mBluetoothAdapter = null;
	private Context mContext=null; 
	private Thread mThread = null;

	
    private ArrayAdapter<String> mConversationArrayAdapter;

	
	  /**
	 * Member object for the chat services
	 */
    private BluetoothService mChatService = null;
    /**
     * String buffer for outgoing messages
     */
    private StringBuffer mOutStringBuffer;
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
	
		super.onCreate(savedInstanceState);
		mContext = this;
		
		mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter(); 
		

        // If the adapter is null, then Bluetooth is not supported
        if (mBluetoothAdapter == null) {
        	CallToast("No Bluetooth Found");
            this.finish();
        }
		
		
		Toast.makeText(mContext,		
				"Android Plugin Start OK!", // and re-run this program
				Toast.LENGTH_LONG).show();

		
	}
	
	
	@Override
    public void onStart() {
        super.onStart();
        // If BT is not on, request that it be enabled.
        // setupChat() will then be called during onActivityResult
        if (!mBluetoothAdapter.isEnabled()) {
        	Intent enabler = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enabler, REQUEST_ENABLE_BT);
            // Otherwise, setup the chat session
        } else if (mChatService == null) {
            setupChat();
        }
    }

	
	  /**
	 * Set up the UI and background operations for chat.
	 */
    private void setupChat() {

        // Initialize the BluetoothChatService to perform bluetooth connections
        mChatService = new BluetoothService(this, mHandler);

        // Initialize the buffer for outgoing messages
        mOutStringBuffer = new StringBuffer("");
    }

	
	
	
	///For Unity Methods ///

	public static void InvokeUnity(String methodName,String mStr) {  
       UnityPlayer.UnitySendMessage("Main Camera",methodName,mStr );  
    }  
	
	public void SetDiscoverable(){
		Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);    
		startActivityForResult(intent,0);   
	}
	
	public void SetBluedeviceOn(){
		Intent enabler=new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
		startActivityForResult(enabler, 0);
	}
	
	public void SetBluedeviceOff(){
		mBluetoothAdapter.disable();
	}
	
	 public void ShowDialog(final String mTitle,final String mContent) {  
		 runOnUiThread(new Runnable()  
	        {  
	            @Override  
	            public void run()   
	            {  
	                //创建Builder  
	                AlertDialog.Builder mBuilder=new AlertDialog.Builder(new ContextThemeWrapper(UnityPlayerExtendActivity.this, R.style.AlertDialogCustom));  
	                //创建对话框  
	                mBuilder.setTitle(mTitle)  
	                        .setMessage(mContent)
	                		.setPositiveButton("确定", 
	                			new DialogInterface.OnClickListener(){
		                            public void onClick(
		                                DialogInterface dialoginterface, int i){
		                            		Toast.makeText(mContext, R.string.ok_press, Toast.LENGTH_LONG).show();		                            	}
		                        }
	                		)
	                        .setNegativeButton("關閉", 
	                        	new DialogInterface.OnClickListener(){
		                            public void onClick(
		                                DialogInterface dialoginterface, int i){
		                            		Toast.makeText(mContext, R.string.close_press, Toast.LENGTH_LONG).show();
		                            }
	                        	}
	                        );  
	                //显示对话框  
	                mBuilder.show();  
	            }  
	        });  
	}  
	
	public void SendBluetoothMsgForUnity(String msg){
		sendBluetoothMessage(msg);
	}
	 
    public void SetVibrator(long mTime){  
        Vibrator mVibrator=(Vibrator)getSystemService(VIBRATOR_SERVICE);  
        mVibrator.vibrate(mTime);  
    }  
	
	public void CallToast(final String a){
		runOnUiThread(new Runnable() { @Override public void run() { Toast.makeText(mContext, a, Toast.LENGTH_LONG).show(); } });
	}
	
	public void OpenScanPanel(){
		Intent intent = new Intent();
		intent.setClass(this, DeviceListActivity.class);
		startActivityForResult(intent, 1);
	}
	
	///For Unity Methods End ///

	
	
    private void connectDevice(Intent data) {
        
        try{
        	
            // Get the device MAC address
            String address = data.getExtras()
                    .getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
            CallToast("Start Connecting!" + address);
            // Get the BluetoothDevice object
            BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
        	
            mChatService.connect(device, BluetoothService.BLUETOOTH_SERIALPORT);
            
            CallToast("Try Connect finish!");

        }
        catch(Exception ex){
        	CallToast("Connect Exception!" + ex.getMessage());
        }
        // Attempt to connect to the device
        
    }
    
    
    
    /**
     * Sends a message.
     *
     * @param message A string of text to send.
     */
    private void sendBluetoothMessage(String message) {
        // Check that we're actually connected before trying anything
        if (mChatService.getState() != BluetoothService.STATE_CONNECTED) {
        	CallToast("No Connection");
            return;
        }

        // Check that there's actually something to send
        if (message.length() > 0) {
            // Get the message bytes and tell the BluetoothChatService to write
            byte[] send = message.getBytes();
            mChatService.write(send);

            // Reset out string buffer to zero and clear the edit text field
            mOutStringBuffer.setLength(0);
        }
    }

    
    /**
     * The Handler that gets information back from the BluetoothChatService
     */
    private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            //FragmentActivity activity = getActivity();
            switch (msg.what) {
                case Constants.MESSAGE_TOAST:
                    if (null != this) {
                    	CallToast(msg.getData().getString(Constants.TOAST));
                    }
                    break;
                case Constants.MESSAGE_READ:
                    byte[] readBuf = (byte[]) msg.obj;
                    // construct a string from the valid bytes in the buffer
                    String readMessage = new String(readBuf, 0, msg.arg1);
                	CallToast(readMessage);
                	InvokeUnity("MsgFromAndroidThread",readMessage);
                    break;
                case Constants.MESSAGE_DEVICE_NAME:
                	String devicename = "Connected To " + msg.getData().getString(Constants.DEVICE_NAME);
                	CallToast(devicename);
                	InvokeUnity("MsgFromAndroidService",devicename);
                	break;
            }
        }
    };
    
    
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
    	
    	 if (resultCode == RESULT_OK) { 
    		 connectDevice(data);
         }
    }

	
}