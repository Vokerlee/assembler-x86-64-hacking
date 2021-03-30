; The program is written by Vokerlee as Ded's mission

.model tiny
.code
org 100h

;==========================================================================
; All necessary ASCII codes and other constants:

background      = 4400h                 ; the symbol of loading
string_color    = 01001110b             ; the colour of text in videomemory

left_upper      = 0ac9h                 ; left  upper angle
right_upper     = 0abbh                 ; right upper angle

right_down      = 0abch                 ; right down angle
left_down       = 0ac8h                 ; left  down angle

horizontal      = 0acdh                 ; horizontal line
vertical        = 0abah                 ; vertical   line

letter_a        = 61h                   ; ASCII code of letter 'a'
letter_h        = 68h                   ; ASCII code of letter 'h' (for hex numbers)

; The address of videomemory
videomemory     = 0b800h                ; the address of videomemory

; The hex representaton of number in memory
base_hex        = 16                    ; base of hexidecimal notation

; The coordinates of the frame:

x1              = 50
x2              = 79
y1              = 1
y2              = 11
delx            equ x2 - x1
dely            equ y2 - y1
;==========================================================================

;==========================================================================
start:
; Preparing for for determination of interrupt

                call intercept_09int    ; implementation of interception of keyboard interrupt

;==========================================================================
; NEW INTERRUPT (establish new interrupt of keyboard)
reg_hander      proc
                push di es dx cx bx ax  ; remember necessary registers
                
; Cheching for keystroke or key release 's'
                mov ah, 4eh             ; set text-colour
                in al, 60h
                
                cmp al, 1fh             ; the scan-code of keystroke the letter 's'
                jne key_release

; Preparing the videomemory to be used (es:[di])
                mov di, videomemory
                mov es, di
                mov di, (78 - delx) * 2
                
; Remembering the current state of videomemory
                call remember_old_videomem

; Preparing the parametres to paint the frame
                mov ax, background
                mov cx, delx                ; Width
                mov bx, dely                ; Height
                mov di, (y1 * 80 + x1) * 2	; the address of the first "pixel"
                
; Frame cleaning:
                call clean_frame

; Painting frame wrapper with registers:
                call paint_frame_wrapper
                call print_registers
                jmp to_old_handler
    
key_release:
                cmp al, 9fh             ; the scan-code of key release the letter 's'
                jne to_old_handler      ; call the old handler 09h to finish the interrupt
                
; Establishment of olf videomemory after relese of button-number 's'
                mov di, videomemory
                mov es, di
                mov di, (78 - delx) * 2
                call establish_old_videomem				

; The end of own interrupt				
to_old_handler:	
                pop ax bx cx dx es di
                jmp dword ptr cs:old09int
                
reg_hander      endp
;==========================================================================

; Interception of keyboard interrupt
; There is no input and output parameters (some registers can be overwritten)

intercept_09int:
                xor bx, bx              ; bx  = 0
                mov es, bx              ; es = bx
                mov bx, 09h * 4         ; bx = address(keyboard-interrupt-function)
                cli                     ; locking of all interrupts not to knock registers 
                
; Recording new addresses, saving old addresses of 09h interrupt
                mov ax, word ptr es:[bx]
                mov word ptr old09int, ax
                mov ax, word ptr es:[bx + 2]
                mov word ptr old09int + 2, ax
                
                mov word ptr es:[bx], offset reg_hander
                mov ax, cs
                mov word ptr es:[bx + 2], ax
                sti                     ; permission to do alien-interrupts again
                
; Make the program resident (31h function of 21h interrupt), calculating necessary memory in pragraphes
                mov ax, 3100h
                mov dx, offset end_prog
                shr dx, 4
                inc dx
                int 21h

; End of the function
                ret
;==========================================================================


;==========================================================================
; Cleaning of frame	
; Input parameters:
;   es - segment register of videomemory
;   di - the start parameter of cleaning zone
;   ax - the bacground colour
;   cx - the length of lines
;   bx - the number of lines
; Output parameters:
;   di - changed parameter	
;==========================================================================

clean_frame:
                push si cx
                mov si, 0               ; for comparing with delx
clean_line:				
                stosw                   ; mov es:[di], ax + inc di
                loop clean_line         ; fill the line with empty symbol
                
                add di, (80 - delx) * 2 ; going to the next line (di = new_line_address)
                inc si
                mov cx, delx
                cmp si, bx
                jne clean_line

                pop cx si
                
                ret
;==========================================================================


;==========================================================================
; Painting frame wrapper
; Input parameters:
;   es - segment register of videomemory
; Output parameters:
;   di - changed parameter
;   si - changed parameter
;   ax - changed parameter
;   cx - changed parameter
;==========================================================================

paint_frame_wrapper:

; The upper line
                mov di, (y1 * 80 + x1 - 81) * 2
                mov ax, left_upper
                stosw                   ; angle

                mov ax, horizontal      ; the upper line
                mov cx, delx
                rep stosw

                mov ax, right_upper
                stosw                   ; angle

; Vertical lines
                mov di, (y1 * 80 + x1 - 1) * 2
                mov ax, vertical
                mov si, 1               ; the counter

update_counter:	
                mov cx, dely

vertical_print:	
                stosw
                add di, 79 * 2
                loop vertical_print
                
                mov di, (y1 * 80 + x2) * 2
                add si, 1
                cmp si, 2
                je update_counter
                
; The lower line
                mov ax, left_down       ;  angle
                mov di, (y2 * 80 + x1 - 1) * 2
                stosw
                
                mov ax, horizontal
                mov cx, delx
                rep stosw
                
                mov ax, right_down      ; angle
                stosw
                ret
;==========================================================================


;==========================================================================
; Remembering of old videomemory
; Input parameters:
;   es - segment register of videomemory
;   di - the start parameter of remembering memory
; Output parameters:
;   di - changed parameter
;==========================================================================

remember_old_videomem:
                push ax bx cx               ; remember old registers
                
; Preparing the registres to save old videomemory
                mov cx, dely + 2
                mov bx, offset old_videomem ; bx - the counter of old memory
                        
save_text:
                push cx	
                mov cx, delx + 2
                
save_line:				
                mov dx, es:[di]
                mov [bx], dx
                add di, 2
                add bx, 2
                
                loop save_line
                
                pop cx
                
                add di, (78 - delx) * 2
                loop save_text
                
                pop cx bx ax
                ret
;==========================================================================


;==========================================================================
; Reestablishment of old videomemory
; Has almost the same description of code as the previous function
; Input parameters:
;   es - segment register of videomemory
;   di - the start parameter of remembering memory
;   oldvideomem - the place, where the old videomemory stores
; Output parameters:
;   di - changed parameter
;==========================================================================

establish_old_videomem:
                push ax bx cx
                
                mov cx, dely + 2
                mov bx, offset old_videomem ; bx - the counter of old memory
                        
load_text:
                push cx
                mov cx, delx + 2
                
load_line:				
                mov dx, [bx]
                mov es:[di], dx
                add di, 2
                add bx, 2
                
                loop load_line
                
                pop cx
                
                add di, (78 - delx) * 2
                loop load_text
                
                pop cx bx ax
                ret
;==========================================================================


;==========================================================================
; Print all beautiful register-words

print_registers:
                mov bx, videomemory
                mov es, bx

                mov ah, string_color
                mov cx, 160 * 2
                mov bx, offset reg_memory   ; make bx the address of registers-massive

; In the following lines dx is considered to be the parameter of function print_phrase
                mov si, offset reg_a        ; make si the address of the message reg_a
                pop dx
                mov cs:[bx], dx             ; saving the number in massive to restore it in future
                call print_phrase
                
                mov si, offset reg_b        ; make si the address of the message reg_b
                pop dx
                mov cs:[bx + 2], dx         ; saving the number in massive to restore it in future
                call print_phrase
                
                mov si, offset reg_c        ; make si the address of the message reg_c
                pop dx
                mov cs:[bx + 4], dx         ; saving the number in massive to restore it in future
                call print_phrase
                
                mov si, offset reg_d        ; make si the address of the message reg_d
                pop dx
                mov cs:[bx + 6], dx         ; saving the number in massive to restore it in future
                call print_phrase
                
                mov dx, cs:[bx + 6]         ; push all the values to stack backwards
                push dx
                mov dx, cs:[bx + 4]
                push dx
                mov dx, cs:[bx + 2]
                push dx
                mov dx, cs:[bx]
                push dx
                
                ret
;==========================================================================


;==========================================================================
; Print the rigister, where:
; cx - its coodinate in videomemory (changes the final value - add by 80 * 2 -> the next line)
; si - the address of phrase (doesn't change in this function)
; es - the segment register of videomemory
;==========================================================================

print_phrase:
                mov di, cx
                sub di, delx * 2    ; di - the address of the beginning of the pharse

; Analyses the line, where '$'- terminal symbol
print_msg:
                mov al, cs:[si]
                inc si
                cmp al, '$'
                je end_printing
                stosw               ; pushing the letter in videomemory

                jmp print_msg

; Printing the numbers of registers
end_printing:
                push cx ax bx
                mov ax, dx
                call hex_transfer   ; printing the hexidecimalnumber of register
                pop bx ax cx
                
                add cx, 80 * 2      ; going to the next line
                ret
;==========================================================================


;==========================================================================
; Hex numbers transfer

hex_transfer:
                mov bx, base_hex    ; the base of translating
                xor cx, cx

; Remembering all tailings in stack
split_piece_hex:
                xor dx, dx
                div bx

                push dx
                inc cx

                test ax, ax
                jnz split_piece_hex

; Print the symbols on the screen, poping all numbers from stack
print_hex_number:
                pop dx
                cmp dx, 9
                jae letter_hex      ; make the letter analog of the number, which is larger than 9

                add dl, '0'
                jmp out_number
                
; Changing the ASCII code of large number
letter_hex:
                sub dx, 10
                add dx, letter_a
                
; Printing the letter
out_number:
                mov ah, string_color
                mov al, dl
                stosw

                loop print_hex_number
; Print the letter 'h' to show, that numbers are hexidecimal
                mov al, letter_h
                stosw

                ret
;==========================================================================


;==========================================================================
; DARA STORAGE
.data
old09int        dd 0            ; the address of old keyboard interrupt

; All necessary messages on the frame
reg_a           db 'ax = $'
reg_b           db 'bx = $'
reg_c           db 'cx = $'
reg_d           db 'dx = $'

; The temp memory for old registers
reg_memory      db 20 dup (?)
; The storage for old videomemory to reestablish it again in the future
old_videomem    db (delx + 2) * (dely + 2) * 2 dup (?)

end_prog:
end start
