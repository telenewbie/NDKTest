//
// Created by telenewbie on 2020/2/4.
//
#include <android/log.h>
#include <dlfcn.h>

#include "com_google_speech_recognizer_AbstractRecognizer.h"
#include "google_type.h"

#define TAG "intercepter_by_mine"

JNIEXPORT jint JNICALL Java_com_google_speech_recognizer_AbstractRecognizer_nativeCancel
        (JNIEnv *env, jobject obj, jlong session) {
    if (!m_cancel) {
        __android_log_print(ANDROID_LOG_ERROR, TAG,
                            "Java_com_google_speech_recognizer_AbstractRecognizer_nativeCancel error");
        return 0;
    }
    __android_log_print(ANDROID_LOG_DEBUG, TAG,
                        "Java_com_google_speech_recognizer_AbstractRecognizer_nativeCancel");
    return m_cancel(env, obj, session);
}

JNIEXPORT jlong JNICALL Java_com_google_speech_recognizer_AbstractRecognizer_nativeConstruct
        (JNIEnv *env, jobject obj) {
    if (!m_construct) {
        __android_log_print(ANDROID_LOG_ERROR, TAG,
                            "Java_com_google_speech_recognizer_AbstractRecognizer_nativeConstruct error");
        return 0;
    }
    __android_log_print(ANDROID_LOG_DEBUG, TAG,
                        "Java_com_google_speech_recognizer_AbstractRecognizer_nativeConstruct");
    return m_construct(env, obj);
}


JNIEXPORT void JNICALL Java_com_google_speech_recognizer_AbstractRecognizer_nativeDelete
        (JNIEnv *env, jobject obj, jlong ptr) {
    if (!m_delete) {
        __android_log_print(ANDROID_LOG_ERROR, TAG,
                            "Java_com_google_speech_recognizer_AbstractRecognizer_nativeDelete error");
        return;
    }
    __android_log_print(ANDROID_LOG_DEBUG, TAG,
                        "Java_com_google_speech_recognizer_AbstractRecognizer_nativeDelete");
    m_delete(env, obj, ptr);
}

JNIEXPORT void JNICALL Java_com_google_speech_recognizer_AbstractRecognizer_nativeInit
        (JNIEnv *env, jclass clazz) {
    if (!m_init) {
        __android_log_print(ANDROID_LOG_ERROR, TAG,
                            "Java_com_google_speech_recognizer_AbstractRecognizer_nativeInit error");
        return;
    }
    __android_log_print(ANDROID_LOG_DEBUG, TAG,
                        "Java_com_google_speech_recognizer_AbstractRecognizer_nativeInit");
    m_init(env, clazz);
}


JNIEXPORT jint JNICALL Java_com_google_speech_recognizer_AbstractRecognizer_nativeInitFromProto
        (JNIEnv *env, jobject obj, jlong ptr1, jlong ptr2, jbyteArray byte) {
    if (!m_initfromproto) {
        __android_log_print(ANDROID_LOG_ERROR, TAG,
                            "Java_com_google_speech_recognizer_AbstractRecognizer_nativeInitFromProto error");
        return 0;
    }
    __android_log_print(ANDROID_LOG_DEBUG, TAG,
                        "Java_com_google_speech_recognizer_AbstractRecognizer_nativeInitFromProto");
    return m_initfromproto(env, obj, ptr1, ptr2, byte);
}


JNIEXPORT jbyteArray JNICALL Java_com_google_speech_recognizer_AbstractRecognizer_nativeRun
        (JNIEnv *env, jobject obj, jlong ptr, jbyteArray array) {
    if (!m_run) {
        __android_log_print(ANDROID_LOG_ERROR, TAG,
                            "Java_com_google_speech_recognizer_AbstractRecognizer_nativeRun error");
        return nullptr;
    }
    __android_log_print(ANDROID_LOG_DEBUG, TAG,
                        "Java_com_google_speech_recognizer_AbstractRecognizer_nativeRun");
    return m_run(env, obj, ptr, array);
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
    m_cancel = (JNI_CALLED_CANCEL) dlsym(handler,
                                         "Java_com_google_speech_recognizer_AbstractRecognizer_nativeCancel");
    m_construct = (JNI_CALLED_CONSTRUCT) dlsym(handler,
                                               "Java_com_google_speech_recognizer_AbstractRecognizer_nativeConstruct");
    m_delete = (JNI_CALLED_DELETE) dlsym(handler,
                                         "Java_com_google_speech_recognizer_AbstractRecognizer_nativeDelete");
    m_init = (JNI_CALLED_INIT) dlsym(handler,
                                     "Java_com_google_speech_recognizer_AbstractRecognizer_nativeInit");
    m_initfromproto = (JNI_CALLED_INITFROMPROTO) dlsym(handler,
                                                       "Java_com_google_speech_recognizer_AbstractRecognizer_nativeInitFromProto");
    m_run = (JNI_CALLED_RUN) dlsym(handler,
                                   "Java_com_google_speech_recognizer_AbstractRecognizer_nativeRun");
}

JNIEXPORT jint JNICALL JNI_OnLoad(
        JavaVM *vm,
        void *reserved) {

    __android_log_print(ANDROID_LOG_DEBUG, "google_speech", "JNI_OnLoad");
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
