package com.example.myapplication.utils;

import android.os.Handler;
import android.os.HandlerThread;
import android.os.SystemClock;
import android.util.Log;

public class ThreadName {
    public final static String TAG = ThreadName.class.getSimpleName();

    public static void renameThreadAndroid() {
        HandlerThread t = new HandlerThread("newbieAndroid");
        t.start();
        // 这种写法造成内存泄漏
        Handler h = new Handler(t.getLooper(), msg -> {
            Log.d(TAG, "renameThreadJava: newbieAndroid");
            SystemClock.sleep(sleepTimeInMs()); //ms 为单位
            t.quit();
            Log.d(TAG, "renameThreadJava: newbieAndroid end ");
            return false;
        });
        h.sendEmptyMessage(1);
    }

    public static void renameThreadJava() {
        Thread t = new Thread(() -> {
            Log.d(TAG, "renameThreadJava: newbieJava1");
            SystemClock.sleep(sleepTimeInMs()); //ms 为单位
        }, "newbieJava1"); // 起名字
        t.setName("newbieJava2");
        t.start();
    }

    public static native void renameThreadCXX();

    public static native long sleepTimeInMs();
}
