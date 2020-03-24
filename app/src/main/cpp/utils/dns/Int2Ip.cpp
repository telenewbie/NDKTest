//
// Created by telenewbie on 2020/3/24.
//

#include "Int2Ip.h"
#include <stdlib.h>
#include <stdio.h>

//IP转换成int
unsigned int IPtoINT(const std::string &strIP) {
    unsigned int nRet = 0;

    char chBuf[16] = "";
    memcpy(chBuf,strIP.c_str(), 15);

    char *szBufTemp = NULL;
    char *szBuf = strtok(chBuf, _IP_MARK);

    int i = 0;//计数
    while (NULL != szBuf)//取一个
    {
        nRet += atoi(szBuf) << ((3 - i) * 8);
        szBuf = strtok(NULL, _IP_MARK);
        i++;
    }

    return nRet;
}

//int转换成IP
std::string INTtoIP(uint32_t num) {

    std::string strRet = "";
    for (int i = 0; i < 4; i++) {
        uint32_t tmp = (num >> ((3 - i) * 8)) & 0xFF;

        strRet += std::to_string(tmp);

        if (i < 3) {
            strRet += _IP_MARK;
        }
    }

    return strRet;
}
