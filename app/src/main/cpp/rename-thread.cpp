//
// Created by telenewbie on 2020/1/8.
//
#include <jni.h>
#include <string>
#include <unistd.h>
#include <thread>
#include <sys/prctl.h>
#include <string>

const  long  sleepTime = 1000*1000;
extern "C" JNIEXPORT jlong JNICALL
Java_com_example_myapplication_ThreadName_sleepTimeInMs(
        JNIEnv *env,
        jclass /* this */) {
    return sleepTime;
}


extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_ThreadName_renameThreadCXX(
        JNIEnv *env,
        jclass /* this */) {

    std::thread t([]{
        // 为线程命名
        prctl(PR_SET_NAME, ("newbie" + std::to_string(1)).c_str());
        printf("hello telenewbie");
        usleep(sleepTime*1000); //us
    });
    t.detach();
}
