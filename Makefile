all:
	gcc -Wall -Wno-unused-function -g bintree/*.c -o bintree/bintreetest
	gcc -Wall -Wno-unused-function -g heap/*.c -o heap/heaptest
	gcc -Wall -Wno-unused-function -g rbtree/*.c -o rbtree/rbtreetest
	gcc -Wall -Wno-unused-function -g smalldicty/*.c -o smalldicty/smalldicty
	gcc -Wall -Wno-unused-function -g utils/memview.c -o utils/memview
	clang -Wall -Wextra -pedantic -Wno-unused-function -std=c11 -O0 -g avltree/*.c -o avltree/avltree

clean:
	rm -f bintree/bintreetest
	rm -f heap/heaptest
	rm -f rbtree/rbtreetest
	rm -f smalldicty/smalldicty
	rm -f utils/memview
	rm -f avltree/avltree
