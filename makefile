make: shell.c
	gcc shell.c -o shell.out

run: make
	./shell.out

clean:
	rm -rf *~
	rm -rf *.out
