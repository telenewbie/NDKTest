#include <jni.h>
#include <string>
#include <unistd.h>
#include <thread>
#include <sys/prctl.h>
#include <string>
#include <android/log.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myapplication_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
//extern "C" JNIEXPORT void JNICALL
//Java_com_example_myapplication_HookActivity_nativeMalloc(
//        JNIEnv *env,
//        jobject /* this */) {
//    __android_log_print(ANDROID_LOG_DEBUG, "hello", "hello");
//}

