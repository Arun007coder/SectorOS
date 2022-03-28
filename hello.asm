[BITS 32]

;; To compile this file. execute the following command: nasm -f bin -o test.com hello.asm

mov eax, 2
mov ebx, 0x0A
int 0x80

mov eax, 2
mov ebx, 'H'
int 0x80
mov eax, 2
mov ebx, 'E'
int 0x80
mov eax, 2
mov ebx, 'L'
int 0x80
mov eax, 2
mov ebx, 'L'
int 0x80
mov eax, 2
mov ebx, 'O'
int 0x80
mov eax, 2
mov ebx, ' '
int 0x80
mov eax, 2
mov ebx, 'W'
int 0x80
mov eax, 2
mov ebx, 'O'
int 0x80
mov eax, 2
mov ebx, 'R'
int 0x80
mov eax, 2
mov ebx, 'L'
int 0x80
mov eax, 2
mov ebx, 'D'
int 0x80
mov eax, 2
mov ebx, '!'
int 0x80

mov eax, 2
mov ebx, 0x0A
int 0x80