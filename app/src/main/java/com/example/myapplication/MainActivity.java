package com.example.myapplication;

import android.content.Context;
import android.os.Bundle;
import android.os.Environment;
import android.os.storage.StorageManager;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
    }


    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native long testAlloc();

    public native void testFree(long ptr);

    private long mptr = 0;

    static  byte[] bytes;
    public void testMemlloc(View view) {
        long l = Runtime.getRuntime().maxMemory(); // 512M 536870912

        Log.d("telenewbie", "testMemlloc: "+l);
       bytes = new byte[100 * 1024 * 1024];

        Log.d("telenewbie", "testMemlloc: "+mptr);
    }

    public void freeMemlloc(View view) {
        Log.d("telenewbie", "freeMemlloc: "+mptr);
//        testFree(mptr);
        bytes = null;
        mptr = 0;
    }

    public void docpu(View view) {
        bytes[0] = '1';
    }
}
