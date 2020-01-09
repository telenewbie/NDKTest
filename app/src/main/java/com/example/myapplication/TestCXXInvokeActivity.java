package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

// 测试C++的调用顺序
public class TestCXXInvokeActivity extends BaseActivity {

    @Override
    protected void initView() {
        addButton("测试加载顺序对于相同方法的覆盖顺序", v -> invokeSameMethod());
    }

    public native static void invokeSameMethod();
}
