compC = gcc -m32 -g -Wall -c -o
complC = gcc -m32 -g -Wall -o

all: printMain

# (target) : (dependencies)
printMain : main_elf.o
	$(complC) printMain main_elf.o

main_elf.o : task3a.c 
	$(compC) main_elf.o $^

clean:
	rm -f *.o printMain
