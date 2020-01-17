package com.example.myapplication

import android.annotation.SuppressLint
import android.os.SystemClock
import android.widget.EditText
import com.litesuits.common.assist.Toastor
import com.litesuits.common.utils.ShellUtil


class ThreadActivity : BaseActivity() {
    private var mToast: Toastor? = null

    // 内部类
    object MyThread : Thread() {
        override fun run() {
            name = "ActThread"
            println("线程开始")
            while (!isInterrupted) {
                SystemClock.sleep(1000)
            }
            println("线程结束")
        }
    }

    private var tr = MyThread
    @SuppressLint("ShowToast")
    override fun initView() {
        mToast = Toastor(this)
        addButton("点击原来是闭包") { println("我是闭包") }
        addButton("创建java层 线程") {
            // 创建线程
            // A thread is alive if it has been started and has not yet died.
            if (!tr.isAlive)
                tr.start()
        }

        val myPid = android.os.Process.myPid()
        val commandView: EditText = addInputLineText("输入命令：", "cat /proc/" + myPid + "/task/*/comm|busybox wc -l")
        addButton("执行命令,线程个数") {
            val execCommand = ShellUtil.execCommand(commandView.text?.toString(), false)

            if (execCommand.result == 0) {
                mToast?.showSingleLongToast(execCommand.responseMsg)
            } else {
                mToast?.showSingleLongToast(execCommand.errorMsg + ",error:" + execCommand.result)
            }
        }
        addButton("线程停止") {
            if (tr.isAlive)
                tr.interrupt()
        }

    }

}
