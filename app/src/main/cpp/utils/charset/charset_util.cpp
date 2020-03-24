#include "charset_util.h"
#include <vector>
#include "tconv_g2u.h"
#if defined(_MSC_VER) && _MSC_VER > 1310
// Visual C++ 2005 and later require the source files in UTF-8, and all strings 
// to be encoded as wchar_t otherwise the strings will be converted into the 
// local multibyte encoding and cause errors. To use a wchar_t as UTF-8, these 
// strings then need to be convert back to UTF-8. This function is just a rough 
// example of how to do this.
#include <windows.h>
const char * ConvertToUTF8(const wchar_t * pStr) {
	static char szBuf[1024];
	WideCharToMultiByte(CP_UTF8, 0, pStr, -1, szBuf, sizeof(szBuf), NULL, NULL);
	return szBuf;
}
std::wstring ConvertFromUtf8ToUtf16(const std::string& str)
{
	std::wstring convertedString;
	int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
	if (requiredSize > 0)
	{
		std::vector<wchar_t> buffer(requiredSize);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], requiredSize);
		convertedString.assign(buffer.begin(), buffer.end() - 1);
	}
	return convertedString;
}
#endif

//GBK编码转换到UTF8编码
std::string CharsetUtil::GBKToUTF8(const std::string& sGBKStr)
{
    const char* pData = sGBKStr.data();
    size_t nSize = sGBKStr.size();

    std::string sBigEndianData;
    if ( *(uint16_t *)"\xFE\xFF" == *(uint16_t*)(sGBKStr.data()) )//(Big Endian)  FE FF
    {
        sBigEndianData.reserve(sGBKStr.size()-2);
          for ( size_t i=0; i<(sGBKStr.size()-2)/2; ++i )
          {
              sBigEndianData.push_back(sGBKStr[2+i*2+1]);
              sBigEndianData.push_back(sGBKStr[2+i*2]);
          }
          pData = sBigEndianData.data();
          nSize = sBigEndianData.size();
    }
    else if ( *(uint16_t *)"\xFF\xFE" == *(uint16_t*)(sGBKStr.data()) ) //(Little Endian)    FF FE
    {
        pData += 2;
        nSize -= 2;
    }


    std::string sResult;
#ifdef WIN32

    int nRetLen = 0;

    nRetLen = ::MultiByteToWideChar(CP_ACP, 0, (char *) pData, -1, NULL, NULL); //获取转换到Unicode编码后所需要的字符空间长度
    wchar_t * lpUnicodeStr = new WCHAR[nRetLen + 1]; //为Unicode字符串空间
    nRetLen = ::MultiByteToWideChar(CP_ACP, 0, (char *) pData, -1, lpUnicodeStr, nRetLen); //转换到Unicode编码
    if (!nRetLen) //转换失败则出错退出
        return 0;

    nRetLen = ::WideCharToMultiByte(CP_UTF8, 0, lpUnicodeStr, -1, NULL, 0, NULL, NULL); //获取转换到UTF8编码后所需要的字符空间长度

    sResult.resize(nRetLen);

    ::WideCharToMultiByte(CP_UTF8, 0, lpUnicodeStr, -1, (char *) sResult.data(), nRetLen, NULL, NULL); //转换到UTF8编码

    if (lpUnicodeStr) delete[] lpUnicodeStr;
#else

    size_t dwUtfLen = 4 * nSize;
    sResult.resize(dwUtfLen);
    tconv_gbk2utf8(pData, nSize, (char*)sResult.data(), &dwUtfLen);
    sResult.resize(dwUtfLen);
#endif
    return sResult;
}


std::string CharsetUtil::UTF16ToUTF8(const std::string& sU16Str)
{
    const char* pData = sU16Str.data();
    size_t nSize = sU16Str.size();
    std::string sBigEndianData;

    std::string sResult;
    size_t dwUtfLen = 4 * sU16Str.size();
    sResult.resize(dwUtfLen);
    if ( *(uint16_t *)"\xFE\xFF" == *(uint16_t*)(sU16Str.data()) )//(Big Endian)  FE FF
    {
        sBigEndianData.reserve(sU16Str.size()-2);
        for ( size_t i=0; i<(sU16Str.size()-2)/2; ++i )
        {
            sBigEndianData.push_back(sU16Str[2+i*2+1]);
            sBigEndianData.push_back(sU16Str[2+i*2]);
        }
        pData = sBigEndianData.data();
        nSize = sBigEndianData.size();

    }
    else if ( *(uint16_t *)"\xFF\xFE" == *(uint16_t*)(sU16Str.data()) ) //(Little Endian)    FF FE
    {
        pData += 2;
        nSize -= 2;
    }

    dwUtfLen = tconv_utf16to8((uint16_t*)(pData), nSize/2, (unsigned char*)sResult.data(), dwUtfLen);
    sResult.resize(dwUtfLen);

    return sResult;
}

bool CharsetUtil::isGBK(const std::string& sData)
{
    bool bRe = false;
    for ( size_t i=0; i<sData.size(); ++i )
    {
        unsigned char uc =  sData[i];
        if ( uc >= 30 && uc < 127 )
            continue;
        else
        {
            if ( i+1 <=sData.size() )
            {
                if ( !TCONV_IS_GBK_CHAR(sData[i], sData[i+1])  )
                {
                    return false;
                }
                else
                {
                    bRe = true;
                }
                ++i;
            }
        }
    }

    for ( size_t i=0; i<sData.size();)
    {
        size_t nLen = TCONV_UTF8_LENGTH(sData[i]);
		if ( nLen == 2 )
		{	
			return true;
		}
        else if ( nLen >=1 )
        {
            i += nLen;
            if ( i == sData.size() )
                return false;
            else if ( i > sData.size() )
                return true;
        }
        else
        {
            return true;
        }
    }

    return bRe;
}
