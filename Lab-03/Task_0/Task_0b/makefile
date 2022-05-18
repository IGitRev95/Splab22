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

all: hexaPrint

# (target) : (dependencies)
hexaPrint : hexaPrint.o
#	@echo "making exe"
	$(complC) hexaPrint hexaPrint.o

hexaPrint.o : hexaPrint.c 
#	@echo "making mainC.o"
	$(compC) hexaPrint.o hexaPrint.c

clean:
	rm -f *.o hexaPrint
