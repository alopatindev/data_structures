all:
	gcc -Wall -Wno-unused-function -g bintree/*.c -o bintree/bintreetest
clean:
	rm -f bintree/bintreetest
