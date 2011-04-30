#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#define LINKED_LIST(NAME, TYPE) \
struct NAME \
{ \
    struct NAME *next; \
    TYPE data; \
};

#endif
