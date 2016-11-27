make: shell.c
	gcc shell.c -g -o shell.out

run: make
	./shell.out

clean:
	rm -rf *~
	rm -rf *.out
