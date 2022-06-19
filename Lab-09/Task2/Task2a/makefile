file_name = task2a
bin_file = ELFexecTestFile

all: $(file_name)
$(file_name): $(file_name).o
	ld -m elf_i386 -o $(file_name) $^
$(file_name).o: $(file_name).s
	nasm -f elf32 -o $(file_name).o $^

.PHONY: clean bindis
clean: 
	rm -f *.o $(file_name)

bindis:
	ndisasm -b 32 $(bin_file) > $(bin_file).asm