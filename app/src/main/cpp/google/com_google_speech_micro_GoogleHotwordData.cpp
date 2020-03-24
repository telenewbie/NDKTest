//
// Created by telenewbie on 2020/2/7.
//

#include <android/log.h>
#include <dlfcn.h>

#include "com_google_speech_micro_GoogleHotwordData.h"
#include "com_google_speech_micro_GoogleHotwordData_type.h"

JNIEXPORT void JNICALL Java_com_google_speech_micro_GoogleHotwordData_nativeClose
        (JNIEnv *, jclass, jlong) {

}

JNIEXPORT jstring JNICALL Java_com_google_speech_micro_GoogleHotwordData_nativeGetHotwordModelId
        (JNIEnv *, jclass, jlong) {
    return 0;
}

JNIEXPORT jint JNICALL Java_com_google_speech_micro_GoogleHotwordData_nativeIdealBufferBytes
        (JNIEnv *, jclass, jlong) {
    return 0;
}

JNIEXPORT jboolean JNICALL
Java_com_google_speech_micro_GoogleHotwordData_nativeIsSpeakerModelCompatible
        (JNIEnv *, jclass, jlong, jbyteArray) {
    return 0;
}

JNIEXPORT jboolean JNICALL
Java_com_google_speech_micro_GoogleHotwordData_nativeIsSpeakerVerificationEnabled
        (JNIEnv *, jclass, jlong) {
    return 0;
}

JNIEXPORT jint JNICALL
Java_com_google_speech_micro_GoogleHotwordData_nativeMaxNumImpostorAttemptsForVoiceUnlock
        (JNIEnv *, jclass, jlong) {
    return 0;
}

JNIEXPORT jlong JNICALL Java_com_google_speech_micro_GoogleHotwordData_nativeNew
        (JNIEnv *, jclass, jbyteArray, jint, jint, jint, jboolean) {
    return 0;
}

void init(JavaVM *vm,
          void *reserved) {
// 动态打开so
// 替换成 需要的so
    void *handler = dlopen(LIB_NAME, RTLD_NOW);
    if (!handler) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "dlopen  error " LIB_NAME);
        return;
    }
    auto ptr = (JNI_ONLOAD_METHOD) dlsym(handler, "JNI_OnLoad");
    if (!ptr) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "JNI_OnLoad cant find");
        return;
    }
    jint ret = ptr(vm, reserved);
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "JNI_OnLoad return %d", ret);
    //获得各个函数


//
//    nativeClose=dlsym(handler, "Java_com_google_speech_micro_GoogleHotwordData_nativeClose");
//    nativeGetHotwordModelId=dlsym(handler, "Java_com_google_speech_micro_GoogleHotwordData_nativeGetHotwordModelId");
//    nativeIdealBufferBytes=dlsym(handler, "Java_com_google_speech_micro_GoogleHotwordData_nativeIdealBufferBytes");
//    nativeIsSpeakerModelCompatible=dlsym(handler, "Java_com_google_speech_micro_GoogleHotwordData_nativeIsSpeakerModelCompatible");
//    nativeIsSpeakerVerificationEnabled=dlsym(handler,"Java_com_google_speech_micro_GoogleHotwordData_nativeIsSpeakerVerificationEnabled");
//    nativeMaxNumImpostorAttemptsForVoiceUnlock=dlsym(handler,"Java_com_google_speech_micro_GoogleHotwordData_nativeMaxNumImpostorAttemptsForVoiceUnlock");
//    nativeNew=dlsym(handler, "Java_com_google_speech_micro_GoogleHotwordData_nativeNew");
}

JNIEXPORT jint JNICALL JNI_OnLoad(
        JavaVM *vm,
        void *reserved) {

    __android_log_print(ANDROID_LOG_DEBUG, TAG,
                        "com_google_speech_micro_GoogleHotwordData:JNI_OnLoad");
    JNIEnv *env = nullptr;
    jint result;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }

    if (nullptr == env) {
        return -1;
    }
    init(vm, reserved);
    result = JNI_VERSION_1_4;

    return result;
}