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

all: addresses

# (target) : (dependencies)
addresses : addresses.o
#	@echo "making exe"
	$(complC) addresses addresses.o

addresses.o : addresses.c 
#	@echo "making mainC.o"
	$(compC) addresses.o addresses.c

clean:
	rm -f *.o addresses
