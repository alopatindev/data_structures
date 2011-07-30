#include <stdio.h>

typedef unsigned char u8;

void printByte(u8 num)  // shows a byte in hexadecimal and binary numeral system
{
    u8 i, x;

    for (i = 0; i < 8; ++i) {
        x = num;
        x <<= i;
        x >>= 7;
        printf("%d", (int)x);
    }

    char buf[2];
    sprintf(buf, "%2x", num);
    if (buf[0] == ' ')
        buf[0] = '0';
    printf("(%s) ", buf);
}

void printMemory(void *v, size_t size)
{
    printf("%ld bytes of memory from %#lx address:\n",
           (long)size,
           (unsigned long)v);

    size_t i;
    for (i = 0; i < size; ++i)
        printByte(((u8 *)v)[i]);

    printf("\n\n");
}

// function example
int testFunc(int i);
void spike();  // we need this hack to calculate the size of
               // the function testFunc at run time

int testFunc(int i)
{
    ++i;
    return ++i;
}

void spike() {}

int main()
{
    int a = -6;
    printMemory(&a, sizeof(a));

    // objdump -d memview | less -p testFunc
    printMemory((void *) &testFunc, (size_t)spike - (size_t)testFunc);
    return 0;
}
