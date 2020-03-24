package com.example.myapplication

import android.os.Looper
import android.os.SystemClock
import kotlinx.coroutines.*
import java.lang.Thread.sleep
import kotlin.concurrent.thread

// kotlin 的线程  thread  vs java  的线程 Thread
// 协程内部的 delay vs thread.sleep
//// 结论： delay 不会 阻塞线程，会挂起 协程
class CoroutinesActivity : BaseActivity(), CoroutineScope by MainScope() {
    private fun showSomeData() = launch {
        if (Looper.getMainLooper() != Looper.myLooper()) {
            throw RuntimeException("error not in main thread!")
        } else {
            println("被调用了：" + Thread.currentThread().id)
        }
    }

    // 创建线程
    val conroutine = CoroutineScope(Dispatchers.Main)


    override fun initView() {
        addText("这是一个协程测试的例子")


        addButton("创建协程") {
            //            CoroutineScope.
            println(Thread.currentThread().name + ":" + Thread.currentThread().id)
            val start = SystemClock.elapsedRealtimeNanos()
            println("current time:$start")
            for (i in 0..20) {
                GlobalScope.async {
                    // it doesn't block a thread, but only suspends the coroutine itself
                    delay(1000)
                    println("协程async：线程id: ${Thread.currentThread().id} current time:" + (SystemClock.elapsedRealtimeNanos() - start) / 1000_1000)
                }
                GlobalScope.launch {
                    // it doesn't block a thread, but only suspends the coroutine itself
                    delay(1000)
                    println("协程 launch：线程id: ${Thread.currentThread().id} current time:" + (SystemClock.elapsedRealtimeNanos() - start) / 1000_1000)
                }
            }

            sleep(1000)
            println("线程id: ${Thread.currentThread().id} current time:" + (SystemClock.elapsedRealtimeNanos() - start) / 1000_1000)
        }
        addButton("kotlin 创建线程 thread 而非 java 的Thread") {
            thread(start = true, name = "hello", priority = -2) {
                println("kotlin 创建的线程中：" + Thread.currentThread().id)
                showSomeData()
            }
        }

    }

}
