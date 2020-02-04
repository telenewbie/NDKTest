package com.example.myapplication;

import android.os.Bundle;

import com.litesuits.common.assist.Toastor;

public abstract class ToasterActivity extends BaseActivity {
    protected Toastor mToast = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        mToast = new Toastor(this);
        super.onCreate(savedInstanceState);
    }
}
