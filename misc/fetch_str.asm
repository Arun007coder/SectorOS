mov es, bx
mov bx, 0x0000

read_HDD: ;;TODO:Read disk from a particular region
    mov ah, 2       ; Read sector to memory
    mov al, 1       ; Number of sectors to read
    mov ch, 0       ; Cylinder
    mov cl, 1       ; Sector
    mov dh, 0       ; Head
    mov dl, 0x00    ; Which drive to load
    mov bx, 0x7E00  ; Offset
    int 0x13        ; Disk service interrupt
    mov ah, 0x0E    ; Print text to screen
    mov al, [0x7E00]; Place where character is stored in memory
    int 0x10        ; Video services interrupt
    ret             ; Return to the main function
