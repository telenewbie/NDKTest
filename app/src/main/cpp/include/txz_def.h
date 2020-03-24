//
// Created by telenewbie on 2019/9/26.
//

#ifndef TENCENT_TXZ_DEF_H
#define TENCENT_TXZ_DEF_H

#include <string>
#include <jni.h>

inline std::string t_jstring2string(JNIEnv *env, jstring jStr) {
    if (!jStr)
        return "";
    const char *buf = env->GetStringUTFChars(jStr, nullptr);
    std::string ret = std::string(buf, static_cast<unsigned int>(env->GetStringUTFLength(jStr)));
    env->ReleaseStringUTFChars(jStr, buf);
    return ret;
}

//转换成字符串
#define STRING_TO_JNI_BYTE_ARRAY(_env,_str,_jb) \
do \
{ \
_jb = (_env)->NewByteArray((_str).size()); \
if (NULL == _jb) break; \
(_env)->SetByteArrayRegion(_jb, 0, (jsize)(_str).size(), (jbyte*) (_str).data()); \
}while(0)

#define JNI_BYTE_ARRAY_TO_STRING(_env, _arr, _str) \
do \
{ \
    if (NULL == (_arr))break; \
    jbyte* ___byte_array_data = (_env)->GetByteArrayElements((_arr), NULL); \
    (_str).assign((const char*)___byte_array_data, (_env)->GetArrayLength((_arr))); \
    (_env)->ReleaseByteArrayElements((_arr), ___byte_array_data, 0); \
} while(0)

#define GET_METHOD(methodVar, env, clazz, method, methodSig) \
do{ \
methodVar = env->GetStaticMethodID(clazz, method,methodSig); \
if (methodVar == nullptr) { \
__android_log_print(ANDROID_LOG_ERROR,"telenewbie", "[%d]can't find method %s",__LINE__,method); \
break; \
} \
}while(0)

#define GET_CLASS(classPtr,env,className) \
do{ \
jclass tmp = env->FindClass(className);\
if (tmp == nullptr) {\
__android_log_print(ANDROID_LOG_ERROR, "telenewbie", "can't find class %s",\
c_class_TsrVadDetector);\
}\
classPtr = (jclass)(env->NewGlobalRef(tmp));\
env->DeleteLocalRef(tmp); \
}while(0)

#endif //TENCENT_TXZ_DEF_H