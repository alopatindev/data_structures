all:
	gcc -g queue.c bintree.c main.c -o main
clean:
	rm -f main
