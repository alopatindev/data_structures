#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#define LINKED_LIST(TYPE, NAME, STRUCTNAME) \
typedef struct STRUCTNAME \
{ \
    TYPE data; \
    struct STRUCTNAME *next; \
} NAME;

#endif
