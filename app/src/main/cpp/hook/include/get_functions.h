#ifndef __GET_FUNCTIONS_H__
#define __GET_FUNCTIONS_H__

#include <jni.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C"
{
#endif

void my_free(
        void* p);
void* my_malloc(
        size_t s);
void* my_calloc(
        size_t size,
        size_t num);
void* my_realloc(
        void* p,
        size_t s);

#ifdef __cplusplus
}
#endif

#endif