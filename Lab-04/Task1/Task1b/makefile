.PHONY : clean

all: count

# (target) : (dependencies)
count : start.o util.o main.o
#	@echo "making exe"
	ld -m elf_i386 start.o main.o util.o -o count

start.o : lab4_start.s # util.l
#	@echo "making mainC.o"
	nasm -f elf32 lab4_start.s -o start.o

util.o : lab4_util.c # util.l
#	@echo "making mainC.o"
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector lab4_util.c -o util.o

main.o : lab4_main.c 
#	@echo "making mainC.o"
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector lab4_main.c -o main.o

clean:
	rm -f *.o count out
