shell: shell.o
	gcc -o shell shell.o
	
shell.o: shell.c shell.h
	gcc -c shell.c
	
clean:
	rm -f *.o shell
