package com.example.myapplication

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Process
import android.util.Log
import android.util.Log.DEBUG
import android.util.Log.INFO
import com.txznet.log.LogNative

class LogActivity : BaseActivity() {
    override fun initView() {
        addButton("打印") {
            val id = LogNative.setLogLevel(DEBUG)
            if (LogNative.isEnableLevel(id, INFO)) {
                Thread(Runnable {
                    for (i in 1..400000) {
                        LogNative.write(id, Process.myPid().toLong(), Process.myTid().toLong(), i, "我是方法名".toByteArray(), "我是文件名".toByteArray(), 2, DEBUG, "newbie".toByteArray(), "你好".toByteArray())
                    }
                    Log.d("newbie", "日志写完")
                }).start()

            }
        }
        addButton("强制打印") {
            val id = LogNative.setLogLevel(INFO)
            LogNative.writeNoCheckLevel(id, Process.myPid().toLong(), Process.myTid().toLong(), 1, "initView".toByteArray(), "".toByteArray(), 2, DEBUG, "newbie".toByteArray(), "强制插入".toByteArray())
        }
        addButton("测试外部导入框架"){

        }
    }

    // 加载so

    companion object {
        init {
            System.loadLibrary("txzlog")
        }
    }

}
