package com.example.myapplication.proxy;

import android.util.Log;
import android.view.View;

/**
 * 点击事件代理
 * Created by J on 2019-08-02.
 */
public class ProxyClickListener implements View.OnClickListener {
    private static final String TAG = ProxyClickListener.class.getSimpleName();
    private View.OnClickListener mClickListener;
    private String mRoute;

    public ProxyClickListener(View.OnClickListener listener, String route) {
        this.mClickListener = listener;
        this.mRoute = route;
    }

    @Override
    public void onClick(final View v) {
        Log.d(TAG, "onClick: " + mRoute+" click...");
        mClickListener.onClick(v);
    }
}
