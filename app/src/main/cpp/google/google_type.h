//
// Created by telenewbie on 2020/2/5.
//

#ifndef MY_APPLICATION_GOOGLE_TYPE_H
#define MY_APPLICATION_GOOGLE_TYPE_H

#include <jni.h>

#define  LIB_NAME "libgoogle_test.so"

typedef jint(*JNI_ONLOAD_METHOD)(JavaVM *vm, void *reserved);

typedef jint(*JNI_CALLED_CANCEL)(JNIEnv *, jobject, jlong);

typedef jlong(*JNI_CALLED_CONSTRUCT)(JNIEnv *, jobject);

typedef void (*JNI_CALLED_DELETE)(JNIEnv *, jobject, jlong);

typedef void (*JNI_CALLED_INIT)(JNIEnv *, jclass);

typedef jint(*JNI_CALLED_INITFROMPROTO)(JNIEnv *, jobject, jlong, jlong, jbyteArray);

typedef jbyteArray(*JNI_CALLED_RUN)(JNIEnv *, jobject, jlong, jbyteArray);

static JNI_CALLED_CANCEL m_cancel = nullptr;
static JNI_CALLED_CONSTRUCT m_construct = nullptr;
static JNI_CALLED_DELETE m_delete = nullptr;
static JNI_CALLED_INIT m_init = nullptr;
static JNI_CALLED_INITFROMPROTO m_initfromproto = nullptr;
static JNI_CALLED_RUN m_run = nullptr;
#endif //MY_APPLICATION_GOOGLE_TYPE_H
