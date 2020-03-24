#ifndef __TCONV_GBK2UTF8_H_1392875R9863
#define __TCONV_GBK2UTF8_H_1392875R9863

#ifdef __cplusplus
extern "C"{
#endif
#include <stdint.h>
#define TCONV_IS_GBK_CHAR(c1, c2) \
	  ((unsigned char)c1 >= 0x81 \
	&& (unsigned char)c1 <= 0xfe \
	&& (unsigned char)c2 >= 0x40 \
	&& (unsigned char)c2 <= 0xfe \
	&& (unsigned char)c2 != 0x7f) 

#define TCONV_UTF8_LENGTH(c) \
	((unsigned char)c <= 0x7f ? 1 : \
	((unsigned char)c & 0xe0) == 0xc0 ? 2: \
	((unsigned char)c & 0xf0) == 0xe0 ? 3: \
	((unsigned char)c & 0xf8) == 0xf0 ? 4: 0)

extern int tconv_utf82gbk(const char * in,
					size_t ilen,
					char * out,
					size_t * olen);

extern int tconv_gbk2utf8(const char * in,
					size_t ilen,
					char * out,
					size_t * olen);

extern int tconv_utf16to8(const uint16_t * u16,
					size_t u16len,
					unsigned char * u8,
					size_t u8maxlen);

extern int tconv_utf8to16(const unsigned char * u8,
					size_t u8len,
					uint16_t * u16,
					size_t u16maxlen);




extern int tconv_single_utf16to8(uint16_t u16, unsigned char * u8);
extern int tconv_single_utf8to16(const unsigned char * u8, uint16_t * u16);


#ifdef __cplusplus
}
#endif

#endif
