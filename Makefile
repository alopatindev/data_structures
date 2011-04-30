all:
	gcc -Wall -Wno-unused-function -g -Iextra bintreetest.c bintree.c stack.c queue.c -o bintreetest
clean:
	rm -f bintreetest
