#ifndef __TXZ_CACHE_DNS_H__
#define __TXZ_CACHE_DNS_H__

#include <stdint.h>
#include <string>
#include <vector>

namespace txz
{
namespace dns
{
namespace cache
{

typedef enum
{
    IP_TYPE_BACKEND = -1, //后台后台返回
    IP_TYPE_DNS = 0, //dns获取
    IP_TYPE_DEFAULT = 1, //默认
    IP_TYPE_MAX
} EIpType;   //ip类别

//刷新某个域名的cache
int autoUpdateOneCache(
        const std::string &domain); //自动更新某个域名 返回doDns的结果
//刷新整个的cache
void updateAllCache(
        bool isTimeOutUpdate = false); //全局更新，是否按超时更新

//添加ip
void addCacheIp(
        const std::string &domain,
        const uint32_t &Ip,
        EIpType type = IP_TYPE_DEFAULT);

//添加ip
void addCacheIp(
        const std::string &domain,
        std::vector< uint32_t >& ipv4,
        EIpType type = IP_TYPE_DEFAULT);

//更新ip，会抛弃没涉及的ip(从dns获取)
void updateOneCache(
        const std::string &domain,
        const std::vector< uint32_t >& ipv4,
        EIpType type = IP_TYPE_DNS);

//获取ip 返回0为成功 1为失败
int getCacheIp(
        const std::string& domain,
        std::vector< uint32_t >& ipv4);

//上ip错误
void ReportIpFailed(
        const std::string &domain,
        const uint32_t &Ip);
} //end namespace cache
} //end namespace dns
} //end namespace txz

#endif // __TXZ_CACHE_DNS_H__
