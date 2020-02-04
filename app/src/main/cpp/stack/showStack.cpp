//
// Created by telenewbie on 2020/1/21.
//
#include <jni.h>
#include <android/log.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <android/log.h>
#include <dlfcn.h>
#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <atomic>
#include <sys/prctl.h>
#include <string>
#include <unistd.h>
#include "backtrace/Backtrace.h"

//ANDROID 5.0以下
/*
 * Describes a single frame of a backtrace.
 */
typedef struct {
    uintptr_t absolute_pc; /* absolute PC offset */
    uintptr_t stack_top; /* top of stack for this frame */
    size_t stack_size; /* size of this stack frame */
} backtrace_frame_t;

/*
 * Describes the symbols associated with a backtrace frame.
 */
typedef struct {
    uintptr_t relative_pc; /* relative frame PC offset from the start of the library,
     or the absolute PC if the library is unknown */
    uintptr_t relative_symbol_addr; /* relative offset of the symbol from the start of the
     library or 0 if the library is unknown */
    char *map_name; /* executable or library name, or NULL if unknown */
    char *symbol_name; /* symbol name, or NULL if unknown */
    char *demangled_name; /* demangled symbol name, or NULL if unknown */
} backtrace_symbol_t;

typedef ssize_t (*Tfunc_unwind_backtrace)(
        backtrace_frame_t *backtrace,
        size_t ignore_depth,
        size_t max_depth);

typedef void (*Tfunc_get_backtrace_symbols)(
        const backtrace_frame_t *backtrace,
        size_t frames,
        backtrace_symbol_t *backtrace_symbols);

typedef void (*Tfunc_free_backtrace_symbols)(
        backtrace_symbol_t *backtrace_symbols,
        size_t frames);

static void *gHandle = NULL;

Tfunc_unwind_backtrace func_unwind_backtrace = NULL;
Tfunc_get_backtrace_symbols func_get_backtrace_symbols = NULL;
Tfunc_free_backtrace_symbols func_free_backtrace_symbols = NULL;

#define LOG log_print
#define TAG "CBackTrace"
#define LOGE(fmt, ...) LOG(ANDROID_LOG_ERROR, TAG, fmt, ##__VA_ARGS__)


int log_print(int prio, const char *tag, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int iret = __android_log_vprint(prio, tag, fmt, ap);
    va_end(ap);
    return iret;
}
#define MAX_DEPTH 16
#define MAX_BACKTRACE_LINE_LENGTH   10240
#define MAX_BACKTRACE_NAME_LEN 64

int captureNativeCrashForAndroid5(const void* ctx, const char* reason){
    LOGE("libbacktrace.so start ");
    char lines[MAX_BACKTRACE_LINE_LENGTH] = { 0, };
    void* libbacktrace = dlopen( "/system/lib/libbacktrace.so", RTLD_LAZY |RTLD_LOCAL );
    if ( !libbacktrace ){
        LOGE("libbacktrace.so error ");
        return -1;
    }
    Backtrace* (*create)(int,int,void*);
    *(void**)&create = dlsym( libbacktrace, "_ZN9Backtrace6CreateEiiP12BacktraceMap" );
    if ( !create ){
        LOGE("libbacktrace.so dlsym ");
        return -1;
    }
    Backtrace* t = create(getpid(), 0, NULL );
    if ( !t ){
        LOGE("libbacktrace.so create ");
        return -1;
    }
    int ret = t->Unwind(0);
//    int ret = t->Unwind(0, (ucontext*)ctx);
    if ( !ret ) {
        LOGE("libbacktrace.so unwind ");
        return -1;
    }
    size_t count = t->NumFrames();
    for ( size_t i=0; i < count; i++ ) {
        std::string line = t->FormatFrameData(i);
        if(line == ""){
            LOGE("libbacktrace.so break ");
            break;
        }
        snprintf(lines, sizeof(lines), "%s \n%s", lines, line.c_str());
        LOGE("back=%s", line.c_str());
    }
//    dumpFile(reason,lines);


    return 0;
}
//显示所有的堆栈信息
// libcorkscrew.so在Android4.4之后废弃了， 取而代之的是采用libbacktrace.so来获取崩溃数据

void init() {
    if (gHandle == NULL) {
        gHandle = dlopen("/system/lib/libcorkscrew.so", RTLD_NOW);
        if (gHandle != NULL) {
            func_unwind_backtrace = (Tfunc_unwind_backtrace) dlsym(gHandle, "unwind_backtrace");
            func_get_backtrace_symbols = (Tfunc_get_backtrace_symbols) dlsym(gHandle,
                                                                             "get_backtrace_symbols");
            func_free_backtrace_symbols = (Tfunc_free_backtrace_symbols) dlsym(gHandle,
                                                                               "free_backtrace_symbols");
            return;
        }
        __android_log_print(ANDROID_LOG_ERROR, "stack", "func '/system/lib/libcorkscrew.so' error");
        gHandle = dlopen("/system/lib/libbacktrace.so", RTLD_NOW);
        if (gHandle != NULL) {
            void *backmap = dlsym(gHandle, "_ZN9Backtrace6CreateEiiP12BacktraceMap");

            captureNativeCrashForAndroid5(nullptr, "hello world");
            return;
        }
        __android_log_print(ANDROID_LOG_ERROR, "stack", "func '/system/lib/libbacktrace.so' error");
    }


}

void showStack() {
    const int skip = 2 /*跳过几级栈*/;
    backtrace_frame_t mStack[MAX_DEPTH];
    if (!func_unwind_backtrace) {
        __android_log_print(ANDROID_LOG_DEBUG, "stack", "error func find empty");
        return;
    }
    int count = func_unwind_backtrace(mStack, skip + 1, MAX_DEPTH);
    __android_log_print(ANDROID_LOG_DEBUG, "stack", "func find success %d", count);

}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_StackActivity_nativeShowStack(
        JNIEnv *env,
        jobject /* this */) {
    showStack();

}


/*
 * System.loadLibrary("lib")时调用
 * 如果成功返回JNI版本, 失败返回-1
 */JNIEXPORT jint JNICALL JNI_OnLoad(
        JavaVM *vm,
        void *reserved) {

    __android_log_print(ANDROID_LOG_DEBUG, "STACK", "JNI_ONLOAD");
    JNIEnv *env = NULL;
    jint result = -1;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }

    if (NULL == env) {
        return -1;
    }
    init();
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
