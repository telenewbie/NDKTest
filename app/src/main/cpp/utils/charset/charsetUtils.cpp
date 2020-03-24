//
// Created by telenewbie on 2020/2/3.
//


#include <jni.h>
#include <android/log.h>

#include "charset_util.h"

// 通过这个函数与 java 层 进行调用
#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT jstring JNICALL
Java_com_example_myapplication_utils_MyCharsetUtils_convertString(JNIEnv *env, jclass clazz,
                                                                  jstring ori) {

    // jstring -> string
    std::string str(env->GetStringUTFChars(ori, nullptr));
    __android_log_print(ANDROID_LOG_DEBUG, "Hello", "native %s", str.c_str());

    std::string result(str);
    if (CharsetUtil::isGBK(str)) {
        // 转成utf-8
        __android_log_print(ANDROID_LOG_DEBUG, "Hello", "native is gbk");
        result = CharsetUtil::GBKToUTF8(str);
    }else{
        __android_log_print(ANDROID_LOG_DEBUG, "Hello", "not gbk");
    }
    return env->NewStringUTF(result.c_str());
}

#ifdef __cplusplus
}
#endif


JNIEXPORT jint JNICALL JNI_OnLoad(
        JavaVM *vm,
        void *reserved) {

    __android_log_print(ANDROID_LOG_DEBUG, "charset", "native-lib is called");
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
