make: shell.c
	gcc util.c shell.c -o shell.out

run: make
	./shell.out

clean:
	rm -rf *~
	rm -rf *.out
	rm -rf *.o
