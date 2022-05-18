.PHONY : clean

all: task2b

# (target) : (dependencies)
task2b : start.o util.o main.o
#	@echo "making exe"
	ld -m elf_i386 start.o main.o util.o -o task2b

start.o : lab4_start.s # util.l
#	@echo "making mainC.o"
	nasm -f elf32 lab4_start.s -o start.o

util.o : lab4_util.c # util.l
#	@echo "making mainC.o"
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector lab4_util.c -o util.o

main.o : task2b.c 
#	@echo "making mainC.o"
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task2b.c -o main.o

clean:
	rm -f *.o task2b out
