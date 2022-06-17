# Lab-08 - Task0a Answers
1.  Q: Where is the entry point specified, and what is its value?
    1.  A: The entry point is specified at the ELF header and it's values:
        -   ELFexec: `0x8048294`
        -   ELFexec2short: `0x8048080`
2.  Q: What fields inside the file header differ? what does this mean?
    1.  A: Just the entry point value. means that the infection were added after compiling.
3.  Q: Where in the file does the code of function "main" start?
    1.  A: at '_start' `0x8048080`
4.  Q: Which file is infected?
    1.  A: `ELFexec` is the infected because the entry point of the file by convention is the `_start` function and in this file the entry point value is not the address of `_start` according to the elf header.