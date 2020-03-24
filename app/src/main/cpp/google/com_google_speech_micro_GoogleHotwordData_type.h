//
// Created by telenewbie on 2020/2/7.
//

#ifndef MY_APPLICATION_COM_GOOGLE_SPEECH_MICRO_GOOGLEHOTWORDDATA_TYPE_H
#define MY_APPLICATION_COM_GOOGLE_SPEECH_MICRO_GOOGLEHOTWORDDATA_TYPE_H


#define  LIB_NAME "libtemp.so"
#define  TAG "GOOGLEHOTWORDDATA"


typedef jint(*JNI_ONLOAD_METHOD)(JavaVM *vm, void *reserved);

typedef void(*JNI_NATIVECLOSE)(JNIEnv *, jclass, jlong);

typedef jstring(*JNI_NATIVEGETHOTWORDMODELID)(JNIEnv *, jclass, jlong);

typedef jint(*JNI_NATIVEIDEALBUFFERBYTES)(JNIEnv *, jclass, jlong);

typedef jboolean(*JNI_NATIVEISSPEAKERMODELCOMPATIBLE)(JNIEnv *, jclass, jlong, jbyteArray);

typedef jboolean(*JNI_NATIVEISSPEAKERVERIFICATIONENABLED)(JNIEnv *, jclass, jlong);

typedef jint(*JNI_NATIVEMAXNUMIMPOSTORATTEMPTSFORVOICEUNLOCK)(JNIEnv *, jclass, jlong);

typedef jlong(*JNI_NATIVENEW)(JNIEnv *, jclass, jbyteArray, jint, jint, jint, jboolean);

JNI_ONLOAD_METHOD nativeClose = nullptr;
JNI_NATIVECLOSE nativeGetHotwordModelId = nullptr;
JNI_NATIVEGETHOTWORDMODELID nativeIdealBufferBytes = nullptr;
JNI_NATIVEIDEALBUFFERBYTES nativeIsSpeakerModelCompatible = nullptr;
JNI_NATIVEISSPEAKERMODELCOMPATIBLE nativeIsSpeakerVerificationEnabled = nullptr;
JNI_NATIVEISSPEAKERVERIFICATIONENABLED nativeMaxNumImpostorAttemptsForVoiceUnlock = nullptr;
JNI_NATIVEMAXNUMIMPOSTORATTEMPTSFORVOICEUNLOCK nativeNew = nullptr;


#endif //MY_APPLICATION_COM_GOOGLE_SPEECH_MICRO_GOOGLEHOTWORDDATA_TYPE_H
