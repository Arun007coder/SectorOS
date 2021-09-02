start_loop:
    pusha
    mov cl, 10
    jmp looper

looper:
    dec cl
    cmp cl, 0
    je end_loop
    jne looper

end_loop:
    popa
    ret
