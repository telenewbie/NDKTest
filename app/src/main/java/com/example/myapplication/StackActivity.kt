package com.example.myapplication

import com.litesuits.common.assist.Toastor

/**
 *  获取堆栈，分为native 层和 java层
 */
class StackActivity : ToasterActivity() {
    override fun initView() {
        //https://time.geekbang.org/column/article/70966
        addButton("java 堆栈") {
            val allStackTraces = Thread.getAllStackTraces()
            val sb = StringBuilder()
            allStackTraces.forEach { entry ->
                sb.append("thread name :").append(entry.key.name).append("\n")
                entry.value?.forEach { sb.append(it.className).append("[").append(it.methodName).append(":").append(it.lineNumber).append("]\n") }
            }
            println(sb.toString())

        }
        addButton("native 堆栈") {
//            mToast.showSingletonToast("还未实现")
            nativeShowStack()
        }
    }

    //native
    private external fun nativeShowStack()

    companion object {
        init {
//            System.loadLibrary("native-lib")
//            System.loadLibrary("myhook")
            System.loadLibrary("mystack")

        }
    }

}
