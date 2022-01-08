[BITS 32]

global shutdown

section .data

section .text

shutdown:
    push edi
    push ebx
wait_gate1:
    in al,64h
    and al,2
    jnz wait_gate1
    mov al,0D1h
    out 64h,al
wait_gate2:
    in al,64h
    and al,2
    jnz wait_gate2
    mov al,0FEh
    out 60h,al
;
    xor eax,eax
    mov cr3,eax

reset_wait:
    jmp reset_wait