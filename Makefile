all:
	gcc -Wall -Wno-unused-function -g bintree/*.c -o bintree/bintreetest
	gcc -Wall -Wno-unused-function -g heap/*.c -o heap/heaptest
	gcc -Wall -Wno-unused-function -g rbtree/*.c -o rbtree/rbtreetest
clean:
	rm -f bintree/bintreetest
	rm -f heap/heaptest
	rm -f rbtree/rbtreetest
