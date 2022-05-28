section .data
    msg: db 'Hello, Infected File', 0xA
    len: equ $ - msg
    newline: db '', 0xA
    filedes: dd 0
    myFileDes: dd 0
    byteToRead: dd 0
    currPos: dd 0
    codeStartMsg: db 'code_start:'
    codeEndMsg: db 'code_end:'
    fileName: db "lab4_start.s"

section .bss
    buff resb 1
    
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
    infection:
        push    ebp             ; Save caller state
        mov     ebp, esp
        pushad
        
        mov     eax, [ebp+8]
        test al, 1
        jnz odd
        mov eax, 4
        mov ebx, 1
        mov ecx, msg
        mov edx, len
        int 0x80
        odd:
        
        popad
        pop ebp
        ret
 
    infector:
        push    ebp             ; Save caller state
        mov     ebp, esp
        pushad        
        
        mov ebx, [ebp+8]     ; get argument - output file name  
        mov eax, 5 ;open output file to write into it (create if neccessary)
        mov ecx, 1025
        ;mov edx, 0x777  
        int 0x80
        mov [filedes], eax ; move file descriptor to filedes     
  
        mov eax, 5 ; open this file to read from it
        mov ebx, fileName
        mov ecx, 0
        ;mov edx, 0777
        int 0x80
        mov [myFileDes], eax 
        
        mov eax, 19 ; lseek in this file
        mov ebx, [myFileDes]
        mov ecx, 0
        sub ecx, 2269
        mov edx, 2
        int 0x80
        
        mov eax, 2269
        mov [byteToRead], eax
        
        mov eax, 0
        mov [currPos], eax ;getting currPos from code_start
        writing: ; write byte to destination file
            mov eax, 3 ; read byte from this file
            mov ebx, [myFileDes]
            ;add ebx, [currPos]
            mov ecx, buff
            mov edx, 1
            int 0x80   

            mov eax, 4 ; write a byte to destination
            mov ebx, [filedes]
            mov ecx, buff
            mov edx, 1
            int 0x80
            
            mov eax, [currPos]
            inc byte eax
            mov [currPos], eax
            dec dword [byteToRead]
            mov eax, [byteToRead]
            cmp eax, 0
            jnz writing
            
            mov eax, 4 ; write a byte to destination
            mov ebx, [filedes]
            mov ecx, newline
            mov edx, 1
            int 0x80
            
            mov eax, 6 ;close file
            mov ebx, [filedes]
            int 0x80        
            mov eax, 6
            mov ebx, [myFileDes]
            int 0x80
        
        
        popad
        pop ebp
        ret
        
code_end:
