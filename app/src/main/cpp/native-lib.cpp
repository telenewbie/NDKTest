#include <jni.h>
#include <string>
#include <unistd.h>
#include <thread>
#include <sys/prctl.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myapplication_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
// 测试C++中申请大量内存 在 AS中的profile中的体现， 使用小米手机8 进行测试
extern "C" JNIEXPORT jlong JNICALL
Java_com_example_myapplication_MainActivity_testAlloc(JNIEnv *env,
                                                      jobject /* this */) {
    void *ptr = malloc(1024 * 1024 * 1024); // 1G
    if (!ptr) printf("telenewbie : allock error");
    return reinterpret_cast<jlong>(ptr);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_MainActivity_testFree(JNIEnv *env,
                                                     jobject /* this */, jlong ptr) {
    free(reinterpret_cast<void *>(ptr));
}
