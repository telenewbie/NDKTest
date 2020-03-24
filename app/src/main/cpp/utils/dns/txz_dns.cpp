#include <ctype.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <assert.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <string.h>
#include <stdio.h>
#include <vector>

#include "txz_dns.h"
#include "txz_cache_dns.h"

#define BUF_SIZE    1024
#define DEFAULT_DNS_SERVER_PORT (53)

namespace txz {
    namespace dns {

#pragma pack(1)

#if 0

        typedef struct _DNS_HDR
        {
            uint16_t id;
            uint16_t tag;
            uint16_t numq;
            uint16_t numa;
            uint16_t numa1;
            uint16_t numa2;
        }DNS_HDR;

#else

        typedef struct {
            uint16_t id;       // identification number
            unsigned char rd :1;     // recursion desired
            unsigned char tc :1;     // truncated message
            unsigned char aa :1;     // authoritive answer
            unsigned char opcode :4; // purpose of message
            unsigned char qr :1;     // query/response flag
            unsigned char rcode :4;  // response code
            unsigned char cd :1;     // checking disabled
            unsigned char ad :1;     // authenticated data
            unsigned char z :1;      // its z! reserved
            unsigned char ra :1;     // recursion available
            uint16_t q_count;  // number of question entries
            uint16_t ans_count; // number of answer entries
            uint16_t auth_count; // number of authority entries
            uint16_t add_count; // number of resource entries
        } DNS_HDR;

#endif

        typedef struct _DNS_QER {
            uint16_t type;
            uint16_t classes;
        } DNS_QER;

        typedef struct {
            uint16_t name;
            uint16_t type;
            uint16_t classes;
        } DNS_QUERY;

#pragma pack()

        typedef struct {
            uint16_t name;
            uint16_t type;
            uint16_t classes;
            char time_to_live[4]; //有效期
            uint16_t data_len; //数据长度
        } DNS_RR; //回答区域，授权区域和附加区域

        int doDns(
                const std::string &domain,
                std::vector<uint32_t> &ipv4,
                const std::vector<struct sockaddr_in> &dns_server,
                int timeout_msec,
                bool use_cache) {
            struct in_addr test_addr;
            if (inet_pton(AF_INET, domain.c_str(), &(test_addr)) != 0) {
                uint32_t ip = *((uint32_t *) (&test_addr));
                ipv4.push_back(ip);
                printf("conver ipv4[%s]: %08X\n", domain.c_str(), ip);
                return 0;
            }

            if (use_cache == false) return _doDns(domain, ipv4, dns_server, timeout_msec);

            int ret = txz::dns::cache::getCacheIp(domain, ipv4);
            if (ret == 0) {
                return ret;
            }
            ret = _doDns(domain, ipv4, dns_server, timeout_msec);
            if (ret == 0)    //更新缓存
                txz::dns::cache::updateOneCache(domain, ipv4, txz::dns::cache::IP_TYPE_DNS);
            return ret;
        }

        int _doDns(
                const std::string &domain,
                std::vector<uint32_t> &ipv4,
                const std::vector<struct sockaddr_in> &dns_server,
                int timeout_msec) {
            int err = 0;
            char reqbuff[BUF_SIZE];
            int domain_len = 1; //含最后一个0
            int reqlen = 0;
            char resbuff[BUF_SIZE];

            //超时设置
            struct timespec begin_ts;
            clock_gettime(CLOCK_MONOTONIC, &begin_ts);

            //数据包设置

            memset(reqbuff, 0, BUF_SIZE);

            //设置DNS_HDR
            DNS_HDR *dnshdr = (DNS_HDR *) reqbuff;
            dnshdr->id = htons(0x2000); //(U16)1;
            dnshdr->qr = 0; //0是请求包，1是响应包
            dnshdr->opcode = 0;
            dnshdr->aa = 0;
            dnshdr->tc = 0;
            dnshdr->rd = 1;
            dnshdr->ra = 1;
            dnshdr->z = 0;
            dnshdr->ad = 0;
            dnshdr->cd = 0;
            dnshdr->rcode = 0;
            dnshdr->q_count = htons(1);
            dnshdr->ans_count = 0;
            dnshdr->auth_count = 0;
            dnshdr->add_count = 0;

            char *p = reqbuff + sizeof(DNS_HDR);
            size_t prev = 0;
            while (true) {
                size_t next = domain.find('.', prev);
                size_t len = (next == std::string::npos) ? (domain.size() - prev) : (next - prev);
                if (len > 127) return -99;
                *p = (char) len;
                memcpy(p + 1, domain.data() + prev, len);
                p += len + 1;
                domain_len += len + 1;
                if (next == std::string::npos) break;
                prev = next + 1;
            }
            *p = 0;

            //设置DNS_QER
            DNS_QER *dnsqer = (DNS_QER *) (p + 1);
            dnsqer->type = htons(1); //查询类型为A，由域名获得IPv4地址
            dnsqer->classes = htons(1); //查询类，Internet数据

            reqlen = sizeof(DNS_HDR) + sizeof(DNS_QER) + domain.size() + 2;

            const socklen_t socklen = sizeof(struct sockaddr_in);

            struct TFdRec {
                int fd;
                int send_status = 1; //1待发送
                int recv_status = 1; //1待接收
            };

            //初始化套接字
            std::vector<TFdRec> fds;
            int maxfd = -1;
            for (size_t i = 0; i < dns_server.size(); ++i) {
                TFdRec rec;
                rec.fd = socket(AF_INET, SOCK_DGRAM, 0);
                if (rec.fd >= 0) {
                    if (fcntl(rec.fd, F_SETFL, fcntl(rec.fd, F_GETFL, 0) | O_NONBLOCK) < 0) {
                        close(rec.fd);
                        rec.fd = -1;
                    }
                }
                if (rec.fd > maxfd) maxfd = rec.fd;
                fds.emplace_back(std::move(rec));
            }

            if (maxfd < 0) {
                //没有生成任何套接字
                return -1;
            }

            while (true) {
                //设置套接字集合
                fd_set rfds;
                fd_set wfds;
                FD_ZERO(&rfds);
                FD_ZERO(&wfds);
                bool empty = true;
                for (size_t i = 0; i < fds.size(); ++i) {
                    if (fds[i].fd >= 0) {
                        if (fds[i].recv_status > 0) {
                            FD_SET(fds[i].fd, &rfds);
                            empty = false;
                        }
                        if (fds[i].send_status > 0) {
                            FD_SET(fds[i].fd, &wfds);
                            empty = false;
                        }
                    }
                }

                if (empty) {
                    err = -2;
                    break;
                }

                //计算超时
                struct timespec cur_ts;
                clock_gettime(CLOCK_MONOTONIC, &cur_ts);
                int64_t t = timeout_msec * 1000 - (cur_ts.tv_sec - begin_ts.tv_sec) * 1000000 -
                            (cur_ts.tv_nsec - begin_ts.tv_nsec) / 1000;
                if (t <= 0) {
                    err = -4;
                    break;
                }
                struct timeval tv;
                tv.tv_sec = t / 1000000;
                tv.tv_usec = t % 1000000;

                errno = 0;
                int ret = select(maxfd + 1, &rfds, &wfds, nullptr, &tv);

                if (ret < 0) {
                    if (errno == EAGAIN) {
                        continue;
                    }
                    err = -3;
                    break;
                }

                if (ret == 0) {
                    err = -4; //超时
                    break;
                }

                bool getresult = false;
                for (size_t i = 0; i < fds.size(); ++i) {
                    if (fds[i].fd >= 0) {
                        if (FD_ISSET(fds[i].fd, &wfds)) {
                            errno = 0;
                            int len = sendto(fds[i].fd, reqbuff, reqlen, 0,
                                             (struct sockaddr *) &(dns_server[i]), socklen);
                            if (len == reqlen) {
                                fds[i].send_status = 0;
                            } else {
                                if (errno != EAGAIN) {
                                    fds[i].send_status = -1;
                                }
                            }
                        }
                        if (FD_ISSET(fds[i].fd, &rfds)) {
                            struct sockaddr_in resaddr;
                            socklen_t ressocklen = sizeof(resaddr);
                            int len = recvfrom(fds[i].fd, resbuff, BUF_SIZE, 0,
                                               (struct sockaddr *) &resaddr, &ressocklen);
                            if (len < (int) (domain_len + sizeof(DNS_HDR) + sizeof(DNS_QER))) {
                                //长度不足
                                continue;
                            }
                            if (resaddr.sin_addr.s_addr != dns_server[i].sin_addr.s_addr ||
                                resaddr.sin_port != dns_server[i].sin_port) {
                                //判断回包是否来自要求的服务器
                                continue;
                            }
                            //判断响应是否为请求的域名
                            int cmpret = memcmp(resbuff + sizeof(DNS_HDR),
                                                reqbuff + sizeof(DNS_HDR), domain_len);
                            if (cmpret != 0) {
                                //不是当前域名的请求
                                //printf("cmpret = %d\n", cmpret);
                                continue;
                            }
                            DNS_HDR *res = (DNS_HDR *) resbuff;
                            if (res->qr != 1) {
                                //不是响应包
                                continue;
                            }
                            if (res->ans_count == 0) {
                                //响应没有结果
                                fds[i].recv_status = -1;
                                continue;
                            }

                            ipv4.clear();

                            int ans_count = ntohs(res->ans_count);
                            int offset = sizeof(DNS_HDR) /*12*/+ sizeof(DNS_QER) /*4*/+ domain_len;
                            // printf("ans_count=%d, offset=%d, len=%d\n", ans_count, offset, len);

#if 0
                            FILE* fp = fopen("dns.dat", "wb");
                            fwrite(resbuff, 1, len, fp);
                            fclose(fp);
#endif
                            uint16_t data_len = 0;
                            for (int i = 0;
                                 i < ans_count && (int) (offset + sizeof(DNS_RR)) <= len; offset +=
                                                                                                  data_len +
                                                                                                  sizeof(DNS_RR), ++i) {
                                DNS_RR *rr = (DNS_RR *) (resbuff + offset);
                                data_len = ntohs(rr->data_len);
                                uint16_t type = ntohs(rr->type);
                                uint16_t classes = ntohs(rr->classes);
                                // printf("data_len=%d, type=%d, classes=%d\n", data_len, type, classes);
                                if (type != 1) continue;
                                if (classes != 1) continue;
                                if (data_len != 4) continue;
                                if ((int) (offset + sizeof(DNS_RR) + data_len) > len) continue;
                                ipv4.push_back(*((uint32_t *) (resbuff + offset + sizeof(DNS_RR))));
                            }
                            getresult = true;
                            err = 0;
                            break;
                        }
                    }
                }

                if (getresult) break;
            };

            for (size_t i = 0; i < fds.size(); ++i) {
                if (fds[i].fd >= 0) {
                    close(fds[i].fd);
                }
            }

            return err;
        }

        int doDns(
                const std::string &domain,
                std::vector<uint32_t> &ipv4,
                const std::vector<std::pair<std::string, uint16_t> > &dns_server,
                int timeout_msec,
                bool use_cache) {
            std::vector<struct sockaddr_in> ss;
            ss.reserve(dns_server.size());

            //DNS服务器设置
            for (size_t i = 0; i < dns_server.size(); ++i) {
                struct sockaddr_in servaddr;
                memset(&servaddr, 0, sizeof(servaddr));
                servaddr.sin_family = AF_INET;
                inet_pton(AF_INET, dns_server[i].first.c_str(), &(servaddr.sin_addr));
                servaddr.sin_port = htons(dns_server[i].second);
                ss.emplace_back(std::move(servaddr));
            }

            return doDns(domain, ipv4, ss, timeout_msec, use_cache);
        }

        int doDns(
                const std::string &domain,
                std::vector<uint32_t> &ipv4,
                const std::vector<std::string> &dns_server,
                uint16_t port,
                int timeout_msec,
                bool use_cache) {
            std::vector<struct sockaddr_in> ss;
            ss.reserve(dns_server.size());

            //DNS服务器设置
            for (size_t i = 0; i < dns_server.size(); ++i) {
                struct sockaddr_in servaddr;
                memset(&servaddr, 0, sizeof(servaddr));
                servaddr.sin_family = AF_INET;
                inet_pton(AF_INET, dns_server[i].c_str(), &(servaddr.sin_addr));
                servaddr.sin_port = htons(port);
                ss.emplace_back(std::move(servaddr));
            }

            return doDns(domain, ipv4, ss, timeout_msec, use_cache);
        }

        int doDns(
                const std::string &domain,
                std::vector<uint32_t> &ipv4,
                const std::vector<std::string> &dns_server,
                int timeout_msec,
                bool use_cache) {
            std::vector<struct sockaddr_in> ss;
            ss.reserve(dns_server.size());

            //DNS服务器设置
            for (size_t i = 0; i < dns_server.size(); ++i) {
                struct sockaddr_in servaddr;
                memset(&servaddr, 0, sizeof(servaddr));
                servaddr.sin_family = AF_INET;
                inet_pton(AF_INET, dns_server[i].c_str(), &(servaddr.sin_addr));
                servaddr.sin_port = htons(DEFAULT_DNS_SERVER_PORT);
                ss.emplace_back(std::move(servaddr));
            }

            return doDns(domain, ipv4, ss, timeout_msec, use_cache);
        }

        int doDns(
                const std::string &domain,
                std::vector<uint32_t> &ipv4,
                const std::string &dns_server,
                uint16_t port,
                int timeout_msec,
                bool use_cache) {
            std::vector<struct sockaddr_in> ss;

            struct sockaddr_in servaddr;
            memset(&servaddr, 0, sizeof(servaddr));
            servaddr.sin_family = AF_INET;
            inet_pton(AF_INET, dns_server.c_str(), &(servaddr.sin_addr));
            servaddr.sin_port = htons(port);
            ss.emplace_back(std::move(servaddr));

            return doDns(domain, ipv4, ss, timeout_msec, use_cache);
        }

#ifdef ANDROID

#include <sys/system_properties.h>

        int getSysDnsServer(
                std::vector<std::string> &dns_server) {
            char buf[PROP_VALUE_MAX];
            int ret = __system_property_get("net.dns1", buf);
            if (ret >= 7) {
                dns_server.emplace_back(buf, ret);
            }
            ret = __system_property_get("net.dns2", buf);
            if (ret >= 7) {
                dns_server.emplace_back(buf, ret);
            }

            return 0;
        }

#else

        int getSysDnsServer(
                std::vector< std::string >& dns_server)
        {
            FILE *file = fopen("/etc/resolv.conf", "r");
            if (file == nullptr)
            {
                return -1;
            }

            char buff[BUF_SIZE] = { 0, };

            while (fgets(buff, BUF_SIZE, file))
            {
                char *begin = strstr(buff, "nameserver");
                if (begin)
                {
                    begin += 10;

                    while (true)
                    {
                        char chbegin = *begin;
                        if (chbegin == ' ' || chbegin == '\t')
                        {
                            ++begin;
                            continue;
                        }
                        if (chbegin >= '0' && chbegin <= '9')
                        {
                            break;
                        }
                        begin = nullptr;
                        break;
                    }
                    if (begin == nullptr) continue;
                    char* end = begin + 1;
                    while (true)
                    {
                        char chend = *end;
                        if ((chend >= '0' && chend <= '9') || chend == '.')
                        {
                            ++end;
                            continue;
                        }
                        if (chend == '#' || chend == '\0' || chend == ' ' || chend == '\t')
                        {
                            break;
                        }
                        end = nullptr;
                        break;
                    }
                    if (end == nullptr) continue;
                    dns_server.emplace_back(begin, (size_t) ((long) (void*) end - (long) (void*) begin));
                }
            }
            fclose(file);

            return 0;
        }

#endif

        int doDns(
                const std::string &domain,
                std::vector<uint32_t> &ipv4,
                int timeout_msec,
                bool use_cache) {
            std::vector<std::string> dns_server;
            getSysDnsServer(dns_server);
            dns_server.emplace_back("8.8.8.8"); //Google
            dns_server.emplace_back("114.114.114.114"); //电信
            dns_server.emplace_back("180.76.76.76"); //百度
            dns_server.emplace_back("223.5.5.5"); //阿里
            dns_server.emplace_back("223.6.6.6"); //阿里
            return doDns(domain, ipv4, dns_server, DEFAULT_DNS_SERVER_PORT, timeout_msec,
                         use_cache);
        }

    } //end namespace dns
} //end namespace txz

#ifdef USE_TXZ_DNS_GETADDRINFO

int getaddrinfo(
        const char *node,
        const char *service,
        const struct addrinfo *hints,
        struct addrinfo **res)
{
    printf("enter custom getaddrinfo, %s:%s\n",node,service);
    std::vector< uint32_t > ipv4;
    int ret = txz::dns::doDns(node, ipv4,2000);
    if (ret != 0) return ret;
    printf("getaddrinfo count : %d first dns:%d \n", (int) ipv4.size(),ipv4[0]);
    uint16_t port = 0;
    if (nullptr != service)
    {
        port = htons(atoi(service));
    }
    for (auto it = ipv4.begin(); it != ipv4.end(); ++it)
    {
        uint32_t ip = *it;
        struct addrinfo * cur = *res = (struct addrinfo *) malloc(sizeof(struct addrinfo));
        memset(cur, 0, sizeof(struct addrinfo));
        cur->ai_family = AF_INET;
        cur->ai_protocol = IPPROTO_IP;
        //cur->ai_canonname = 0;
        //cur->ai_socktype = 0;
        cur->ai_addr = (struct sockaddr*) malloc(sizeof(struct sockaddr_in));
        cur->ai_addrlen = sizeof(struct sockaddr_in);
        memset(cur->ai_addr, 0, sizeof(struct sockaddr_in));
        ((struct sockaddr_in*) cur->ai_addr)->sin_family = AF_INET;
        ((struct sockaddr_in*) cur->ai_addr)->sin_port = port;
        ((struct sockaddr_in*) cur->ai_addr)->sin_addr.s_addr = ip;
        res = &(cur->ai_next);
    }
    return 0;
}

void freeaddrinfo(struct addrinfo *res)
{
    printf("freeaddrinfo start : %p\n",res);
    while (res != nullptr)
    {
        if (res->ai_addr != nullptr)
        {
            free(res->ai_addr);
        }
        struct addrinfo *tmp = res;
        res = res->ai_next;
        free(tmp);
    }
    printf("freeaddrinfo stop\n");
}

#endif

#ifdef USE_TXZ_DNS_TEST

int main(
        int argc,
        char* argv[])
{
    std::vector< uint32_t > ipv4;
    ipv4.emplace_back(0);

    int ret = txz::dns::cache::getCacheIp(argv[1], ipv4);

    for (auto it = ipv4.begin(); it != ipv4.end(); ++it)
    {
        struct in_addr addr;
        addr.s_addr = *it;
        printf("doDns ret=%d: %s\n", ret, inet_ntoa(addr));
    }

    struct addrinfo* addrs = nullptr, *tmp;
    ret = getaddrinfo(argv[1], nullptr, nullptr, &addrs);
    tmp = addrs;
    while (tmp != nullptr)
    {
        printf("getaddrinfo ret=%d: %s\n", ret, inet_ntoa(((struct sockaddr_in*) (tmp->ai_addr))->sin_addr));
        tmp = tmp->ai_next;
    }
    txz::dns::cache::autoUpdateOneCache("baidu.com");
    txz::dns::cache::addCacheIp("baidu.com",123,txz::dns::cache::EIpType::IP_TYPE_BACKEND);
    txz::dns::cache::addCacheIp("sogou.com",456,txz::dns::cache::EIpType::IP_TYPE_BACKEND);
    txz::dns::cache::updateAllCache();
    txz::dns::cache::ReportIpFailed("baidu.com",123);
    txz::dns::cache::updateAllCache();
    freeaddrinfo(addrs);

    while(1);

    return 0;
}

#endif
