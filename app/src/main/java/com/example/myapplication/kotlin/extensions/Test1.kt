package com.example.myapplication.kotlin.extensions

import java.io.*
import java.lang.StringBuilder
import android.icu.lang.UCharacter.GraphemeClusterBreak.T


/**
 * 测试拓展函数
 */
data class Test1(var name: String, var mine: Test1?) : Serializable {
    val list = hashSetOf<String>()
    override fun toString(): String {
        return "name=$name ,mine=[${mine?.toString()}],${list.fold(StringBuilder()) { acc, s -> acc.append(s) }}"
    }
}

fun main() {
    val test1 = Test1("我", Test1("你", null))
    val serialOut: Test1? = test1.deepcopy()
    println("$serialOut")
}

fun <T> Serializable.deepcopy(): T? {
    var obj: T? = null
    ByteArrayOutputStream().use { baos ->
        ObjectOutputStream(baos).use {
            it.writeObject(this)
        }
        ByteArrayInputStream(baos.toByteArray()).use { bais ->
            ObjectInputStream(bais).use {
                @Suppress("UNCHECKED_CAST")
                obj = it.readObject() as? T
            }
        }
    }
    return obj
}

