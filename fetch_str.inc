mov es, bx
mov bx, 0x0000

read_HDD: ;;TODO:Read disk from a particular region
    mov ah, 2
    mov al, 1
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0x00
    mov bx, 0x7E00
    int 0x13
    mov ah, 0x0E
    mov al, [0x7E00]
    int 0x10
    ret
