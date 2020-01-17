package com.example.myapplication;

import com.example.myapplication.utils.JNIComm;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

public class ProxyActivity extends BaseActivity {


    @Override
    protected void initView() {
        addButton("Java Proxy",v->{
            Proxy.newProxyInstance(JNIComm.class.getClassLoader(), JNIComm.class.getInterfaces(), new InvocationHandler() {
                @Override
                public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
                    return null;
                }
            });
        });
    }
}
