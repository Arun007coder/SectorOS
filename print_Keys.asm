get_input: ;; TODO:Add Commands
   mov ax, 0
   int 0x16
   cmp al, 27
   je shutdown
   cmp al, 9
   je echo1
   mov ah, 0x0e
   int 0x10
   jmp get_input

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

beep:
   mov ax, 2305h
   int 0x15
   mov ah, 0x0E
   int 0x10
   jmp get_input

shutdown: ; Code by BiTByte
   mov ax,5300h
   mov bx,0
   int 15h
   push ax

	mov ax,5308h
	mov bx,1
	mov cx,1
	int 15h
	mov ax,5308h
	mov bx,0ffffh
	mov cx,1
	int 15h

	mov ax,5301h
	mov bx,0
	int 15h
	mov ax,530Eh
	mov bx,0
        pop cx

	int 15h
	mov ax,530Dh
	mov bx,1
	mov cx,1
	int 15h
	mov ax,530Fh
	mov bx,1
	mov cx,1
	int 15h
	mov ax,5307h
	mov bx,1
	mov cx,3
	int 15h
        int 20h