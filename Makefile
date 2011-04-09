all:
	gcc -Wall -Wno-unused-function -g *.c -o main
clean:
	rm -f main
