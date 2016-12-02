make: shell.c
	gcc -c -g util.c
	gcc -c -g shell.c
	gcc -g util.o shell.o -o shell.out

run: make
	./shell.out

clean:
	rm -rf *~
	rm -rf *.out
