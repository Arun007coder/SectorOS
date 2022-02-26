[BITS 32]

global shutdown

section .data

section .text

shutdown:
    mov cr6, eax