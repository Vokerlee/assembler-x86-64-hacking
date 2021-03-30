.model tiny
.286
.code
org 100h  

; Warning: there is no functions in this program. That is why there is no doccumentations to functions.
; Warning #2: the program is hard to understand.
; THIS PROGRAM IS HARD TO HACK. IF YOU WANT TO CHILL OUT, SEE THE OTHER FILE.
; Almost all moments in program are add - it is normal, because this program is to be hacked.

Start:
                jmp Start2

check_password:
                pop ax
                pop ax
                mov bx, offset pass

Next1:
                mov dl, byte ptr [bx]
		        cmp dl, '$'
                je sup1
                cmp al, dl
                jne end1
                inc bx

                mov dl, byte ptr [bx]
                cmp ah, dl
                jne end1
                pop ax
                inc bx
                jmp next1

end1:
                call fail_print
sup1:
                call success_print

		        ret

success_print:
                mov bx, offset okey
                mov ah, 02h

next4:
                mov dl, [bx]
                cmp dl, '$'
                je end4
                int 21h
                inc bx
                jmp next4
end4:
                call exit
		        ret

fail_print:
                mov bx, offset lox
                mov ah, 02h
next3:
                mov dl, [bx]
                cmp dl, '$'
                je end3
                int 21h
                inc bx
                jmp next3

end3:
                call exit
		        ret

hello_print:
                mov bx, offset hello
                mov ah, 02h

next5:
                mov dl, [bx]
                cmp dl, '$'
                je end5
                int 21h
                inc bx
                jmp next5

end5:
                ret

read_msg:
                pop di
		        mov cx, 0       ; low

next2:
                mov ah, 01h
                int 21h

                cmp al, 0Dh
                je end2

                cmp cx, 0
                jne low_sit

                mov dh, al
                push dx
                inc cx
                jmp high_sit	

low_sit:	
                pop dx
                mov dl, al
                push dx
                dec cx

high_sit:	
		        jmp next2

end2:
                push di
		        ret

exit:		
                mov ax, 4c00h
                int 21h

                           
pass:           db 'drowssap', '$$'
hello:          db 0ah, 'Enter the pass, loser:', 0ah, '$'
okey:           db 'OK, you are successfull this time', '$'
lox:            db 'Lol, I was sure you are really loser', '$'

Start2:
                call hello_print
                call read_msg
                call check_password
                call exit
      
end             Start
	

