print_al:
    mov al, 96
    int 0x10
    mov al, 'a'

    _loop:
        int 0x10
        inc al

        cmp al, 123
        je exit
        jne _loop
