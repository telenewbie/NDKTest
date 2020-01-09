//
// Created by telenewbie on 2020/1/9.
//

#ifndef MY_APPLICATION_LOGUTIL_H
#define MY_APPLICATION_LOGUTIL_H

#include <android/log.h>

//__android_log_print(ANDROID_LOG_DEBUG, "telenewbie", "message:[%s:%p]you set jvm ", __FUNCTION__,vm);
// Android log function wrappers
static const char *kTAG = "telenewbie-jni";
#define LOGD(...) \
  ((void)__android_log_print(ANDROID_LOG_DEBUG, kTAG, __VA_ARGS__))
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))
#endif //MY_APPLICATION_LOGUTIL_H
