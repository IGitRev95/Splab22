# Lab-08 - Task0a Answers
1. Q: Where is the entry point specified, and what is its value?
   1.  A: The entry point is specified at the ELF header and it's value of (for a.out file) is: `0x80482E0`
2.  Q: How many sections are there in a.out?
    1.  A: In `a.out` there are 32 (34 in total) sections (according to section headers table).
3.  Q: What is the size of the .text section?
    1.  A: The size of section .text is: `0x1B8` or `440` Bytes.
4.  Q: Does the symbol `_start` occur in the file? If so, where is it mapped to in virtual memory?
    1.  A: According to the symbol table `_start` is appearing the file and it is maped to `0x080482E0` which is also the file entry point.
5.  Q: Does the symbol main occur in the file? If so, where is it mapped to in virtual memory?
    1.  A: According to the symbol table `main` is appearing the file and it is maped to `0x08048388` .
6.  Q: Where in the file does the code of function "main" start?
    1.  A: Not Sure