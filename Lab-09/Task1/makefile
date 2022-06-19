file_name = task1a

all: $(file_name)
$(file_name): $(file_name).o
	ld -m elf_i386 -o $(file_name) $^
$(file_name).o: skeleton.s
	nasm -f elf32 -o $(file_name).o $^

.PHONY: clean
clean: 
	rm -f *.o $(file_name)

bindis:
	ndisasm -b 32 bin_file > bin_file.asm