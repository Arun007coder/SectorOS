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