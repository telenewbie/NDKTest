#ifndef __CHARSET_UTIL_H__
#define __CHARSET_UTIL_H__

#include <stdio.h>
#include <string>
#if defined(_MSC_VER) && _MSC_VER > 1310
// Visual C++ 2005 and later require the source files in UTF-8, and all strings 
// to be encoded as wchar_t otherwise the strings will be converted into the 
// local multibyte encoding and cause errors. To use a wchar_t as UTF-8, these 
// strings then need to be convert back to UTF-8. This function is just a rough 
// example of how to do this.
#include <windows.h>
const char * ConvertToUTF8(const wchar_t * pStr);
std::wstring ConvertFromUtf8ToUtf16(const std::string& str);
#define utf16(str) ConvertFromUtf8ToUtf16(str)
# define utf8(str)  ConvertToUTF8(L##str)
#else
// Visual C++ 2003 and gcc will use the string literals as is, so the files 
// should be saved as UTF-8. gcc requires the files to not have a UTF-8 BOM.
# define utf8(str)  str
#endif

class CharsetUtil
{
public:
    typedef int uchar_type; //定义Unicode字符类型
    typedef ::std::basic_string< uchar_type > ustring_type; //定义Unicode字符串类型
public:
    ///
    static   std::string GBKToUTF8(const std::string& sGBKStr);
    static  std::string UTF16ToUTF8(const std::string& sU16Str);
    static bool isGBK(const std::string& sData);


    //////////////////////////////////////////////////////////////////////////////
    static size_t getUtf8Len(
            const char *pszUtf8,
            size_t nUtf8Len)
    {
        uchar_type uch;
        size_t i = 0;
        size_t count = 0;
        while (i < nUtf8Len)
        {
            i += Utf8ToUnicodeOne(pszUtf8 + i, uch);
            count++;
        }
        return count;
    }

    static size_t getUtf8Len(
            const ::std::string& strUtf)
    {
        return getUtf8Len(strUtf.c_str(), strUtf.length());
    }

    //////////////////////////////////////////////////////////////////////////////
    static int Utf8ToUnicode(
            const std::string &str,
            ustring_type &ustr)
    {
        int bufLen = (int)getUtf8Len(str.c_str(), (int)str.size());
        if (bufLen > 0)
        {
            ustr.resize(bufLen);
            Utf8ToUnicode(str.c_str(), str.size(), &(ustr[0]), ustr.size());
        }
        return 0;
    }

    static int Utf8ToUnicode(
            const char* pszUtf8,
            size_t nUtf8Len,
            uchar_type *puszUni,
            size_t nUniLen)
    {
        size_t i = 0, count = 0;
        while (i < nUtf8Len)
        {
            i += Utf8ToUnicodeOne(pszUtf8 + i, puszUni[count]);
            count++;
            //if (count >= nUniLen) return -1;
        }
        puszUni[count] = 0;
        return (int)count;
    }

    static int Utf8ToUnicode(
            const char* pszUtf8,
            size_t nUtf8Len,
            ustring_type& strUni)
    {
        size_t i = 0, count = 0;
        uchar_type uch = 0;
        strUni.clear();
        strUni.reserve(nUtf8Len);
        while (i < nUtf8Len)
        {
            i += Utf8ToUnicodeOne(pszUtf8 + i, uch);
            strUni.push_back(uch);
            count++;
        }
        return (int)count;
    }

    static int Utf8ToUnicode(
            const ::std::string& strUtf8,
            uchar_type *puszUni,
            size_t nUniLen)
    {
        return Utf8ToUnicode(strUtf8.data(), strUtf8.size(), puszUni, nUniLen);
    }

    static ustring_type Utf8ToUnicode(
            const std::string &str)
    {
        ustring_type ret;
        Utf8ToUnicode(str, ret);
        return ret;
    }
    static ustring_type Utf8ToUnicode(
            const char* pszUtf8,
            size_t nUtf8Len)
    {
        ustring_type ret;
        Utf8ToUnicode(pszUtf8, nUtf8Len, ret);
        return ret;
    }

    //////////////////////////////////////////////////////////////////////////////

    static int UnicodeToUtf8(
            const ustring_type& ustr,
            std::string &str)
    {
        int bufLen = getUtf8LenFromUni(ustr.data(), (int)ustr.size());
        if (bufLen > 0)
        {
            str.resize(bufLen);
            UnicodeToUtf8(ustr.data(), ustr.size(), &(str[0]), str.size());
        }
        return 0;
    }

    static int UnicodeToUtf8(
            const uchar_type* puszStr,
            size_t nUniLen,
            std::string &str)
    {
        int bufLen = getUtf8LenFromUni(puszStr, (int)nUniLen);
        if (bufLen > 0)
        {
            str.resize(bufLen);
            UnicodeToUtf8(puszStr, nUniLen, &(str[0]), str.size());
        }
        return 0;
    }

    static int UnicodeToUtf8(
            const uchar_type* puszStr,
            size_t nUniLen,
            char *pszStr,
            size_t nLen)
    {
        size_t i = 0, count = 0;
        while (i < nUniLen)
        {
            //if (count + UnicodeToUtf8OneLen(puszStr[i]) >= nLen) return -1;
            int l = UnicodeToUtf8One(puszStr[i], pszStr + count);
            i++;
            count += l;
        }
        pszStr[count] = '\0';
        return (int)count;
    }

    static int UnicodeToUtf8(
            const ustring_type& ustrUni,
            char *pszStr,
            size_t nLen)
    {
        return UnicodeToUtf8(ustrUni.data(), ustrUni.size(), pszStr, nLen);
    }

    static ::std::string UnicodeToUtf8(
            const ustring_type& ustr)
    {
        ::std::string ret;
        UnicodeToUtf8(ustr, ret);
        return ret;
    }

    static ::std::string UnicodeToUtf8(
            const uchar_type* puszUni,
            size_t nLen)
    {
        ::std::string ret;
        UnicodeToUtf8(puszUni, nLen, ret);
        return ret;
    }


    //////////////////////////////////////////////////////////////////////////////
private:
    static int getUtf8LenFromUni(
            const uchar_type *puszUni,
            int nUniLen)
    {
        int i = 0;
        int count = 0;
        while (i < nUniLen)
        {
            count += UnicodeToUtf8OneLen(puszUni[i]);
            i++;
        }
        return count;
    }

    static int Utf8ToUnicodeOne(
            const char* utf8,
            uchar_type& wch)
    {
        unsigned char firstCh = utf8[0];
        if (firstCh >= 0xC0)
        {
            int afters, code;
            if ((firstCh & 0xE0) == 0xC0)
            {
                afters = 2;
                code = firstCh & 0x1F;
            }
            else if ((firstCh & 0xF0) == 0xE0)
            {
                afters = 3;
                code = firstCh & 0xF;
            }
            else if ((firstCh & 0xF8) == 0xF0)
            {
                afters = 4;
                code = firstCh & 0x7;
            }
            else if ((firstCh & 0xFC) == 0xF8)
            {
                afters = 5;
                code = firstCh & 0x3;
            }
            else if ((firstCh & 0xFE) == 0xFC)
            {
                afters = 6;
                code = firstCh & 0x1;
            }
            else
            {
                wch = firstCh;
                return 1;
            }
            for (int k = 1; k < afters; ++k)
            {
                if ((utf8[k] & 0xC0) != 0x80)
                {
                    //判断失败，不符合UTF8编码的规则，直接当成一个ANSI字符返回
                    wch = firstCh;
                    return 1;
                }
                code <<= 6;
                code |= (unsigned char) utf8[k] & 0x3F;
            }
            wch = code;
            return afters;
        }
        wch = firstCh;
        return 1;
    }

    static int UnicodeToUtf8OneLen(
            uchar_type wchar)
    {
        if (wchar < 0xC0)
        {
            return 1;
        }
        else if (wchar < 0x800)
        {
            return 2;
        }
        else if (wchar < 0x10000)
        {
            return 3;
        }
        else if (wchar < 0x200000)
        {
            return 4;
        }
        else if (wchar < 0x4000000)
        {
            return 5;
        }
        else if ((unsigned)wchar < 0x80000000)
        {
            return 6;
        }
        return 0;
    }

    static int UnicodeToUtf8One(
            uchar_type wchar,
            char *utf8)
    {
        int len = 0;
        if (wchar < 0xC0)
        {
            utf8[len++] = (char) wchar;
        }
        else if (wchar < 0x800)
        {
            utf8[len++] = 0xc0 | (wchar >> 6);
            utf8[len++] = 0x80 | (wchar & 0x3f);
        }
        else if (wchar < 0x10000)
        {
            utf8[len++] = 0xe0 | (wchar >> 12);
            utf8[len++] = 0x80 | ((wchar >> 6) & 0x3f);
            utf8[len++] = 0x80 | (wchar & 0x3f);
        }
        else if (wchar < 0x200000)
        {
            utf8[len++] = 0xf0 | ((int) wchar >> 18);
            utf8[len++] = 0x80 | ((wchar >> 12) & 0x3f);
            utf8[len++] = 0x80 | ((wchar >> 6) & 0x3f);
            utf8[len++] = 0x80 | (wchar & 0x3f);
        }
        else if (wchar < 0x4000000)
        {
            utf8[len++] = 0xf8 | ((int) wchar >> 24);
            utf8[len++] = 0x80 | ((wchar >> 18) & 0x3f);
            utf8[len++] = 0x80 | ((wchar >> 12) & 0x3f);
            utf8[len++] = 0x80 | ((wchar >> 6) & 0x3f);
            utf8[len++] = 0x80 | (wchar & 0x3f);
        }
        else if ((unsigned)wchar < 0x80000000)
        {
            utf8[len++] = 0xfc | ((int) wchar >> 30);
            utf8[len++] = 0x80 | ((wchar >> 24) & 0x3f);
            utf8[len++] = 0x80 | ((wchar >> 18) & 0x3f);
            utf8[len++] = 0x80 | ((wchar >> 12) & 0x3f);
            utf8[len++] = 0x80 | ((wchar >> 6) & 0x3f);
            utf8[len++] = 0x80 | (wchar & 0x3f);
        }
        return len;
    }


};

#endif
