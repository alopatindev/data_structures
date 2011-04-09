#ifndef PAIR_H
#define PAIR_H

#include <stdlib.h>
#include <stdio.h>
//#include "../bintree.h"

#define PAIR(TYPE1, TYPE2, NAME) \
typedef struct \
{ \
    TYPE1 first; \
    TYPE2 second; \
} NAME; \
\
static NAME *createPair(TYPE1 first, TYPE2 second) \
{ \
    NAME *p = (NAME *)malloc(sizeof(NAME)); \
    if (p == NULL) { \
        fprintf(stderr, "Failed to allocate memory for a pair\n"); \
        return NULL; \
    } \
    p->first = first; \
    p->second = second; \
    return p; \
}

#endif
