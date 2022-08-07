extern kernel_stack
extern sys_call_table
extern get_cur_task
extern kernel_schedule
global test_flush_cache
global sys_call
global open
extern to_eoi

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
	mov edi, eax 
    mov eax, ss
    mov ds, eax
	mov eax, edi 
	
	push esi 
	push edx
	push ecx 
	push ebx 
	call [sys_call_table + eax * 4]

	add esp, 16
	call get_cur_task
	push eax
	call to_eoi
	pop eax
	mov esp, eax
	
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
	mov edx, [ebp + 16]
	mov ecx, [ebp + 12]
	mov ebx, [ebp + 8]
	mov eax, 0
	int 0x50
	
	
	pop ebp 
	ret


