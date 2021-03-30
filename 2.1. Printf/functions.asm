section .text

;==========================================================================
; Convert decimal number to 2^n base representation
; and print it in stdout
;
; Input parameters: 
;   rbx - number
;   r13 - radix of number (can be 2^n - 2, 4, 8 and 16)
;
; Output parameters:
;   There is no any parameters
;
; Destroyed registers: 
;   r15, r14, r13, rdx, rax, rcx, rbx, rdi
;==========================================================================
base_print:       
                dec r13             ; translate 0000...1000..00 ->  0000...0111..11
                call count_ones     ; r15 = n
                mov r14, 64         ; r14 = 64 - n
                sub r14, r15
                
                mov rcx, 0          ; initialized for analyse every n bit sector
                mov rdi, data       ; the address of line, where the number representation is stored
                mov rdx, rbx        ; save rbx
                
                cmp r15, 3          ; if (base == 8),
                jne loop_base_print ; do the following:

                shr rbx, 63         ; you can see, that for n = 1, 2 and 4 there are integer number of bit sectors:
                mov al, bl          ; for example 64 / 4 = 16 or 64 / 2 = 32 sectors. But (64 mod 3) = 1. 
                mov rbx, dig16      ; So there are 21 bit sectors and 1 the highest bit and we should save it
                xlat                ; in our data (ASCII code) before next loop for other 21 bit sectors
                stosb               ;
                inc rcx             ;
            
loop_base_print:
                mov rbx, rdx        ; restore rbx
                shl rbx, cl         ; take the n high bits
                mov rax, rcx        ; save rcx
                mov rcx, r14        ; rcx = 64 - n
                shr rbx, cl         ; move them to right of word
                mov rcx, rax        ; restore rcx
                
                mov al, bl          ;
                mov rbx, dig16      ; al = dig16[al]
                xlat                ; 
                
                stosb               ; store al (ASCII code) in data
                
                add rcx, r15        ; rcx += n - go to next n bit sector
                cmp rcx, 64         ;
                je .end_loop_printf

                jmp loop_base_print ; repeat again for teh next bit sector

.end_loop_printf:             
                mov rdi, data       ; the address of line, where the number representation is stored
                xor rdx, rdx
                mov rax, 64         ; rax = 64 / n
                div r15
                mov rcx, rax        ; the length of message in data
                add rcx, rdx        ; add the remainder of division (for base = 8 case)
            
                mov al, 0x30        ; ASCII code of '0'
                repe scasb          ; skip '0'
                
                inc cx              ; the length of message in data + 1 (for end symbol 0)
                dec rdi
                mov rsi, rdi        ; buffer

                call putbuffer

                ret
;==========================================================================


;==========================================================================
; Count number of one's
;
; Input parameters: 
;   r13 - given number
;
; Output parameters:
;   r15 - the number of one's
;
; Destroyed registers: 
;   rcx 
;==========================================================================
count_ones:
                mov rcx, 64         ; number of bits in r13 - for loop
                xor r15, r15        ; for answer

count_ones_loop:
                test r13, 1         ; test last bit
                jz .continue
                inc r15
            
.continue:  
                shr r13, 1          ; shift rbx
                loop count_ones_loop
                ret
;==========================================================================


;==========================================================================
; Print decimal number in stdout
;
; Input parameters: 
;   rbx - given number
;
; Output parameters:
;   There is no any parameters
;
; Destroyed registers: 
;   rax, rbx, rdx, rcx, rdi, rsi  
;========================================================================== 
print_dec:    
                mov rdi, data       ; for stosb
                mov rsi, 10         ; for div
                mov rax, rbx
            
next_dnum:
                xor rdx, rdx         
                div rsi             ; rax /= 10
                mov rcx, rax        ; save rax
                    
                mov al, dl
                add al, 0x30        ; 0x30
                stosb
                mov rax, rcx        ; restore rax
                
                or rax, rax
                je .end_loop

                jmp next_dnum
.end_loop:            

                dec rdi
                mov rsi, rdi
.print_loop:
                call putC
                dec rsi   
                cmp rsi, data
                jge .print_loop 
                
                ret
;==========================================================================


;==========================================================================
; Print one symbol in stdout (uses the address)
;
; Input parameters: 
;   rsi - address of the symbol
;
; Output parameters:
;   There is no any parameters
;
; Destroyed registers: 
;   rax, rdi, rdx
;==========================================================================
putC:
                mov rax, 0x01      ; write function
                mov rdi, 1         ; stdout
                mov rdx, 1         ; length
                syscall      
                
                ret
;==========================================================================


;==========================================================================
; Print one symbol in stdout (uses ASCII code)
;
; Input parameters: 
;   cl - symbol
;
; Output parameters:
;   There is no any parameters
;
; Destroyed registers: 
;   rax, rdi, rdx, rsi
;==========================================================================
putc:       
                sub rsp, 100
                mov bl, byte [rsp]
                mov byte [rsp], cl
                mov rsi, rsp

                mov rax, 0x01      ; write function
                mov rdi, 1         ; stdout
                mov rdx, 1         ; length
                syscall
                    
                mov byte [rsp], bl
                add rsp, 100
                
                ret
;==========================================================================


;==========================================================================
; Print n symbols in stdout
;
; Input parameters: 
;   rsi - address of the symbol
;   rcx - number of symbols
;
; Output parameters:
;   There is no any parameters
;
; Destroyed registers: 
;   rax, rdi, rdx
;==========================================================================
putbuffer:
                mov rax, 0x01      ; write
                mov rdi, 1         ; stdout
                mov rdx, rcx       ; length
                syscall
                
                ret
;==========================================================================


;==========================================================================
; Print symbols from address rsi, while not '\0' 
;
; Input parameters: 
;   rsi - address of the string
;
; Output parameters:
;   There is no any parameters
;
; Destroyed registers: 
;   rax, rdi, rdx
;==========================================================================
puts:       
.next_char:
                cmp byte [rsi], byte 0
                je .end_loop
            
                call putC
                inc rsi
                jmp .next_char

.end_loop:                
                ret
;========================================================================== 


;==========================================================================
; Remember addresses of all % labels for jump table
;
; Input parameters: 
;   There is no any parameters
;
; Output parameters:
;   There is no any parameters
;
; Destroyed registers: 
;   rsi, r15, r14
;==========================================================================
fill_jump_table: 
; Filling all massive with addresses of error_handler      
                mov rsi, jump_table
                mov r15, 0          ; counter for cycle

.fill_table:
                cmp r15, n_table
                je .end_loop

                mov r14, error_loop
                mov [rsi + 8 * r15], r14

                inc r15             ; end of loop
                jmp .fill_table
                
.end_loop:
                mov r14, continue_d
                mov r15, 'd' - 'b'
                mov [rsi + 8 * r15], r14

                mov r14, continue_x
                mov r15, 'x' - 'b'
                mov [rsi + 8 * r15], r14

                mov r14, continue_o
                mov r15, 'o' - 'b'
                mov [rsi + 8 * r15], r14

                mov r14, continue_b
                mov r15, 'b' - 'b'
                mov [rsi + 8 * r15], r14

                mov r14, continue_s
                mov r15, 's' - 'b'
                mov [rsi + 8 * r15], r14

                mov r14, continue_c
                mov r15, 'c' - 'b'
                mov [rsi + 8 * r15], r14

                ret
;==========================================================================

DEFAULT REL
section .data
printf_test:    db "I %d %s %x %d%% %c %b", 0

end_test:       db "-----------------------------------------------------------------------------", 0
line:           db "Function works steadily (test)", 0
love:           db "love", 0

space:          db " " 
dig16:          db '0123456789ABCDEF'
error_message:  db 0xA, "Undefined symbol", 0xA
escape:         db 0xA, 9, 9, 0xB, 0x5C, '\'

jump_table:     db 8 * n_table dup (0)
n_table         equ 23              ; the amount of elements in jump_table

data:           dq 0                ; temp storage for numbers (to print them in stdout)
