package com.example.myapplication

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Process
import android.widget.Toast
import com.litesuits.common.assist.Toastor

class PIDActivity : BaseActivity() {

    override fun initView() {
//        val toast = Toastor(this)
        addText("结论 Process.myTid() == Process.myPid()， 表明是主线程")

        addButton("获取当前线程的tid") {
            Toast.makeText(applicationContext, "当前线程的tid:${Thread.currentThread().id}", Toast.LENGTH_SHORT).show()
        }
        addButton("Process.myTid()") {
            Toast.makeText(applicationContext, "当前线程的tid:${Process.myTid()}", Toast.LENGTH_SHORT).show()
        }
        addButton("Process.myUid()") {
            Toast.makeText(applicationContext, "当前线程的pid:${Process.myUid()}", Toast.LENGTH_SHORT).show()
        }
        addButton("获取当前进程的pid") {
            Toast.makeText(applicationContext, "当前进程的tid:${Process.myPid()}", Toast.LENGTH_SHORT).show()
        }
    }

}
