//
// Created by telenewbie on 2020/1/9.
//

// 测试C++ 的方法

#include "link-multi.h"
#include <jni.h>
#include "logutil.h"

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_TestCXXInvokeActivity_invokeSameMethod(JNIEnv *env,
                                                                      jclass /* this */
) {
    invoke_mine();
}