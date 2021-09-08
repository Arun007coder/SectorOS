get_input: ;; TODO:Add Commands
   Buffer: times 10 db 0
   shell:  db '$: ', 0
   jmp setcursor
   setcursor:
      mov ah, 0x02   ; Set Cursor location
      mov bh, 0x00   ; Page number
      mov dh, 0x18   ; Row 24
      mov dl, 0x00   ; Column 0
      int     0x10
      mov ah, 0x0E
      mov bx, shell
      call ptr_str
      jmp input_loop
   input_loop:
      mov ax, 0
      int 0x16
      mov bx, Buffer
      mov [bx], al
      inc bx
      cmp al, 27
      je shutdown
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
   int 0x10
   jmp get_input

echo1:
   mov ah, 0x0B ;; Change the colour of the background
   mov bh, 0x00
   mov bl, 0x05
   int 0x10
   jmp get_input

print_Tex:
   pusha
   mov ah, 0x0E
   mov bx, Buffer
   .lop:
      mov al, [bx]
      int 0x10
      add bx, 1
      cmp al, 13
      je ext
      jmp .lop

   ext:
      popa
      jmp input_loop