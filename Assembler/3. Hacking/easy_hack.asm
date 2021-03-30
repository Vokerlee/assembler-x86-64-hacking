.model tiny
.286
.code
org 100h

; THIS PROGRAM IS TOO EASY HACKING. IF YOU WANT TO KILL YOURSELF, SEE THE OTHER FILE.
; Some moments in program are add - it is normal, because this program is to be hacked.

;==========================================================================
Start:		
                mov ax, 01234h
                push ax
                mov bx, 05678h
                push bx

                call hello_print
                call read_msg

                call check_password

end_program:
                pop ax
                pop bx
                mov ax, 4c00h
                int 21h
;==========================================================================


;==========================================================================
; Print hello message in console
;
; Input parameters: 
;   There is no any parameters
;
; Output parameters:
;   There is no any parameters
;
; Destroyed registers: 
;   bx, ax, dx
;==========================================================================
hello_print:
                mov bx, offset hello_msg
                mov ah, 02h
hello_loop:
                mov dl, [bx]

                cmp dl, '$'
                je hello_end

                int 21h
                inc bx
                jmp hello_loop

hello_end:
                ret
;==========================================================================


;==========================================================================
; Read password from console in template massive msg_storage
;
; Input parameters: 
;   There is no any parameters
;
; Output parameters:
;   There is no any parameters
;
; Destroyed registers: 
;   bx, ax
;==========================================================================
read_msg:
                mov bx, offset msg_storage

read_loop:		
                mov ah, 01h
                int 21h

                cmp al, 0Dh             ; when enter was on
                je read_end

                mov [bx], al            ; remember input parameter al in msg_storage
                cmp byte ptr [bx + 1], 024h
                je read_end             ; if end of password (8 symbols - maximum)

                inc bx
                jmp read_loop
read_end:	
                ret
;==========================================================================


;==========================================================================
; Print failure message in console
;
; Input parameters: 
;   bx - the address of message
;
; Output parameters:
;   There is no any parameters
;
; Destroyed registers: 
;   bx, ax, dx
;==========================================================================
msg_print:
                mov ah, 02h

msg_loop:
                mov dl, [bx]

                cmp dl, '$'
                je msg_end

                int 21h
                inc bx
                jmp msg_loop

msg_end:
                ret
;==========================================================================


;==========================================================================
; Checks the message-password to be valid and print appropriate message
;
; Input parameters: 
;   There is no any parameters
;
; Output parameters:
;   There is no any parameters
;
; Destroyed registers: 
;   bx, ax, dx
;==========================================================================
check_password:
                mov bx, offset msg_storage
                mov ah, 02h

                mov dl, 0ah

                int 21h

                mov dl, [bx]
                cmp dl, 'b'
                jne fail
                mov dl, [bx + 1]
                cmp dl, 'a'
                jne fail
                mov dl, [bx + 2]
                cmp dl, 'i'
                jne fail
                mov dl, [bx + 3]
                cmp dl, 't'
                jne fail
                mov dl, [bx + 4]
                cmp dl, 'i'
                jne fail
                mov dl, [bx + 5]
                cmp dl, 'i'
                jne fail
                mov dl, [bx + 6]
                cmp dl, 'm'
                jne fail
                mov dl, [bx + 7]
                cmp dl, '!'
                jne fail

                mov bx, offset success_msg
                jmp check_end

fail:  
                mov bx, offset fail_msg
check_end:  
                call msg_print
                ret
;==========================================================================

;==========================================================================
.data

hello_msg       db 0ah, 'Enter the pass, loser:', 0ah, '$'
success_msg:    db 'OK, you are successfull this time', '$'
fail_msg:       db 'Lol, I was sure you are really loser', '$'
msg_storage:    db '00000000', '$'
end             Start                                      	

