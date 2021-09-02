mov ah, 0x0E
mov al, 65

Print_Alph:
    int 0x10
    inc al

    cmp al, 91
    je print_al
    jne Print_Alph

