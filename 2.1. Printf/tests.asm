%include "functions.asm"
%include "printf.asm"

; This file just only tests all funtions (see functions file)
; If someething is unclear, please, open files "functions.asm" and "printf.asm" (there you can find documentation to all functions)

section .text

global _start
_start:

;====================================================     
; Binary numbers test
;==================================================== 
; Print binary 1100_1100_0100_0110   
                mov rbx, 0b1100_1100_0100_0110
                mov r13, 2
                call base_print
                mov rsi, space      
                call putC

                call go_to_next_test   

;====================================================
; Octal numbers test
;====================================================
; Print octal 0o00123456776              
                mov rbx, 0o00123456776
                mov r13, 8
                call base_print
                mov rsi, space
                call putC

                call go_to_next_test    

;====================================================          
; Hexadecimal numbers test
;====================================================
; Print hexadecimal 0x0123eea789acdef           
                mov rbx, 0x0123456789ACDEF
                mov r13, 16
                call base_print
                mov rsi, space
                call putC

                call go_to_next_test

;==================================================== 
; Decimal numbers test
;====================================================
; Print decimal 0x4453 = 17491           
                mov rbx, 0x4453
                call print_dec
                mov rsi, space
                call putC

                call go_to_next_test 

;====================================================                     
; Puts function test
;====================================================
; Print the text in variable "line"
                mov rsi, line
                call puts
                mov rsi, space
                call putC

                call go_to_next_test  

;====================================================
; Printf function test
;====================================================
; Test #1:
                push 127
                push '!'
                push 100
                push 3802
                push love
                push 22
                push printf_test    ; format line
                call printformat
                pop r10
                pop r10
                pop r10
                pop r10
                pop r10
                pop r10
                pop r10
                
                call go_to_next_test  

                mov rax, 0x3C      ; exit
                xor rdi, rdi
                syscall       

;====================================================
go_to_next_test:
; The end of the test (texting enter and beautiful line)
                mov rsi, escape
                call putC
                
                push end_test
                call printformat
                pop r10

                mov rsi, escape
                call putC

                ret
;====================================================