package com.example.myapplication

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.example.myapplication.annotation.HelloAnnotation

class AnnotationActivity : BaseActivity() {
    override fun initView() {
        addButton("测试注解的作用，以及再这里面的用法，尝试使用ButterKnief") {
            var hello = HelloAnnotation()
            println(hello.hashCode())
        }
    }

}
