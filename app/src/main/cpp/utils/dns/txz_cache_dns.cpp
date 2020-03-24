#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <map>
#include <set>
#include <thread>
#include <string.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <pthread.h>
//#include <bits/pthreadtypes.h>
#include <arpa/inet.h>
#include <logutil.h>

#ifdef USE_TXZ_DNS_TEST
#define LOGD(fmt, args...) do {fprintf(stdout, "[txz_dns] " fmt "\n", ##args); fflush(stdout);}while(0)
#else
//#include "osal.h"
#endif

#include "txz_dns.h"
#include "txz_cache_dns.h"
// ip 有效时间 600000ms
#define IP_VALID_TIME 600000

namespace txz
{
namespace dns
{
namespace cache
{
//不对外开放
int64_t getTickCount();

typedef class TAddrStatus
{
public:
    uint32_t failed = 0; //是否失败
    uint32_t ip = 0; //ip
    uint16_t port = 0; //端口
    uint32_t weight = 0; //权重
    int64_t display = 0; //延时
    int def = 0; //默认ip类型，-1为后台返回，0为dns获取，1为默认内置

    inline bool operator<(
            const TAddrStatus& other) const
    {
        if (this->failed < other.failed) return true;       //错误越低越优先
        if (this->def < other.def) return true;              //后台类型优先
        if (this->display < other.display) return true;   //同类型延时越低越优先
        if (this->weight > other.weight) return true;   //同类型权重越高越优秀
        return false;
    }
}*PTAddrStatus;

inline bool cmpPTAddrStatus(
        const PTAddrStatus& left,
        const PTAddrStatus& right)
{
    return *left < *right;
}

struct TDomainInfo
{
    int64_t timeStamp = 0; //
    std::set< PTAddrStatus, std::function< bool(
            const PTAddrStatus&,
            const PTAddrStatus&) > > addrs;
    std::map< uint32_t, TAddrStatus > addrsMap;

    TDomainInfo() :
            timeStamp(0), addrs(cmpPTAddrStatus)
    {

    }
};

std::map< std::string, TDomainInfo > s_mDnscache;
pthread_rwlock_t m_lockRWDnscacheMap;
pthread_t s_pDoDnsThread;

static void* procDoDns(
        void*)
{
    prctl(PR_SET_NAME, "DoDns");
    while (true)
    {
        sleep(60);   //每分钟检测一次
        updateAllCache(true);  //全局更新
    }
    return nullptr;
}

class CacheModule
{
public:
    CacheModule()
    {
        pthread_rwlock_init(&m_lockRWDnscacheMap, NULL);
        pthread_create(&s_pDoDnsThread, nullptr, procDoDns, nullptr);
    }
};

static CacheModule s_CacheModule;   //单例

void updateOneCache(
        std::map< std::string, TDomainInfo >::iterator &it)
{
    LOGD("update domain : %s", it->first.c_str());
    if (it == s_mDnscache.end())
    {
        return;
    }
    std::vector< uint32_t > ipv4;
    if (doDns(it->first, ipv4, 2000, false) == 0)
    {
        pthread_rwlock_wrlock(&m_lockRWDnscacheMap);
        it->second.timeStamp = getTickCount();
        for (auto it2 = it->second.addrsMap.begin(); it2 != it->second.addrsMap.end();) //删除元素
        {
            bool has = false;
            for (auto it3 = ipv4.begin(); it3 != ipv4.end(); ++it3)
            {
                if (it2->first == *it3)
                {
                    has = true;
                    break;
                }
            }
            if (has == false)
            {
                if (it2->second.def == 0)   //如果是本地获取的，则删除
                {
                    it->second.addrsMap.erase(it2++);   //删除元素
                    continue;
                }
            }
            ++it2;
        } //endfor

        for (auto it2 = ipv4.begin(); it2 != ipv4.end(); ++it2) //插入元素
        {
            it->second.addrsMap[*it2].ip = *it2;
            ++(it->second.addrsMap[*it2].weight);
        } //endfor

        //更新vector
        it->second.addrs.clear();   //清除vector
        for (auto it2 = it->second.addrsMap.begin(); it2 != it->second.addrsMap.end(); ++it2)
        {
            it->second.addrs.insert(&(it2->second));
            LOGD("%s update ip %d", it->first.c_str(), it2->second.ip);
        } //endfor
        pthread_rwlock_unlock(&m_lockRWDnscacheMap);
    }
    else
    {
        it->second.timeStamp = 0;
    }
}

int autoUpdateOneCache(
        const std::string &domain)
{

    LOGD("update domain : %s", domain.c_str());
    int ret = 0;
    std::vector< uint32_t > ipv4;
    ret = doDns(domain, ipv4, 2000, false);    //获取ip

    pthread_rwlock_wrlock(&m_lockRWDnscacheMap);
    auto &domainCahe = s_mDnscache[domain];
    if (ret == 0)
    {
        domainCahe.timeStamp = getTickCount();
        for (auto it2 = domainCahe.addrsMap.begin(); it2 != domainCahe.addrsMap.end();) //删除元素
        {
            bool has = false;
            for (auto it3 = ipv4.begin(); it3 != ipv4.end(); ++it3)
            {
                if (it2->first == *it3)
                {
                    has = true;
                    break;
                }
            }
            if (has == false)
            {
                if (it2->second.def == 0)   //如果是本地获取的，则删除
                {
                    domainCahe.addrsMap.erase(it2++);   //删除元素
                    continue;
                }
            }
            ++it2;
        } //endfor

        for (auto it2 = ipv4.begin(); it2 != ipv4.end(); ++it2) //插入元素
        {
            domainCahe.addrsMap[*it2].ip = *it2;
        } //endfor

        //更新vector
        domainCahe.addrs.clear();   //清除vector
        for (auto it2 = domainCahe.addrsMap.begin(); it2 != domainCahe.addrsMap.end(); ++it2)
        {
            domainCahe.addrs.insert(&(it2->second));
            LOGD("%s update ip %d", domain.c_str(), it2->second.ip);
        } //endfor
    }
    else
    {
        domainCahe.timeStamp = 0;
    }
    pthread_rwlock_unlock(&m_lockRWDnscacheMap);
    return ret;
}

void updateOneCache(
        const std::string &domain,
        const std::vector< uint32_t >& ipv4,
        EIpType type)
{

    LOGD("update domain : %s", domain.c_str());

    pthread_rwlock_wrlock(&m_lockRWDnscacheMap);
    auto &domainCahe = s_mDnscache[domain];
    domainCahe.timeStamp = getTickCount();
    for (auto it2 = domainCahe.addrsMap.begin(); it2 != domainCahe.addrsMap.end();) //删除元素
    {
        bool has = false;
        for (auto it3 = ipv4.begin(); it3 != ipv4.end(); ++it3)
        {
            if (it2->first == *it3)
            {
                has = true;
                break;
            }
        }
        if (has == false)
        {
            if (it2->second.def == 0)   //如果是本地获取的，则删除
            {
                domainCahe.addrsMap.erase(it2++);   //删除元素
                continue;
            }
        }
        ++it2;
    } //endfor

    for (auto it2 = ipv4.begin(); it2 != ipv4.end(); ++it2) //插入元素
    {
        domainCahe.addrsMap[*it2].ip = *it2;
        domainCahe.addrsMap[*it2].def = type;
    } //endfor

    //更新vector
    domainCahe.addrs.clear();   //清除vector
    for (auto it2 = domainCahe.addrsMap.begin(); it2 != domainCahe.addrsMap.end(); ++it2)
    {
        domainCahe.addrs.insert(&(it2->second));
        LOGD("%s update ip %d", domain.c_str(), it2->second.ip);
    } //endfor
    pthread_rwlock_unlock(&m_lockRWDnscacheMap);
}

void updateAllCache(
        bool isTimeOutUpdate)
{
    LOGD("update cache all,isTimeOutUpdate = %d", isTimeOutUpdate);
    for (auto it = s_mDnscache.begin(); it != s_mDnscache.end(); ++it)
    {
        if ((isTimeOutUpdate == false) || (it->second.timeStamp == 0) || ((it->second.timeStamp + IP_VALID_TIME) < getTickCount())) //强制更新 或 上一次获取ip失败 或 超时
        {
            updateOneCache(it);
        }
    }
}

void addCacheIp(
        const std::string &domain,
        const uint32_t &Ip,
        EIpType type)
{
    if (domain.empty())
    {
        return;
    }

    LOGD("%s add %d[%d] ", domain.c_str(), Ip, type);
    pthread_rwlock_wrlock(&m_lockRWDnscacheMap);
    auto &domainCahe = s_mDnscache[domain];
    domainCahe.addrsMap[Ip].ip = Ip;
    domainCahe.addrsMap[Ip].def = type;
    domainCahe.addrs.insert(&(domainCahe.addrsMap[Ip]));    //插入set
    pthread_rwlock_unlock(&m_lockRWDnscacheMap);
}

void addCacheIp(
        const std::string &domain,
        std::vector< uint32_t >& ipv4,
        EIpType type)  //插入ip
{
    if (domain.empty())
    {
        return;
    }

    for (auto it = ipv4.begin(); it != ipv4.end(); ++it)
    {
        addCacheIp(domain, (*it), type);
    }
}

void ReportIpFailed(
        const std::string &domain,
        const uint32_t &Ip)
{
    pthread_rwlock_wrlock(&m_lockRWDnscacheMap);
    auto &domainCahe = s_mDnscache[domain];
    int failedConnt = ++domainCahe.addrsMap[Ip].failed;   //增加失败次数
    //更新set
    domainCahe.addrs.clear();
    for (auto it2 = domainCahe.addrsMap.begin(); it2 != domainCahe.addrsMap.end(); it2++)
    {
        domainCahe.addrs.insert(&(it2->second));
    } //endfor
    pthread_rwlock_unlock(&m_lockRWDnscacheMap);

    LOGD("report %s[%d] failed[%d] ", domain.c_str(), Ip, failedConnt);
}

int getCacheIp(
        const std::string& domain,
        std::vector< uint32_t >& ipv4)  //是否更新缓存后获取ip
{
    int ret = -1;   //默认没找到
    if (domain.empty())
    {
        return ret;
    }

    pthread_rwlock_rdlock(&m_lockRWDnscacheMap);
    auto it = s_mDnscache.find(domain);
    if (it != s_mDnscache.end())
    {
        for (auto it2 = it->second.addrs.begin(); it2 != it->second.addrs.end(); ++it2)
        {
            ipv4.push_back((**it2).ip);
        }
        LOGD("get Cache %s count %d",domain.c_str(),ipv4.size());
        ret = 0;    //找到
    }
    pthread_rwlock_unlock(&m_lockRWDnscacheMap);
    return ret;
}

int64_t getTickCount()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (((int64_t) ts.tv_sec) * 1000 + ts.tv_nsec / 1000000);
}

}    //end namespace cache
} //end namespace dns
} //end namespace txz

