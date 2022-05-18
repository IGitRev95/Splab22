# Readme:
# gcc : gnu c/c++ compiler
# gcc flags:
# 	-m32 : use 32bit libraries
# 	-g : add debug info - no flag no debug
# 	-Wall : warning level - all
# 	-c : no linking (just compile)
# 	-o : optimize
# nasm : assembly compiler

# local vars:
compC = gcc -m32 -g -Wall -c -o
complC = gcc -m32 -g -Wall -o
complS = nasm -g -f elf -w+all -o

.PHONY : clean

all: wordc

# (target) : (dependencies)
wordc : count-words.o
	$(complC) wordc count-words.o

count-words.o : count-words.c 
	$(compC) count-words.o count-words.c

clean:
	rm -f *.o wordc
