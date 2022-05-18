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

all: menu_map

# (target) : (dependencies)
menu_map : menu_map.o
#	@echo "making exe"
	$(complC) menu_map menu_map.o

menu_map.o : menu_map.c 
#	@echo "making mainC.o"
	$(compC) menu_map.o menu_map.c

clean:
	rm -f *.o menu_map
