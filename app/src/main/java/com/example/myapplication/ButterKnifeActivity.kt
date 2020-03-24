package com.example.myapplication

import android.annotation.SuppressLint
import android.os.Bundle
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import butterknife.BindView
import butterknife.ButterKnife
import butterknife.OnClick

class ButterKnifeActivity : AppCompatActivity() {

    @BindView(R.id.button)
    lateinit var mBtn_test: Button

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_butter_knief)
        //注解进行标注，这句就是将标注的进行 赋值
        ButterKnife.bind(this)
    }

    @SuppressLint("SetTextI18n")
    @OnClick(R.id.button)
    fun click() {
        mBtn_test.text = "通过butterknief 进行bindview后设值"

    }
}
