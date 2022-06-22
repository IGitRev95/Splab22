compC = gcc -m32 -g -Wall -c -o
complC = gcc -m32 -g -Wall -o

all: myELF

# (target) : (dependencies)
myELF : main_elf.o
	$(complC) myELF main_elf.o

main_elf.o : task1.c 
	$(compC) main_elf.o $^

clean:
	rm -f *.o myELF
