//
// Created by telenewbie on 2020/3/24.
//

#ifndef NDK_TEST_INT2IP_H
#define NDK_TEST_INT2IP_H

/**
 * int 转成IP 地址
 *
 */
#include <string.h>
#include <unistd.h>
#include <string>
#include <cstring>

#define _IP_MARK "."

//IP转换成int
unsigned int IPtoINT(const std::string &strIP);

//int转换成IP
std::string INTtoIP(uint32_t num);

#endif //NDK_TEST_INT2IP_H
