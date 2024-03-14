package com.example.AndroidAttack;


import android.app.Activity;
import android.os.Bundle;





public class MainActivity extends Activity {
	
	static{
		System.loadLibrary("soloader");
	}

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }


}
