compC = gcc -m32 -g -Wall -c -o
compS = nasm -g -f elf -w+all -o

.PHONY : all clean

all: task0

task0 : main.o numbers.o add.o
#	@echo "making adder.exe"
	gcc -m32 -g -Wall -o adder main.o numbers.o add.o

main.o : main.c 
#	@echo "making main.o"
	$(compC) main.o main.c

numbers.o : numbers.c
#	@echo "making numbers.o"
	$(compC) numbers.o numbers.c

add.o : add.s
#	# @echo "making add.o"
	$(compS) add.o add.s


clean:
	rm *.o adder
