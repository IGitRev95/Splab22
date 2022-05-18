# Readme:
# gcc : gnu c/c++ compiler
# gcc flags:
# 	-m32 : use 32bit libraries
# 	-g : add debug info - no flag no debug
# 	-Wall : warning level - all
# 	-c : no linking (just compile)
# 	-o : optimize
# nasm : assembly compiler

# macros:
compC = gcc -m32 -g -Wall -c -o
complC = gcc -m32 -g -Wall -o
complS = nasm -g -f elf -w+all -o

.PHONY : clean

all: lab3_mergesort.out

# (target) : (dependencies)
lab3_mergesort.out : lab3_mergesort.o
#	@echo "making exe"
	$(complC) lab3_mergesort.out lab3_mergesort.o

lab3_mergesort.o : lab3_mergesort.c 
#	@echo "making mainC.o"
	$(compC) lab3_mergesort.o lab3_mergesort.c

clean:
	rm -f *.o lab3_mergesort.out
