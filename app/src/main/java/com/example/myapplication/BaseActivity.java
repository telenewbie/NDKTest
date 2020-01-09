package com.example.myapplication;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

import com.example.myapplication.proxy.ProxyClickListener;

import java.util.Random;

public abstract class BaseActivity extends Activity {

    public LinearLayout linearLayout = null;

    private int[] colors = new int[]{
            Color.parseColor("#87CEFA"),
            Color.parseColor("#123456"),
            Color.parseColor("#FFFF00"),
            Color.parseColor("#00FFFF"),
            Color.parseColor("#654321")};

    Random random = new Random();

    public void addButton(String name, View.OnClickListener listener) {
        if (linearLayout == null) {
            linearLayout = new LinearLayout(this);
            linearLayout.setOrientation(LinearLayout.VERTICAL);
            linearLayout.setLayoutParams(new FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT, FrameLayout.LayoutParams.MATCH_PARENT));
        }
        Button button = new Button(this);
        button.setText(name);
        button.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT));
        button.setBackgroundColor(colors[random.nextInt(colors.length)]);
        button.setOnClickListener(new ProxyClickListener(listener,name));
        linearLayout.addView(button);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        initView();
        setContentView(linearLayout);
    }


    protected abstract void initView();
}
