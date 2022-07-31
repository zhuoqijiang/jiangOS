
extern start
extern cur_task
extern init_first_process
extern kernel_schedule 
[SECTION .bss]
stack_space  resb 2 * 1024
stack_top:

[SECTION .text]
global _start
bits 32
_start:
	mov esp, stack_top
	mov ebp, esp
	call start
	jmp restart 

bits 32
restart:
;	call init_first_process
;	mov esp, [cur_task]
	call kernel_schedule
	mov esp, eax
;	mov esi, [scheduler]
;	mov esp, [esi]
	pop gs
	pop fs
	pop es
	pop ds
	popad
	add esp , 4
	iretd



	
	
