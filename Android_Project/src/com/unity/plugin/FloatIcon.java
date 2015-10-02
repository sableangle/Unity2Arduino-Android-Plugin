package com.unity.plugin;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.graphics.PixelFormat;
import android.graphics.drawable.BitmapDrawable;
import android.os.IBinder;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.PopupWindow;

public class FloatIcon extends Service {
	  private LinearLayout menuBody;
	  private WindowManager windowManager;
	  private ImageView chatHead;
	  WindowManager.LayoutParams params ;
	  WindowManager.LayoutParams params_menu ;

	  private int posX;
	  private int posY;
	  @Override public IBinder onBind(Intent intent) {
	    // Not used
	    return null;
	  }
	  
	  @Override public void onCreate() {
	    super.onCreate();

	    windowManager = (WindowManager) getSystemService(WINDOW_SERVICE);

	    chatHead = new ImageView(this);
	    chatHead.setImageResource(R.drawable.ic_launcher);

	    params = new WindowManager.LayoutParams(
	        WindowManager.LayoutParams.WRAP_CONTENT,
	        WindowManager.LayoutParams.WRAP_CONTENT,
	        WindowManager.LayoutParams.TYPE_PHONE,
	        WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
	        PixelFormat.TRANSLUCENT);
	    
	    params_menu = new WindowManager.LayoutParams(
		        WindowManager.LayoutParams.WRAP_CONTENT,
		        WindowManager.LayoutParams.WRAP_CONTENT,
		        WindowManager.LayoutParams.TYPE_PHONE,
		        WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
		        PixelFormat.TRANSLUCENT);

	    /*params.gravity = Gravity.TOP | Gravity.LEFT;
	    params.x = 0;
	    params.y = 100;*/

	    
	    chatHead.setOnTouchListener(new View.OnTouchListener() {
	    	  private int initialX;
	    	  private int initialY;
	    	  private float initialTouchX;
	    	  private float initialTouchY;

	    	  @Override public boolean onTouch(View v, MotionEvent event) {
	    	    switch (event.getAction()) {
	    	      case MotionEvent.ACTION_DOWN:
	    	        initialX = params.x;
	    	        initialY = params.y;
	    	        initialTouchX = event.getRawX();
	    	        initialTouchY = event.getRawY();
	    	        return true;
	    	      case MotionEvent.ACTION_UP:
	    	        return true;
	    	      case MotionEvent.ACTION_MOVE:
	    	        params.x = initialX + (int) (event.getRawX() - initialTouchX);
	    	        params.y = initialY + (int) (event.getRawY() - initialTouchY);
	    	        posX = params.x;
	    	        posY = params.y;
	    	        windowManager.updateViewLayout(chatHead, params);
	    	        return true;
	    	    }
	    	    return false;
	    	  }
	    	  
	    	});
	    
		    chatHead.setOnClickListener(new View.OnClickListener() {
				@Override
				public void onClick(View v) {
					// TODO Auto-generated method stub
					ShowPopupWindow(chatHead);
				}
			});
	    
	    windowManager.addView(chatHead, params);
	  }
	  

	  
	  private void ShowPopupWindow(View anchor) {
		   
		 LayoutInflater mLayoutInflater = (LayoutInflater) this.getSystemService(LAYOUT_INFLATER_SERVICE);
		 View v_pop = mLayoutInflater.inflate(R.layout.bluetooth_menu, null);
		 PopupWindow popup = new PopupWindow(v_pop,200,300);
		 
		 
		 // 使其聚集  
		 popup.setFocusable(true);  
        // 设置允许在外点击消失  
		 popup.setOutsideTouchable(true);  
  
        // 这个是为了点击“返回Back”也能使其消失，并且并不会影响你的背景  
		 popup.setBackgroundDrawable(new BitmapDrawable());  
        WindowManager windowManager = (WindowManager) getSystemService(Context.WINDOW_SERVICE);  
        // 显示的位置为:屏幕的宽度的一半-PopupWindow的高度的一半  
        int xPos = windowManager.getDefaultDisplay().getWidth() / 2  
                - popup.getWidth() / 2;    
        popup.showAsDropDown(anchor, xPos, 0);  
  
        /*lv_group.setOnItemClickListener(new OnItemClickListener() {  
  
            @Override  
            public void onItemClick(AdapterView<?> adapterView, View view,  
                    int position, long id) {  

                if (popup != null) {  
                	popup.dismiss();  
                }  
            }  
        });  
	 */
		 
		 popup.setBackgroundDrawable(new BitmapDrawable());
		 
		 
	  }

	  
	  
	  
	  @Override
	  public void onDestroy() {
	    super.onDestroy();
	    if (chatHead != null) windowManager.removeView(chatHead);
	  }
	}
