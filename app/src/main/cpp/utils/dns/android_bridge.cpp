//
// Created by telenewbie on 2020/3/24.
//


#include <jni.h>
#include <android/log.h>
#include "txz_dns.h"
#include "com_example_myapplication_utils_DnsUtils_Companion.h"
#include "txz_def.h"
#include "Int2Ip.h"

#define DEFAULT_DNS_SERVER_PORT (53)

JNIEXPORT jbyteArray JNICALL Java_com_example_myapplication_utils_DnsUtils_00024Companion_getIps
        (JNIEnv *env, jobject, jstring domain) {

    // string 转  byte数组
    std::string domain_cxx = t_jstring2string(env, domain);


    std::vector<uint32_t> ipv4s;
    std::vector<std::string> dns_server;
    dns_server.emplace_back("8.8.8.8"); //Google
    dns_server.emplace_back("114.114.114.114"); //电信
    dns_server.emplace_back("180.76.76.76"); //百度
    dns_server.emplace_back("223.5.5.5"); //阿里
    dns_server.emplace_back("223.6.6.6"); //阿里
    ::txz::dns::doDns(domain_cxx, ipv4s, dns_server, DEFAULT_DNS_SERVER_PORT, 2000, false);

    // 打印ipv4 的集合

    std::string result;
    for (auto ipv4 : ipv4s) {
        std::string ip_str = INTtoIP(ipv4);
        result += ip_str;
        result += "|";
        __android_log_print(ANDROID_LOG_DEBUG, "dns", "ipv4:%d,str=%s", ipv4, ip_str.c_str());
    }
    jbyteArray bytes;
    STRING_TO_JNI_BYTE_ARRAY(env, result, bytes);
    __android_log_print(ANDROID_LOG_DEBUG, "dns", "over");
    return bytes;
}

JNIEXPORT jint JNICALL JNI_OnLoad(
        JavaVM *vm,
        void *reserved) {

    __android_log_print(ANDROID_LOG_DEBUG, "dns", "dns-lib is called");
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