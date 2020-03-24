#ifdef ANDROID

#include "jni.h"
#include <iostream>
#include <unistd.h>
#include "JNILogManager.h"

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_txznet_log_LogNative_setLogLevel(JNIEnv *env, jclass jnicom, jint level) {
    JNILogManager *ptr = new JNILogManager((int) level);
    return (jlong) ptr;
}

JNIEXPORT jboolean JNICALL
Java_com_txznet_log_LogNative_isEnableLevel(JNIEnv *env, jclass jnicom, jlong ptradapter,
                                            jint level) {
    JNILogManager *ptr = (JNILogManager *) ptradapter;
    return (jboolean) ptr->isEnableLevel((int) level);
}

JNIEXPORT void JNICALL Java_com_txznet_log_LogNative_write(JNIEnv *env,
                                                           jclass jnicom,
                                                           jlong ptradapter,
                                                           jlong pid,
                                                           jlong tid,
                                                           jint logid,
                                                           jbyteArray function,
                                                           jbyteArray file,
                                                           jint line,
                                                           jint level,
                                                           jbyteArray tag,
                                                           jbyteArray msg) {
    JNILogManager *ptr = (JNILogManager *) ptradapter;

    if (!ptr->isEnableLevel((int) level)) {
        return;
    }

    int8_t *byte_buf = env->GetByteArrayElements(function, nullptr);
    size_t byte_size = env->GetArrayLength((jarray) function);
    std::string strFunction((char *) byte_buf, byte_size);
    env->ReleaseByteArrayElements(function, byte_buf, 0);
    env->DeleteLocalRef((jobject) function);

    byte_buf = env->GetByteArrayElements(file, nullptr);
    byte_size = env->GetArrayLength((jarray) file);
    std::string strFile((char *) byte_buf, byte_size);
    env->ReleaseByteArrayElements(file, byte_buf, 0);
    env->DeleteLocalRef((jobject) file);

    byte_buf = env->GetByteArrayElements(tag, nullptr);
    byte_size = env->GetArrayLength((jarray) tag);
    std::string strTag((char *) byte_buf, byte_size);
    env->ReleaseByteArrayElements(tag, byte_buf, 0);
    env->DeleteLocalRef((jobject) tag);

    byte_buf = env->GetByteArrayElements(msg, nullptr);
    byte_size = env->GetArrayLength((jarray) msg);
    std::string strMsg((char *) byte_buf, byte_size);
    env->ReleaseByteArrayElements(msg, byte_buf, 0);
    env->DeleteLocalRef((jobject) msg);

    ptr->writeLogMessage((long) pid,
                         (long) tid,
                         (int) logid,
                         strFunction,
                         strFile,
                         (int) line,
                         (int) level,
                         strTag,
                         strMsg);
    return;

}

JNIEXPORT void JNICALL Java_com_txznet_log_LogNative_writeNoCheckLevel(JNIEnv *env,
                                                                       jclass jnicom,
                                                                       jlong ptradapter,
                                                                       jlong pid,
                                                                       jlong tid,
                                                                       jint logid,
                                                                       jbyteArray function,
                                                                       jbyteArray file,
                                                                       jint line,
                                                                       jint level,
                                                                       jbyteArray tag,
                                                                       jbyteArray msg) {
    JNILogManager *ptr = (JNILogManager *) ptradapter;

    int8_t *byte_buf = env->GetByteArrayElements(function, nullptr);
    size_t byte_size = env->GetArrayLength((jarray) function);
    std::string strFunction((char *) byte_buf, byte_size);
    env->ReleaseByteArrayElements(function, byte_buf, 0);
    env->DeleteLocalRef((jobject) function);

    byte_buf = env->GetByteArrayElements(file, nullptr);
    byte_size = env->GetArrayLength((jarray) file);
    std::string strFile((char *) byte_buf, byte_size);
    env->ReleaseByteArrayElements(file, byte_buf, 0);
    env->DeleteLocalRef((jobject) file);

    byte_buf = env->GetByteArrayElements(tag, nullptr);
    byte_size = env->GetArrayLength((jarray) tag);
    std::string strTag((char *) byte_buf, byte_size);
    env->ReleaseByteArrayElements(tag, byte_buf, 0);
    env->DeleteLocalRef((jobject) tag);

    byte_buf = env->GetByteArrayElements(msg, nullptr);
    byte_size = env->GetArrayLength((jarray) msg);
    std::string strMsg((char *) byte_buf, byte_size);
    env->ReleaseByteArrayElements(msg, byte_buf, 0);
    env->DeleteLocalRef((jobject) msg);

    ptr->writeLogMessage((long) pid,
                         (long) tid,
                         (int) logid,
                         strFunction,
                         strFile,
                         (int) line,
                         (int) level,
                         strTag,
                         strMsg);
    return;
}

}
#endif


