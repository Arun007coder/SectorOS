ptr_str:
    pusha

print_Text:
    mov al, [bx]
    cmp al, 0
    je ex1
    int 0x10
    add bx, 1
    jmp print_Text

ex1:
    popa
    ret

