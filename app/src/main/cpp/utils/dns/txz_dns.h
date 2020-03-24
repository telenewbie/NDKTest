#ifndef __TXZ_DNS_H__
#define __TXZ_DNS_H__

#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string>
#include <vector>

namespace txz
{
namespace dns
{

int _doDns(
        const std::string& domain,
        std::vector< uint32_t >& ipv4,
        const std::vector< struct sockaddr_in >& dns_server,
        int timeout_msec);

int doDns(
        const std::string& domain,
        std::vector< uint32_t >& ipv4,
        const std::vector< struct sockaddr_in >& dns_server,
        int timeout_msec,
        bool use_cache = true);

int doDns(
        const std::string& domain,
        std::vector< uint32_t >& ipv4,
        const std::vector< std::pair< std::string, uint16_t > >& dns_server,
        int timeout_msec,
        bool use_cache = true);

int doDns(
        const std::string& domain,
        std::vector< uint32_t >& ipv4,
        const std::vector< std::string >& dns_server,
        uint16_t port,
        int timeout_msec,
        bool use_cache = true);

int doDns(
        const std::string& domain,
        std::vector< uint32_t >& ipv4,
        const std::vector< std::string >& dns_server,
        int timeout_msec,
        bool use_cache = true);

int doDns(
        const std::string& domain,
        std::vector< uint32_t >& ipv4,
        const std::string & dns_server,
        uint16_t port,
        int timeout_msec,
        bool use_cache = true);

int doDns(
        const std::string& domain,
        std::vector< uint32_t >& ipv4,
        int timeout_msec,
        bool use_cache = true);

int getSysDnsServer(
        std::vector< std::string >& dns_server);

} //end namespace dns
} //end namespace txz

#endif // __TXZ_DNS_H__
