//
// Created by telenewbie on 2020/2/3.
//


#include <jni.h>
#include <android/log.h>

JNIEXPORT jint JNICALL JNI_OnLoad(
        JavaVM *vm,
        void *reserved) {

    __android_log_print(ANDROID_LOG_DEBUG, "STACK", "native-lib is called");
    JNIEnv *env = NULL;
    jint result = -1;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }

    if (NULL == env) {
        return -1;
    }
//    init();
//    if (!registerNatives(env))
//    {//注册
//        return -1;
//    }
//
//    TXZ_Log::init(vm);

    //成功
    result = JNI_VERSION_1_4;

    return result;
}