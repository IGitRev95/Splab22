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

all: mypipeline

# (target) : (dependencies)
mypipeline : mypipeline.o  LineParser.o # looper.o
#	@echo "making exe"
	$(complC) mypipeline $^

mypipeline.o : mypipeline.c 
#	@echo "making mainC.o"
	$(compC) mypmypipelineipe.o $^

LineParser.o : LineParser.c
#	@echo "making mainC.o"
	$(compC) LineParser.o $^

#looper.o : looper.c 
#	@echo "making mainC.o"
#	$(compC) looper.o $^

clean:
	rm -f *.o mypipeline
