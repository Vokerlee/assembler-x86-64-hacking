DEFAULT REL
extern printf

;==========================================================================
section .text

global main
main:
                xor rax ,rax

                mov rdx, 33
                mov rsi, 6
                mov rdi, msg
                call printf

                xor rax ,rax
                xor rdx ,rdx
                
                ret 

;==========================================================================
section .data

msg:            db `HELLO! Number %d %x\n`, 0



  