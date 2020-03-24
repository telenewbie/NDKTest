package com.example.myapplication.utils;

public class MyCharsetUtils {
    static {
        System.loadLibrary("charset");
    }

    private static class Instance {
        static MyCharsetUtils instance = new MyCharsetUtils();
    }

    public static MyCharsetUtils getInstance() {
        return Instance.instance;
    }

    // 将文本进行一层过滤，并返回过滤后的数据
    public native static String convertString(String ori);


}
