/*
功能:
       1.  编码转换公共函数

Created by IPD, 2006-07
Change list:

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tconv_g2u.h"

/**
 * UTF-16到UTF-8的编码转换函数
 *
 * @author  国际产品中心
 * @version  1.0
 * @param u16 	要转换的字符
 * @param u8 	输出的字符串
 * @return u8的字节数
 */
int
tconv_single_utf16to8(uint16_t u16, unsigned char * u8)
{
	if (u16 <= 0x7f){
		// 1bytes
		u8[0] = (u16 & 0xff);
		return 1;
	}
	else if (u16 <= 0x7ff){
		// 2bytes
		u8[0] = 0xc0 | ((u16 & 0x7c0) >> 6);
		u8[1] = 0x80 | (u16 & 0x3f);
		return 2;
	}

	// 3bytes
	u8[0] = 0xe0 | ((u16 & 0xf000) >> 12);
	u8[1] = 0x80 | ((u16 & 0xfc0) >> 6);
	u8[2] = 0x80 | (u16 & 0x3f);

	return 3;
}

/**
 * UTF-8到UTF-16的编码转换函数
 *
 * @author  国际产品中心
 * @version  1.0
 * @param u8 	要转换的字符串
 * @param u16 	输出的字符
 * @return 转换的u16字符数，1表示成功转换，0表示没有转换
 */
int
tconv_single_utf8to16(const unsigned char * u8, uint16_t * u16)
{
	switch(TCONV_UTF8_LENGTH(u8[0])){
	case 1:
		(*u16) = u8[0];
		break;
	case 2:
		(*u16) = ((u8[0] & 0x1f) >> 2);
		(*u16) <<= 8;
		(*u16) |= ((u8[0] & 0x3) << 6) | (u8[1] & 0x3f);
		break;
	case 3:
		(*u16) = ((u8[0] & 0xf) << 4) | ((u8[1] & 0x3f) >> 2);
		(*u16) <<= 8;
		(*u16) |= ((u8[1] & 0x3) << 6) | (u8[2] & 0x3f);
		break;
	default:
		// 不处理4bytes的
		(*u16) = 0;
		return 0;
		break;
	}

	return 1;
}

/**
 * UTF-16到UTF-8的编码转换函数，末尾补0
 *
 * @author  国际产品中心
 * @version  1.0
 * @param u16 	      要转换的字符串
 * @param u16len      u16的字符数(每字符两个字节)
 * @param u8 	      输出的字符串
 * @param u8maxlen    u8的最大字节数
 * @return u8的字节数
 */
int
tconv_utf16to8(const uint16_t * u16,
			size_t u16len,
			unsigned char * u8,
			size_t u8maxlen)
{
	const uint16_t * u16end = u16 + u16len;
	uint32_t u8len = 0;
	if (u8maxlen <= 1){
		*u8 = 0;
		return 0;
	}
	--u8maxlen;

	for (; u16 < u16end; ++u16){
		if ((*u16) <= 0x7f){
			// 1bytes
			if (u8len + 1 > u8maxlen){
				// 空间不够
				break;
			}
			u8[0] = (*u16 & 0xff);
			++u8len;
			++u8;
		}
		else if ((*u16) <= 0x7ff){
			// 2bytes
			if (u8len + 2 > u8maxlen){
				// 空间不够
				break;
			}
			u8[0] = 0xc0 | ((*u16 & 0x7c0) >> 6);
			u8[1] = 0x80 | (*u16 & 0x3f);
			u8len += 2;
			u8 += 2;
		}
		else{
			// 3bytes
			if (u8len + 3 > u8maxlen){
				// 空间不够
				break;
			}
			u8[0] = 0xe0 | ((*u16 & 0xf000) >> 12);
			u8[1] = 0x80 | ((*u16 & 0xfc0) >> 6);
			u8[2] = 0x80 | (*u16 & 0x3f);
			u8len += 3;
			u8 += 3;
		}
	}

	*u8 = 0;
	return u8len;
}

/**
 * UTF-8到UTF-16的编码转换函数，末尾补0
 *
 * @author  国际产品中心
 * @version  1.0
 * @param u8 	       要转换的字符串
 * @param u8len        u8的字节数
 * @param u16          输出的字符串
 * @param u16maxlen    u16的最大字符数(每字符两个字节)
 * @return u16的字符数
 */
int
tconv_utf8to16(const unsigned char * u8,
			size_t u8len,
			uint16_t * u16,
			size_t u16maxlen)
{
	const unsigned char * u8end = u8 + u8len;
	uint32_t u8bytes = 0;
	uint32_t u16len = 0;

	if (u16maxlen <= 1) {
		u16[0] = 0;
		return 0;
	}
	--u16maxlen;

	for (; u8 < u8end && u16len < u16maxlen; u8 += u8bytes)
	{
		u8bytes = TCONV_UTF8_LENGTH(u8[0]);

		if (u8bytes + u8 > u8end)
			break;

		if (u8bytes == 1){
			(*u16) = u8[0];
		}
		else if (u8bytes == 2){
			(*u16) = ((u8[0] & 0x1f) >> 2);
			(*u16) <<= 8;
			(*u16) |= ((u8[0] & 0x3) << 6) | (u8[1] & 0x3f);
		}
		else if (u8bytes == 3){
			(*u16) = ((u8[0] & 0xf) << 4) | ((u8[1] & 0x3f) >> 2);
			(*u16) <<= 8;
			(*u16) |= ((u8[1] & 0x3) << 6) | (u8[2] & 0x3f);
		}
		else {
			u8bytes = 1;
			continue;
		}
		// 不处理0,4,5,6,7,8bytes的情况

		++u16, ++u16len;
	}

	*u16 = 0;

	return u16len;
}

