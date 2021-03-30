; Videoramochka by Vokerlee (2021)

.model tiny

; All necessary ASCII codes and other constants:

vacuum          = 0e00h         ; the empty symbol (space)
background		= 4400h		; the symbol of loading
string_color    = 00111000b

left_upper		= 0ac9h         ; left  upper angle
right_upper		= 0abbh         ; right upper angle

right_down		= 0abch         ; right down angle
left_down		= 0ac8h         ; left  down angle

horizontal		= 0acdh         ; horizontal line
vertical		= 0abah         ; vertical   line

videomemory		= 0b800h


; The coordinates of the frame:

x1				= 8           
x2				= 72
y1				= 5
y2				= 15
delx			equ x2 - x1
dely			equ y2 - y1        

.code
org 100h

start:

; The beginning of the programm

                mov ax, videomemory	; loading videomemory 
                mov es, ax		; in segment register 'es'
                
                mov ax, vacuum
                mov cx, delx		; Width
                mov bx, dely		; Height
                
                mov di, (y1 * 80 + x1) * 2	; the address of the first "pixel"
                mov si, 0			; for comparing with delx
                

; Frame cleaning:

call clean_frame

; Painting frame wrapper:

call paint_frame_wrapper

; Loading background

call load_background

; The message printing

call print_message

                mov cx, 0098h		; the parameter of delay
                mov dx, 0000h
        
                mov ah, 86h
                int 15h
                
                mov ax, 4c00h            
                int 21h
        
; THE END OF THE PROGRAM

;///////////////////////////////////////////////////////////////////////////////////
; Cleaning of frame		
        
clean_frame:

cleaning_x:
                stosw			    ; mov es:[di], ax + inc di
                loop cleaning_x		; fill the line with empty symbol

cleaning_y:
                add di, (80 - delx) * 2	; going to the next line (di = new_line_address)
                inc si
                mov cx, delx
                cmp si, bx
                jne cleaning_x
                
                ret
;///////////////////////////////////////////////////////////////////////////////////
; Painting frame wrapper

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
;///////////////////////////////////////////////////////////////////////////////////
; Loading background

load_background:

                mov si, 0		; for comparing with delx
                mov bx, background
                        
load_back:				        ; painting the loading frame
                mov di, si
                mov ax, 2
                mul di
                mov di, ax
                add di, (y1 * 80 + x1) * 2
                mov cx, dely
                
load_vertical:				    ; the cycle of loading
                mov es:[di], bx
                add di, 80 * 2
                loop load_vertical
                        
                mov cx, 0002h	; the param1 of delay
                mov dx, 0000h	; the param2 of delay
                
                mov ah, 86h		; the delay-function
                int 15h
                        
                inc si			; the increase of the column
                        
                cmp si, delx
                jne load_back
                
                ret
;///////////////////////////////////////////////////////////////////////////////////
;Message printing

print_message:
                mov di, (y1 * 80 + x1 + 80 * 5 + 14) * 2
                mov si, offset load
                mov ah, string_color

print_mess:	lodsb
                cmp al, '$'
                je done
                stosw
                jmp print_mess	

done:
                ret
;///////////////////////////////////////////////////////////////////////////////////				
; DATA STORAGE
.data
load		db 'Your charge of battery is full!!!$'

end start