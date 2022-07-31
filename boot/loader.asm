jmp start 
loadseg equ 0x800  ;loader segment address
load   equ 0x8000  ;loader address
kernel equ 0x70000 ; kernel address
kernelseg equ 0x7000 ;kernel segment address
gdtseg equ 0x7e0 ;gdt segment address 
gdtptr: dw 24 - 1; length of gdt
        dd 0x7e00 ;gdt start address
  

idtptr: dw 128 - 1 ; idt length
idt:    dd 0x6000 ;idt start address 

stack_space: times 256 db 0
top_of_stack equ load + $

real_kernel_start equ 0x30400
real_kernel equ 0x30000

code_selector equ 1 << 3
data_selector equ 2 << 3
;stack_selector equ 3 << 3
 

message: db 'load successfully', 0x0d, 0x0a, '$'
 
now_cylind db 0
now_header db 0
now_sector db 6

max_cylind equ 79
max_header equ 1
max_sector equ 18

kernel_sector equ 35

now_load_kernel_sector db 255 ;count't dw
start: 
	bits 16
    mov ax, loadseg
    mov ds, ax
    mov si, message
    call printstr
    call read_kernel
	call cancel_cursor 
    jmp start_protect
	jmp $

read_disk_one_sector:
	mov bx, 0
	mov ch, [now_cylind] 
	mov dh, [now_header] 
	mov cl, [now_sector] 
	mov ah, 2
	mov al, 1
	mov dl, 0
	int 0x13
	jc read_disk_one_sector
	inc byte[now_sector] 
	ret
	
read_kernel:
	mov ax, kernelseg ;segment
	mov es, ax
read_disk_loop:
	call read_disk_one_sector
	mov ax, es 
	add ax, 0x0020 
	mov es, ax
	inc byte[now_load_kernel_sector]
	cmp byte[now_load_kernel_sector], kernel_sector 
	je  read_kernel_over
	cmp byte[now_sector], max_sector + 1
	jne read_disk_loop 
	mov byte[now_sector], 1
	xor byte[now_header], 0x01
	cmp byte[now_header], 0
	jne read_disk_loop 
	inc byte[now_cylind]
	cmp byte[now_cylind], max_cylind + 1
	jne read_disk_loop
	;inc byte[now_cylind]
	;cmp byte[now_cylind], max_cylind + 1
	;jne read_disk_loop 
	;mov byte[now_cylind], 0
	;inc byte[now_header]
	;cmp byte[now_header], max_header + 1
	;jne read_disk_loop 
read_kernel_over:
	ret
	
;	mov bx, 0 ; offset
;   mov ch, 0 ; zhumian 
;    mov dh, 0 ; citou 
;    mov cl, 6 ; read from which sanmian
;    mov ah, 2 ; read disk
;    mov al, 72 ; num of sanmian
;    mov dl, 0 ; soft disk
;    int 0x13
;	jc read_kernel 
	

printstr:                  ;显示指定的字符串, 以'$'为结束标记
    mov al,[si]
    cmp al,'$'
    je disover
    mov ah,0x0e
    int 0x10
    inc si
    jmp printstr
disover:
    ret

cancel_cursor:
	mov cx, 0x2000
	mov ah, 0x01
	int 0x10
	ret 

start_protect:
    call init_gdt
    jmp inprotect
    ret
 
init_gdt:
    ;gdt address
    lgdt [gdtptr]
 
	mov ax, gdtseg
	mov es, ax
     ;create space 
    mov dword [es:0], 0
    mov dword [es:4], 0
     ;create code 
    mov dword [es:code_selector], 0x0000ffff
    mov dword [es:code_selector + 0x4], 0x00c09800
     ;create data
    mov dword [es:data_selector], 0x0000ffff
    mov dword [es:data_selector + 0x4], 0x00c09200
    ;create stack
     ;mov dword [es:stack_selector], 0x00007a00
     ;mov dword [es:stack_selector + 0x4], 0x00409600
    ret

inprotect:
     ; open 20 bit address
	in al, 0x92
    or al, 0x2
    out 0x92, al
 
     ;ban interrupte
    cli
 
    ; set cr0, open safe
    mov eax, cr0
    or eax, 1
    mov cr0, eax
	jmp code_selector: load + inprotectmode ;16位的描述符选择子：32位偏移[bits 32]
	inprotectmode:

    bits 32
    mov ax, data_selector
    mov ds, ax
    mov es, ax
    mov gs, ax
	mov ss, ax
	mov esp, top_of_stack 
	mov ebp, esp
	;call test_win 
     ;call start_interpret
     ;call register_idt
	call init_kernel 
	jmp code_selector: real_kernel_start
	jmp $
 
bits 32
test_win:
	;cli
    ;mov al, 0x61
    ;out 0x20, al
    ;in al, 0x60
    mov ebx, (80 * 0 + 0 ) * 2
    mov ah, 0x0c
	mov al, '1'
    add ebx, 0x0b8000 
    mov [ds:ebx], ax

    mov ebx, (80 * 0 + 1 ) * 2
    mov ah, 0x0c
    mov al, '2'
    add ebx, 0x0b8000
    mov [ds:ebx], ax
	;mov al, 0x20
    ;out 0x20, al
    ;out 0xa0, al
    ;iret
	ret
;elf 
bits 32
init_kernel:
	xor esi, esi
	mov cx, word [kernel + 0x2c]  ; e_phnum
	movzx ecx, cx
	mov esi, [kernel + 0x1c]; e_phoff
	add esi, kernel 
begin:
	mov eax, [esi]
	cmp eax, 0
	jz noaction

	mov eax, [esi + 0x14] ; size 10
	cmp eax, 0
	jz noaction

	push dword [esi + 0x14]   ;size 10
	mov eax, [esi + 0x4]
	add eax, kernel 
	push eax  ;src
	push dword [esi + 0x8] ;dst
	call memcpy
	add esp, 12
noaction:
	add esi, 0x20 ; e_phentsize
	dec ecx
	jnz begin

	ret 

;memcpy(src, dst, size)

bits 32
memcpy:
	mov ebp, esp 
	sub esp, 12
	mov [ebp - 4], ecx
	mov [ebp - 8], esi 
	mov [ebp - 12], ebx
	mov ecx, [ebp + 12] ;size
	mov esi, [ebp + 8]  ;src
	mov ebx, [ebp + 4]  ;dst
memcpyloop:
	mov eax, [esi]
	mov [ebx], eax 
	inc esi
	inc ebx 
	loop memcpyloop
	mov ecx, [ebp - 4]
	mov esi, [ebp - 8]
	mov ebx, [ebp - 12]
	add esp, 12
	ret 


bits 32
start_interpret:
    call init_8259A
    lidt [load + idtptr]
    call register_idt
    sti
    ret
 
bits 32
init_8259A:
    ;ICW1
    mov al, 0x11
    out 0x20, al
    call io_delay
    out 0xa0, al
    call io_delay
    ;ICW2
    mov al, 0x20
    out 0x21, al
    call io_delay
    mov al, 0x28
    out 0xa1, al
    call io_delay
     ;ICW3
    mov al, 0x4
    out 0x21, al
    call io_delay
    mov al, 0x2
    out 0xa1, al
    call io_delay
     ;ICW4
    mov al, 0x1
    out 0x21, al
    call io_delay
    out 0xa1, al
    call io_delay
     ;OCW1 
 
    mov al, 0xfd ; 1111 1101
    out 0x21, al
    call io_delay
    mov al, 0xff ; 1111 1111 
    out 0xa1, al
    call io_delay
     ;OCW2
    mov al, 0x20
    out 0x20, al
    call io_delay
    ret
 
register_idt:
     ;mov
     mov ah, 0
     mov al, 0x21
     mov bl, 8
     mul bl
     mov di, ax
     add di, 0x6000
     mov word [ds:di], test_win + 0x8000
     mov word [ds:di + 0x02], code_selector
     mov dword [ds:di + 0x04], 0000000000000000_1_00_01110_00000000b
     ret
 
 io_delay:
     nop
     nop
     nop
     nop
     ret
 times 512 * 4 - ($-$$) db 0
 
 
 

