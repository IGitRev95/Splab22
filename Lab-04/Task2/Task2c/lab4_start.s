section	.rodata			; we define (global) read-only variables in .rodata section
	output_string: db "Hello, Infected File"
    output_string_len: equ $ - output_string

section .bss
    write_address: resb 4  ; counter of write_address from code_start

section .text
global _start
global system_call
global infection
global infector
extern main
_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes (number of arguments of 4 bytes each)
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop
        
system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller


code_start:
    nop

infection:
    push    ebp             ; Save caller state
    mov     ebp, esp
    pushad                  ; Save some more caller state
    mov eax, [ebp+8]
    shr eax, 1
    jc .finish
    mov eax, 4
    mov ebx, 1
    mov ecx, output_string
    mov edx, output_string_len
    int 0x80
    .finish:
        popad
        pop ebp
        ret

infector:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack - opend file descibtor
    pushad                  ; Save some more caller state
    .open:
        mov ebx, [ebp+8]       ; get path to reg
        mov ecx, 0x441           ; push open file flags
        mov eax, 5             ; push sys_call op_code
        int 0x80                ; open
    
    mov [ebp-4], eax        ; get fd returned value
    ;mov dword [ebp-4], 0x01        ; get fd returned value
    mov dword [write_address], code_start         ; setting write_address to 0
    
    .write:
        mov eax, 4
        mov ebx, [ebp-4]
        mov ecx, [write_address]
        mov edx, 0x01
        int 0x80

        inc byte [write_address]

        cmp dword [write_address], code_end
        jne .write
        
    
    .close:
        mov eax, 5
        mov ebx, [ebp-4]
        int 0x80

    .finish:
        popad
        add     esp, 4          ; Restore caller state
        pop ebp
        ret


code_end:
    nop

; writing & closing not working