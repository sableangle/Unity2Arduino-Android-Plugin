package com.unity.test;

import com.unity3d.player.UnityPlayer;

import android.app.Service;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;

public class TestService extends Service {
	private int i = 0;
	private Handler handler = new Handler();
	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return null;
	}

	
	
	@Override
	public void onStart(Intent intent,int startId){
		handler.postDelayed(showTime, 1000);
		super.onStart(intent, startId);
	}
	@Override
    public void onDestroy() {
        handler.removeCallbacks(showTime);
        super.onDestroy();
    }
	
	private Runnable showTime = new Runnable() {
        public void run() {
        	i++;
            UnityPlayer.UnitySendMessage("Main Camera","MsgFromAndroidService","Message from Java Service:"+Integer.toString(i));
        	handler.postDelayed(this, 1000);
        }
    };
}
