package com.example.myapplication

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.example.myapplication.utils.DnsUtils
import com.example.myapplication.utils.DnsUtils.Companion.getIps

class DnsActivity : BaseActivity() {
    override fun initView() {
        addButton("测试域名转ip") {
            val ip = getIps("www.baidu.com")
            println(String(ip))
        }
    }
}
