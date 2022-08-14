extern kernel_stack
extern sys_call_table
extern get_cur_task
extern kernel_schedule
global test_flush_cache
global sys_call
global open
global write
global read 
global close 
;extern to_eoi

sys_call:
    cli
    sub esp, 4
    pushad
    push ds
    push es
    push fs
    push gs

    mov esp, kernel_stack
    add esp, 4096
	mov ebx, eax 
    mov eax, ss
    mov ds, eax
	mov eax, ebx 
	
	push edi 
	push edx
	push ecx 
	push esi 
    call [sys_call_table + eax * 4]
	mov ebx, eax
	add esp, 16
	call get_cur_task
	;push eax
	;pop eax
	mov esp, eax
	mov esi, eax 
	mov [esi + 11 * 4], ebx   ;save return to eax
	
    pop gs
    pop fs
    pop es
    pop ds
    popad
    add esp, 4
    iret



open:
	push ebp 
	mov ebp, esp 
	;mov edx, [ebp + 16]
	mov ecx, [ebp + 12]
	mov esi, [ebp + 8]
	mov eax, 0
	int 0x50
	
	
	pop ebp 
	ret

write:
	push ebp 
	mov ebp, esp 
	mov edx, [ebp + 16]
	mov ecx, [ebp + 12]
	mov esi, [ebp + 8]
	mov eax, 1
	int 0x50
	pop ebp 
	ret

read:
	push ebp 
	mov ebp, esp 
	mov edx, [ebp + 16]
	mov ecx, [ebp + 12]
	mov esi, [ebp + 8]
	mov eax, 2
	int 0x50
	pop ebp 
	ret

close:
	push ebp 
	mov ebp, esp 
	mov esi, [ebp + 8]
	mov eax, 3
	int 0x50
	pop ebp 
	ret
