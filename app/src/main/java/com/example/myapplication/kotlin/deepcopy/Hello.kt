package com.example.myapplication.kotlin.deepcopy


//@DeepCopy
data class Newbie(val name: String? = null, var age: Int)

fun main() {
    val list = arrayOf(
            1..10,
            20..30,
            50..100
    )

//    val mergeList = list.flatMap { it.map { } }
//    arrayOf(1, 2, 3).map { i: Int -> if (i == 1) "NO$i" else i * 2 }.forEach(::println)
//    arrayOf(1, 2, 3).flatMap { listOf(it) }.map { "NO.$it" }.forEach(::println)
//    val fold = (0..5).fold(StringBuilder()) { acc, i -> acc.append(i).append(",") }
//    val foldResult1 = arrayOf(1, 2, 3).fold(10, { a, b -> a + b })//计算过程为10+1+2+3,等于16
//    println(fold)
//
//    list.flatMap { it }.filterIndexed { index, it -> it in 5..54 }.map { "NO.$it" }.forEach(::println)
//    arrayOf(4, 3, 2, 5).takeWhile { i -> i > 2 }.forEach(::println)
//    mergeList.forEach(::println)
//    with(FileOutputStream("")) {
//
//    }
//    list?.run {  }

//    var a = null
//    (a as? String)?.let { println(it) }
    arrayOf("first", null, 1, 2, null, "final", "100").mapNotNull { it as? Int }.forEach(::println)
}