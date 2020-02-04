
#include <stdlib.h>
#include <android/log.h>
#include <dlfcn.h>
#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <atomic>
#include <sys/prctl.h>

#include "get_functions.h"

void my_free(
        void *p) {
//    showStacks(MEM_ACTION_FREE, NULL, p, 0, 0);
    free(p);
}

void *my_malloc(
        size_t s) {
    void *ret = malloc(s);
//    void *ret =nullptr;
//    showStacks(MEM_ACTION_MALLOC, ret, NULL, s, 1);
    return ret;
}

void *my_calloc(
        size_t size,
        size_t num) {
    void *ret = calloc(size, num);
//    showStacks(MEM_ACTION_CALLOC, ret, NULL, size, num);
    return ret;
}

void *my_realloc(
        void *p,
        size_t s) {
    void *ret = realloc(p, s);
//    showStacks(MEM_ACTION_REALLOC, ret, p, s, 1);
    return ret;
}