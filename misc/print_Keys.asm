get_input: ;; TODO:Add Commands
   jmp setcursor
   setcursor:
      mov ah, 0x02   ; Set Cursor location
      mov bh, 0x00   ; Page number
      mov dh, 0x18   ; Row 24
      mov dl, 0x00   ; Column 0
      int     0x10
      call print_shell
   input_loop:
      mov ax, 0
      int 0x16
      mov byte [Buffer], al
      cmp al, 27
      je shut
      cmp al, 13
      je print_Tex
      cmp al, 9
      je echo1
      mov ah, 0x0e
      int 0x10
      jmp input_loop

clear_scr:
   mov ah, 0x00 ;; Set the graphical mode
   mov al, 0x02
   int 0x10
   mov ah, 0x0B ;; Change the colour of the background
   mov bh, 0x00
   mov bl, 0x01
   mov dh, 0x00
   mov byte [CursorPOS], dh
   int 0x10
   jmp get_input

echo1:
   mov ah, 0x0B ;; Change the colour of the background
   mov bh, 0x00
   mov bl, 0x00
   int 0x10
   jmp get_input

print_Tex:
   pusha
   mov dh, byte [CursorPOS]
   inc dh
   cmp dh , 0x18
   je clear_scr
   mov ah, 0x02   ; Set Cursor location
   mov bh, 0x00   ; Page number
   ;;mov dh, 0x04   ; Row 4
   int     0x10
   mov byte [CursorPOS], dh
   mov ah, 0x0E
   mov bx, Buffer1
   call ptr_str
   jmp setcursor


print_shell:
   mov ah, 0x0E
   mov bx, shell
   call ptr_str
   jmp input_loop
   

shut:
   shu:
      mov dh, byte [CursorPOS]
      inc dh
      mov ah, 0x02
      int     0x10
      mov ah, 0x0E
      mov bx, Buffer1
      call ptr_str
      jmp shutdown

Buffer1: db "This is test", 0
Buffer: db 0
shell:  db '$: ', 0
CursorPOS: db 0x05
