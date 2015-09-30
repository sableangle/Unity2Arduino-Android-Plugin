package com.unity.test;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class List extends Activity {
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
        setContentView(R.layout.list);
        
        Bundle bundle = this.getIntent().getExtras();
		
        TextView view_result = (TextView) findViewById(R.id.ListText);
        view_result.setText(bundle.getString("data"));
	}
	@Override
	public void onBackPressed() {
	    moveTaskToBack(true);
	}
}
