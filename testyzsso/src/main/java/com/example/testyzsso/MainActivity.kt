package com.example.testyzsso

import android.content.Intent
import android.os.Bundle
import android.os.Environment
import android.os.Looper
import android.util.Log
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import butterknife.ButterKnife
import butterknife.OnClick
import com.example.utils.PermissionUtils
import com.example.utils.RecorderUtils
import com.example.utils.Utils
import com.litesuits.common.io.FileUtils
import com.litesuits.common.utils.SdCardUtil
import com.txz.ecnr.AECHelper
import com.txz.ecnr.Speex
import com.txz.engine.Unisound
import java.io.File

class MainActivity : AppCompatActivity() {
    private val TAG = "newbie"


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        // 申请权限
        ButterKnife.bind(this)
        PermissionUtils.checkPermission(this, "android.permission.RECORD_AUDIO", 100)
        PermissionUtils.checkPermission(this, "android.permission.WRITE_EXTERNAL_STORAGE", 101)
        PermissionUtils.checkPermission(this, "android.permission.READ_EXTERNAL_STORAGE", 102)

        Unisound.setOnAsrEventListener { eventId: Int, time: Int, eventContent: ByteArray, eventErrCode: Int, eventExtendInfo: Long ->
            val string = String(eventContent)
            Log.d(TAG, "AsrEventHandle: eventId=$eventId,time=$time,eventContent=$string,eventErrorcode=$eventErrCode,evneyErrInfo=$eventExtendInfo")
            if (eventId == Unisound.UNI_EVENT_ENUM.UNI_LOCAL_ASR_SELF_RESULT) {
                val wakeupTag = "<_wakeupKws>"
                val substring = string.substring(string.indexOf(wakeupTag) + wakeupTag.length, string.indexOf("</_wakeupKws>"))
                showTip(substring.trim())
            }
        }
    }

    private fun showTip(msg: String) {
        Log.d("newbie", msg)
        if (Looper.getMainLooper() == Looper.myLooper())
            Toast.makeText(this, msg, Toast.LENGTH_SHORT).show()
        else {
            runOnUiThread {
                Toast.makeText(this, msg, Toast.LENGTH_SHORT).show()
            }
        }

    }

    @OnClick(R.id.btn_ar_create, R.id.btn_ar_start, R.id.btn_ar_release, R.id.btn_ar_stop, R.id.btn_ar_read,
            R.id.btn_at_stop, R.id.btn_at_open, R.id.btn_at_play, R.id.btn_at_release, R.id.btn_at_write,
            R.id.btn_reg_create, R.id.btn_reg_load, R.id.btn_reg_start, R.id.btn_reg_init,
            R.id.btn_reg_, R.id.btn_reg_file_unzip,
            R.id.btn_aec_cancellation, R.id.btn_aec_create, R.id.btn_aec_destory, R.id.btn_aec_proc_mic, R.id.btn_aec_proc_ref,
            R.id.btn_open_preprocess_nr, R.id.btn_nr_create, R.id.btn_nr_destory, R.id.btn_open_preprocess_aec, R.id.btn_preprocess, R.id.btn_open_preprocess_vad,
            R.id.btn_test)
    fun clickView(view: View) {
        var result: Int = -1
        when (view.id) {
            R.id.btn_ar_create -> result = Unisound.CaptureOpen(0, 1, 16, 16000)
            R.id.btn_ar_start -> result = Unisound.CaptureStart(0)
            R.id.btn_ar_release -> result = Unisound.CaptureClose(0)
            R.id.btn_ar_stop -> result = Unisound.CaptureStop(0)
            R.id.btn_ar_read -> write2File()
            R.id.btn_at_stop -> result = Unisound.AudioStop(0)
            R.id.btn_at_open -> result = Unisound.AudioOpen(0, 1, 16, 16000)
            R.id.btn_at_play -> result = Unisound.AudioStart(0)
            R.id.btn_at_release -> result = Unisound.AudioClose(0)
            R.id.btn_at_write -> readFromFile()
            R.id.btn_reg_init -> {
//                // step 1.
//                val initialize = Unisound.Initialize()
//                println("initialize:$initialize")

                val intent = Intent()
                intent.setClass(this, UnisoundAsrActivity::class.java)
                intent.setPackage(packageName)
                this@MainActivity.startActivity(intent)
            }
            R.id.btn_reg_create -> {
                // step 2.
                val recognCreate = Unisound.RecognCreate("$filesDir/config/config_file".toByteArray())
                println("recognCreate:$recognCreate")
            }
            R.id.btn_reg_start -> {
                // step 5. 启动识别引擎
                val recognStart = Unisound.RecognStart(Unisound.UNI_RECOGN_MODE.UNI_RASR_MODE)
//                val recognStart = Unisound.RecognStart(Unisound.UNI_RECOGN_MODE.UNI_STD_LASR)
                println("recognStart:$recognStart")
            }
            R.id.btn_test -> {
                Unisound.RecognStop()
                println("RecognStop:")
            }
            R.id.btn_reg_load -> {
                // step 4.加载
                val reLoadMode = Unisound.ReLoadMode("/sdcard/newbie/hello.dat".toByteArray())
                println("reLoadMode $reLoadMode ")
            }
            R.id.btn_reg_ -> {
            }
            R.id.btn_reg_file_unzip -> {
                // step 0.
                println("getFilesDir().toString():$filesDir")
                println("getFilesDir().toString():${SdCardUtil.getSDCardPath()}")
                Utils.copyFilesFassets(this, "config", "$filesDir/config")
                Utils.copyFilesFassets(this, "models", "/sdcard/models")
            }
            R.id.btn_aec_cancellation -> {
                // 4. 回音消除
                AECHelper.cancellation(this.application)
            }
            R.id.btn_aec_create -> {
                // 1. 创建
                AECHelper.create()
            }
            R.id.btn_aec_destory -> {
                // 6. 释放 aec
                AECHelper.destory()
            }
            R.id.btn_aec_proc_mic -> {
                AECHelper.procMic()
            }
            R.id.btn_aec_proc_ref -> {
                AECHelper.procRef()
            }
            R.id.btn_open_preprocess_nr -> {
                AECHelper.openNR(-10000)
            }
            R.id.btn_nr_create -> {
                // 2. 创建nr
                AECHelper.nrCreate()
            }
            R.id.btn_nr_destory -> {
                // 6. 释放 nr
                AECHelper.nrDestory()
            }
            R.id.btn_open_preprocess_aec -> {
                AECHelper.openAEC(-1000000)
            }
            R.id.btn_preprocess -> {
                Thread {
                    kotlin.run {

                        Unisound.CaptureOpen(0, 1, 16, 16000)
                        Unisound.CaptureStart(0)

                        val size = 320
                        val bytes = ByteArray(size) { 0 }
                        val out = ByteArray(size) { 0 }

                        while (Unisound.CaptureRead(0, bytes, size) >= 0) {
                            // 测试
                            //
                            val preProc = AECHelper.preProc(bytes, out)
                            println("current state：$preProc")
                        }
                    }
                }.start()

                AECHelper.nrProc()
            }
            R.id.btn_open_preprocess_vad -> {
                // 3. 设置aec和nr 关联
                AECHelper.openVAD(99, 99)
            }

        }
        showTip("结果：$result")
    }

    val file = File(Environment.getExternalStorageDirectory(), "hello.pcm")
    fun write2File() {
        Thread {
            kotlin.run {
                if (file.exists())
                    file.delete()
                val size = 1024
                val bytes = ByteArray(size) { 0 }
                while (Unisound.CaptureRead(0, bytes, size) >= 0) {
                    FileUtils.writeByteArrayToFile(file, bytes, true)
                }
            }
        }.start()
    }

    fun readFromFile() {
        Thread {
            kotlin.run {
                val byte = FileUtils.readFileToByteArray(file)
                showTip("读取到数据量：" + byte.size)
                Unisound.AudioWrite(0, byte, byte.size)
            }
        }.start()
    }


}
