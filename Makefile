all:
	gcc -Wall -Wno-unused-function -g bintree/*.c -o bintree/bintreetest
	gcc -Wall -Wno-unused-function -g heap/*.c -o heap/heaptest
	gcc -Wall -Wno-unused-function -g rbtree/*.c -o rbtree/rbtreetest
	gcc -Wall -Wno-unused-function -g smalldicty/*.c -o smalldicty/smalldicty
<<<<<<< HEAD
<<<<<<< HEAD
	gcc -Wall -Wno-unused-function -g utils/memview.c -o utils/memview

=======
>>>>>>> parent of 18647de... memory viewer added
=======
>>>>>>> parent of 18647de... memory viewer added
clean:
	rm -f bintree/bintreetest
	rm -f heap/heaptest
	rm -f rbtree/rbtreetest
	rm -f smalldicty/smalldicty
