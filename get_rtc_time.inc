;; TODO: Read RTC Time
;; FIXME

ini_prt_time:
    pusha

prt_time:
    mov ah, 04h
    int 0x1A
    mov ah, 0x0e
    int 0x10
    jmp prt_time

end_loop:
    popa
    ret