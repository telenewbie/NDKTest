package com.example.myapplication;

import com.example.myapplication.utils.ThreadName;

public class RenameThreadActivity extends BaseActivity {

    // 拦截方法

    @Override
    protected void initView() {
        addButton("Java 线程", v -> ThreadName.renameThreadJava());
        addButton("C++ 线程", v -> ThreadName.renameThreadCXX());
        addButton("Android 线程", v -> ThreadName.renameThreadAndroid());
    }
}
