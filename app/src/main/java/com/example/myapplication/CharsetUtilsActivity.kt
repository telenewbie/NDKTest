package com.example.myapplication

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import com.example.myapplication.utils.MyCharsetUtils
import java.util.*

class CharsetUtilsActivity : BaseActivity() {
    override fun initView() {
        addButton("检测是否是乱码") {
            val convertString = MyCharsetUtils.convertString(String.format(Locale.CHINESE,"包含中英文newbie 数字123 标点符号，,?？的一串文本"));
            println(convertString)
        }
    }

}
