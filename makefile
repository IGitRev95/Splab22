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

nasmDis =  ndisasm -b 32

.PHONY : clean, bindis

all: exeFile

# (target) : (dependencies)
exeFile : mainC.o asm.o
#	@echo "making exe"
	$(complC) exeFile mainC.o asm.o

mainC.o : mainC.c 
#	@echo "making mainC.o"
	$(compC) mainC.o mainC.c

asm.o : asm.s
#	@echo "making asm.o"
	$(complS) asm.o asm.s

bindis:
	$(nasmDis) bin_file > bin_file.asm

clean:
	rm -f *.o exeFile
