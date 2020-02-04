//
// Created by telenewbie on 2020/1/19.
//

#include <jni.h>
#include <android/log.h>
#include <stdlib.h>

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_HookActivity_nativeMalloc(
        JNIEnv *env,
        jobject /* this */) {

    void *ptr = malloc(sizeof(int *));
    __android_log_print(ANDROID_LOG_DEBUG, "telenewbie", "hello%p", ptr);
}