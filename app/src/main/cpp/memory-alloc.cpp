//
// Created by telenewbie on 2020/1/9.
//

// 测试C++中申请大量内存 在 AS中的profile中的体现， 使用小米手机8 进行测试
#include <jni.h>
#include <malloc.h>
#include <android/log.h>

extern "C" JNIEXPORT jlong JNICALL
Java_com_example_myapplication_utils_MemoryAlloc_allocByCXX(JNIEnv *env,
                                                            jobject /* this */,
                                                            jlong size) {
    void *ptr = malloc(static_cast<size_t>(size)); // 1G
    if (!ptr) __android_log_print(ANDROID_LOG_DEBUG, "telenewbieJNI", "message:[%s:malloc failed]", __FUNCTION__);

    return reinterpret_cast<jlong>(ptr);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_utils_MemoryAlloc_freeCXX(JNIEnv *env,
                                                         jobject /* this */, jlong ptr) {
    if (!ptr) {
        __android_log_print(ANDROID_LOG_DEBUG, "telenewbieJNI", "message:[%s:free nullptr?]",
                            __FUNCTION__);
        return;
    }
    free(reinterpret_cast<void *>(ptr));
}