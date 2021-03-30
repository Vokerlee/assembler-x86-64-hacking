.model tiny
.code
org 100h

number		= 123eh
base_10		= 10
base_2		= 2
base_16		= 16

enter_n		= 20h
letter_a	= 41h

start:			
            mov ah, 09h
            mov dx, offset msg_dec
            int 21h
            
            mov ax, number		; the number
            call dec_transfer
            
            mov ah, 09h
            mov dx, offset msg_bin
            int 21h
            
            mov ax, number		; the number
            call bin_transfer
            
            mov ah, 09h
            mov dx, offset msg_hex
            int 21h
            
            mov ax, number		; the number
            call hex_transfer
            
            mov ax, 4c00h
            int 21h
            
;//////////////////////////////////////////////////////////////////////////
; Decimal numbers transfer
            
dec_transfer:	
            mov bx, base_10		; the base of translating
            xor cx, cx
            
split_piece_10:			
            xor dx, dx
            div bx
            
            push dx
            inc cx
            
            test ax, ax
            jnz split_piece_10
            
; Print on the screen			
            mov ah, 02h
            
print_number_10:
            pop dx
            add dl, '0'
            
            int 21h 
            
            loop print_number_10
            
            mov dl, enter_n
            int 21h
            
            ret
; The end
;//////////////////////////////////////////////////////////////////////////
; Binary numbers transfer
            
bin_transfer:
            mov bx, base_2		; the base of translating
            xor cx, cx
            
split_piece_2:			
            xor dx, dx
            div bx
            
            push dx
            inc cx
            
            test ax, ax
            jnz split_piece_2
            
; Print on the screen			
            mov ah, 02h
            
print_number_2:
            pop dx
            add dl, '0'
            
            int 21h 
            
            loop print_number_2
            
            mov dl, enter_n
            int 21h
            
            ret
; The end
;//////////////////////////////////////////////////////////////////////////
; Hex numbers transfer
            
hex_transfer:
            mov bx, base_16		; the base of translating
            xor cx, cx
            
split_piece_16:			
            xor dx, dx
            div bx
            
            push dx
            inc cx
            
            test ax, ax
            jnz split_piece_16 
            
; Print on the screen			
            mov ah, 02h
            
print_number_16:
            pop dx
            cmp dx, 9
            jae letter_hex
        
            add dl, '0'
            jmp out_number

letter_hex:
            sub dx, 10
            add dx, letter_a
            
out_number:
            int 21h
            
            loop print_number_16
            
            mov dl, enter_n
            int 21h
            
            ret
; The end
;//////////////////////////////////////////////////////////////////////////
            
.data
msg_hex			db "Hex: $"
msg_bin			db "Bin: $"
msg_dec			db "Dec: $"

end start


