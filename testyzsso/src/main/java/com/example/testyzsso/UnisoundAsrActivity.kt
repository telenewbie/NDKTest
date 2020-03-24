package com.example.testyzsso

import android.content.Context
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.JsonReader
import android.util.Log
import android.view.ContextMenu
import android.view.View
import android.widget.TextView
import butterknife.BindView
import butterknife.ButterKnife
import butterknife.OnClick
import com.example.data.ASR_ONLINE_RESULT
import com.example.utils.Utils
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import com.txz.engine.Unisound
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class UnisoundAsrActivity : AppCompatActivity() {

    val gson = Gson()


    @BindView(R.id.tv_unisound_result)
    lateinit var tvResult: TextView


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_unisound_asr)
        ButterKnife.bind(this)

//        Utils.copyFilesFassets(this, "config", "$filesDir/config")
        GlobalScope.launch {
            copy2Sdcard(this@UnisoundAsrActivity)
        }

        tvResult.text = ""
        Unisound.setOnAsrEventListener { eventId: Int, time: Int, eventContent: ByteArray, eventErrCode: Int, eventExtendInfo: Long ->
            val string = String(eventContent)
            Log.d("newbie", "AsrEventHandle: eventId=$eventId,time=$time,eventContent=$string,eventErrorcode=$eventErrCode,evneyErrInfo=$eventExtendInfo")
            if (eventId == Unisound.UNI_EVENT_ENUM.UNI_REMOTE_ASR_SELF_RESULT) {

                val resultType = object : TypeToken<ASR_ONLINE_RESULT>() {}.type
                val fromJson = gson.fromJson<ASR_ONLINE_RESULT>(string, resultType)
                println("hello::::::${fromJson.text}")

                tvResult.text = fromJson.text
                //{"asr_recongize":"哎super tuesday which says fool to date and two other什么乱七八糟的看下，","rc":5,"general":{"text":"哭唧唧，这个问题我不会，又被你的智慧降服了","type":"T"},"returnCode":0,"code":"ANSWER","retTag":"nlu","service":"cn.yunzhisheng.error","nluProcessTime":"1187","text":"哎super tuesday which says fool to date and two other什么乱七八糟的看下","history":"cn.yunzhisheng.error","responseId":"fa4c638856da4e2aa0adf3bba5c66e1a"}
            }
        }
    }

    @OnClick(R.id.btn_unisound_init_engine,
            R.id.btn_unisound_create_asr,
            R.id.btn_unisound_asr_start,
            R.id.btn_unisound_asr_stop)
    fun clickView(view: View) {
        when (view.id) {
            R.id.btn_unisound_init_engine -> {
                Unisound.Initialize()
                val recognCreate = Unisound.RecognCreate("$filesDir/config/config_file".toByteArray())

            }
            R.id.btn_unisound_create_asr -> {
                val recognCreate = Unisound.RecognCreate("$filesDir/config/config_file".toByteArray())
            }
            R.id.btn_unisound_asr_start -> {
                val recognStart = Unisound.RecognStart(Unisound.UNI_RECOGN_MODE.UNI_RASR_MODE)
            }
            R.id.btn_unisound_asr_stop -> {
                Unisound.RecognStop()
            }
        }
    }

    suspend fun copy2Sdcard(ctx: Context) {
        withContext(Dispatchers.IO)
        {
            Utils.copyFilesFassets(ctx, "config", "$filesDir/config")
            Utils.copyFilesFassets(ctx, "models", "/sdcard/models")
        }
    }
}
