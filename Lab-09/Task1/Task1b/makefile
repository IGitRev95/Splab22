file_name = task1b

all: $(file_name)
$(file_name): $(file_name).o
	ld -m elf_i386 -o $(file_name) $^
$(file_name).o: $(file_name).s
	nasm -f elf32 -o $(file_name).o $^

.PHONY: clean
clean: 
	rm -f *.o $(file_name)
