#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <android/log.h>

#include "get_functions.h"
//#include "replace_functions.h"


extern "C"
{

void free(
        void *p) {
    my_free(p);
}
void *malloc(
        size_t s) {
    return my_malloc(s);
}
void *calloc(
        size_t size,
        size_t num) {
    return my_calloc(size, num);
}
void *realloc(
        void *p,
        size_t s) {
    return my_realloc(p, s);
}

}
