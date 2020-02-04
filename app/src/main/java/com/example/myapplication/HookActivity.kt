package com.example.myapplication

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle

class HookActivity : ToasterActivity() {
    override fun initView() {
        addButton("java hook") {

        }
        addButton("native malloc 进行拦截") {
            // 优先连接内部符号
            nativeMalloc()
        }
        addButton("native hook") {

        }
    }

    private external fun nativeMalloc()

    companion object {
        init {
//            System.loadLibrary("native-lib")
            System.loadLibrary("myhook")
        }
    }
}
