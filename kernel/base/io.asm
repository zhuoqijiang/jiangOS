

extern kernel_space
global show
global display_str
global out_byte 
global in_byte 
global set_lgdt 
global set_lidt
global set_lldt
global set_ltr
global to_mcli 
global to_msti
global set_paging
global in_byte16
global out_byte16 

[SECTION .data]
display_pos: dd 0xb8000

[SECTION .text]
bits 32
show:
	push ebp 
	mov ebp, esp
	mov eax, 0
	mov ax, [ebp + 8]
	mov bx, 80
	mul bx
	add ax, [ebp + 12]
	mov bx, 2
	mul bx 
	mov ebx, eax
	mov ah, [ebp + 16]
	mov al, [ebp + 20]
	add ebx, 0x0b8000 
	mov [ds:ebx], ax
	pop ebp 
	ret

bits 32
out_byte:
	push ebp 
	mov ebp, esp
	mov edx, [ebp + 8]
	mov al, [esp + 12]
	out dx, al
	nop ;delay
	nop ;delay
	pop ebp 
	ret

bits 32
out_byte16:
	push ebp 
	mov ebp, esp
	mov edx, [ebp + 8]
	mov ax, [esp + 12]
	out dx, ax
	nop ;delay
	nop ;delay
	pop ebp 
	ret

bits 32
in_byte:
	push ebp 
	mov ebp, esp
	mov edx, [ebp + 8]
	in al, dx
	mov esi, [ebp + 12]
	mov [esi], al
	nop 
	nop
	pop ebp
	ret

bits 32
in_byte16:
	push ebp 
	mov ebp, esp 
	mov edx, [ebp + 8]
	in ax, dx
	mov esi, [ebp + 12]
	mov [esi], ax 
	nop 
	nop 
	pop ebp
	ret

bits 32
set_lgdt:
	push ebp 
	mov ebp, esp
	mov eax, [ebp + 8]
	lgdt [eax]
	pop ebp
	ret

bits 32
set_lidt:
	push ebp
	mov ebp, esp
	mov eax, [ebp + 8]
	lidt [eax]
	pop ebp 
	ret

bits 32
set_lldt:
	push ebp
	mov ebp, esp
	mov eax, [ebp + 8]
	lldt [eax]
	pop ebp 
	ret

bits 32
set_ltr:
	push ebp
	mov ebp, esp
	mov eax, [ebp + 8]
	ltr [eax]
	pop ebp 
	ret

bits 32
to_mcli:
	cli 
	ret

bits 32
to_msti:
	sti
	ret

bits 32
set_paging:
	push eax 
	mov eax, [kernel_space] 
	mov cr3, eax
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
	pop eax
	ret
