[org 0x7C00] ;; To start reading from the memory address 0x7C00

mov ah, 0x00 ;; Set the graphical mode
mov al, 0x02
int 0x10

mov ah, 0x0B ;; Change the colour of the background
mov bh, 0x00
mov bl, 0x01
int 0x10

mov ah, 0x0E 
mov bx, Text
call ptr_str ;; Print Text variable

mov ah, 0x0E
mov bx, Text2
call ptr_str ;; Prints Text2 variable

mov bx, Text3
call ptr_str ;; Prints Text3 variable

mov bx, Text4
call ptr_str ;; Prints Text4 variable
call read_HDD ;; Reads the active drive for a byte in the second sector and prints it
call get_input ;; To get input from the keyboard

%include 'print_Text.asm'
%include 'print_Keys.asm'
%include 'fetch_str.asm'
%include 'Shutdown.asm'

Text: db 'Welcome to SectorOS primitive kernel', 0x0A, 0x0D, 0
Text2: db 'Basic keyboard input is working', 0x0A, 0x0D, 0
Text3: db 'Trying to read second sector of the Drive' , 0x0A, 0x0D, 0
Text4: db 'Basic sector reading is working', 0x0A, 0x0D, 0


;;Bootsector padding
times 510-($-$$) db 0
db 0x55, 0xAA

;; Write 'A' in the second sector of the disk
times 50 db 'A'
times 50 db 'B'
times 412 db 0