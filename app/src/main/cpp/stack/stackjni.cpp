//#include <string>
#define UNW_LOCAL_ONLY
#define LOG_NDEBUG
#include <android/log.h>
#include "unwind/libunwind.h"
#include "backtrace/Backtrace.h"
//#include "libbacktrace/BacktraceLog.h"
#include <string>
#include <memory>
#include <dlfcn.h>

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

const char* dump_backtrace_to_string(Backtrace* backtrace) {
    std::string result = "";
    for (size_t i = 0; i < backtrace->NumFrames(); i++) {
        result += backtrace->FormatFrameData(i) + "\n";
    }
    return result.c_str();
}
void dump_backtrace_to_log(Backtrace* backtrace) {
    LOGE("%s", "dump_backtrace_to_log");
    for (size_t i = 0; i < backtrace->NumFrames(); i++) {
        LOGE("back=%s", backtrace->FormatFrameData(i).c_str());
        printf("Backtrace = %s\n", backtrace->FormatFrameData(i).c_str());
    }
}



#ifdef __cplusplus
extern "C" {
#endif

const char* dump_backtrace(bool isLog) {
    void* libbacktrace = dlopen( "/system/lib/libbacktrace.so", RTLD_LAZY |RTLD_LOCAL );
    if ( !libbacktrace ){
        LOGE("libbacktrace.so error ");
        return NULL;
    }
    Backtrace* (*create)(int,int,void*);
    *(void**)&create = dlsym( libbacktrace, "_ZN9Backtrace6CreateEiiP12BacktraceMap" );
    if ( !create ){
        LOGE("libbacktrace.so dlsym ");
        return NULL;
    }
    Backtrace * backtrace =create(getpid(), 0, NULL );// create(BACKTRACE_CURRENT_PROCESS, BACKTRACE_CURRENT_THREAD);
    if (backtrace->Unwind(0)) {
        if (isLog) {

            dump_backtrace_to_log(backtrace);
            return NULL;
        } else {
            return dump_backtrace_to_string(backtrace);
        }
    } else {

        LOGE("%s", "Unwind Error");
    }

    delete backtrace;
    return "";
}

void printBacktrace () {
    dump_backtrace(true);

}

const char* getBacktrace(){
    return dump_backtrace(false);
}

#ifdef __cplusplus
}
#endif