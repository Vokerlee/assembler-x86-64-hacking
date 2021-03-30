DEFAULT REL
%include "functions.asm"
; The only function in this file is printf function.

section .text

;==========================================================================
; Do printf function
;
; Input parameters: 
;   Format string on the top of the stack and
;   arguments for format string
;
; Output parameters:
;   There is no any parameters
;
; Destroyed registers: 
;   rax, rbx, rdx, rcx, rdi, rsi, r15, r14, r12, r11, r10, r9
;==========================================================================
global printformat

printformat:             
                pop r14             ; r14 = the address of command, where we must arrive after function

                push r9             ; Default parameter passing is implemeted by registers (from rdi to r9),     
                push r8             ; according to the agreement. Then, if the number of parametres is more than 6, 
                push rcx            ; everything is pushed in stack. So it is a good idea to push all parameters 
                push rdx            ; in stack and work with stack. At the end of programm we should shift rsp back 
                push rsi            ; by 48 (6 registers, 8 bytes). rsp - the pointer to the top of stack.
                push rdi

                mov r9, r14         ; r9 = r14 - return address (is stored there all the time of function)

; Write all labels in jump-table
                call fill_jump_table

                mov rsi, [rsp]      ; the format string
                mov r10, 1          ; the offset to stack-pointer-rigister rsp
            
printf_loop:
                lodsb               ; rax = new char
                or rax, rax         ; if end of string (ASCII code 0), go to the end of function
                je end_printf
                
                cmp rax, '%'        ; if format specifier, go next
                je format_specifier

                cmp rax, '\'        ; if escape sequance, go next
                je escape_symbol

                dec rsi             ; if symbol, just print it and loop again
                call putC
                inc rsi
                jmp printf_loop

format_specifier:
                lodsb               ; read format_specifier in rax

; Comparing with %%:
                cmp rax, '%'        ; % symbol
                jne jump_to_table_func
                dec rsi             ; for putС
                call putC           ; putC
                inc rsi             ; for putС
                jmp printf_loop

jump_to_table_func:
; Prepare register to jump with jump-table

                mov rbx, [rsp + 8 * r10]
                inc r10

                mov rcx, rax

; Error handler (if < 'b' or > 'x'):
                cmp rcx, 'b'
                jb error_loop

                cmp rcx, 'x'
                ja error_loop

; Otherwise use jump table
                sub rcx, 'b'        ; rbx = number in jump table
                mov r11, jump_table

                jmp [r11 + 8 * rcx]

; Beginning of comparing with jump-table

continue_d:
                mov r12, rsi        ; for decimal number
                call print_dec
                mov rsi, r12
                jmp printf_loop

continue_o:  
                mov r12, rsi        ; for octal number
                mov r13, 8
                call base_print   
                mov rsi, r12      
                jmp printf_loop

continue_x:  
                mov r12, rsi        ; for hexadecimal number
                mov r13, 16
                call base_print
                mov rsi, r12
                jmp printf_loop

continue_b:  
                mov r12, rsi        ; for binery number
                mov r13, 2
                call base_print
                mov rsi, r12
                jmp printf_loop

continue_s:  
                mov r12, rsi
                mov rsi, rbx        ; for puts
                call puts
                mov rsi, r12
                jmp printf_loop

continue_C:  
                mov r12, rsi
                mov rsi, rbx        ; for putC
                call putC
                mov rsi, r12
                jmp printf_loop

continue_c: 
                mov r12, rsi
                mov cl, bl          ; for putc
                call putc
                mov rsi, r12
                jmp printf_loop

error_loop:
; Error message (if % is unknown)
                mov rsi, error_message
                jmp end_printf


%macro  PRINT_ESCAPE 1              ; macros with 1 parameter - print escape and continue

                mov rsi, escape + %1; print the appropriate symbol in escape-massive of special symbols (ASCII-cdoes)
                call putc           ; print escape
                mov rsi, r15        ; restore rsi
                jmp printf_loop

%endmacro

escape_symbol:
                lodsb               ; read escape symbol
                mov r15, rsi        ; save rsi

continue_n:    
                cmp rax, 'n'        ; new_line
                jne continue_t
                PRINT_ESCAPE(0)

continue_t:  
                cmp rax, 't'        ; tab
                jne continue_v
                PRINT_ESCAPE(1)

continue_v:
                cmp rax, 'v'        ; vertical tab
                jne continue_sl
                PRINT_ESCAPE(3)
            
continue_sl:  
                cmp rax, '\'        ; \ symbol
                jne error_loop
                PRINT_ESCAPE(4)          

end_printf:
                add rsp, 8 * 6
                push r9             ; arrive to old function (r9 - its address)
                ret 
