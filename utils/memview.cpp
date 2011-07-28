#include <stack>
#include <iostream>
#include <cstdio>

using namespace std;

typedef unsigned char u8;

void printByte(u8 num)  // shows a byte in hexadecimal and binary numeral system
{
    stack<u8> st;
    printf("(%2x)", num);

    while (num >= 2) {
        st.push(num % 2);
        num /= 2;
    }
    st.push(num);

    for (size_t i = 0; i < 8-st.size(); ++i)
        cout << 0;

    while (!st.empty()) {
        cout << (int)st.top();
        st.pop();
    }

    cout << ' ';
}

void printMemory(void *v, size_t size)
{
    for (size_t i = 0; i < size; ++i)
        printByte(((u8 *)v)[i]);
}

/*int testFunc(int i);
void spike();  // we need this hack to calculate the size of
               // the function testFunc at run time

int testFunc(int i)
{
    ++i;
    return ++i;
}

void spike() {}*/

int main()
{
    int a = -6;
    printMemory(&a, sizeof(a));
    //printMemory((void *) &testFunc, (size_t)spike - (size_t)testFunc);
    return 0;
}
