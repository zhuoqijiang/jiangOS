extern dis_str
extern to_eoi
extern delay1s
extern cur_task
extern kernel_stack
extern schedule
extern handle_keyboard
global keyboard_handle
global clock_handle
extern kernel_schedule 
[SEGMENT .data]
hello: db 'hello world',0
esp_temp: dd 0
[SEGMENT .text]

keyboard_handle:
	cli
	sub esp, 4
	pushad
	push ds
	push es
	push fs
	push gs ;应禁止中断
	;mov al,  0x61
	;out 0x20, al  ;PIC0_OCW2
	mov [esp_temp], esp 
	mov esp, kernel_stack
	add esp, 4096 
	mov eax, ss
	mov ds, eax

	in  al, 0x60  ;从键盘读入按键扫描码
	mov ah, 0
	push eax 
	call handle_keyboard 
	;add esp, 4
	pop eax
	mov               al , 0x20  ;告诉硬件,中断处理完毕,即发送 EOI 消息
	out               0x20 , al
	out               0xa0 , al
	mov esp, [esp_temp]
	pop gs
	pop fs
	pop es
	pop ds
	popad
	add esp, 4
	iret


clock_handle:
	cli 
	sub esp, 4
	pushad
	push ds
	push es
	push fs
	push gs
	
	mov esp, kernel_stack
	add esp, 4096 
	
	mov eax, ss
	mov ds, eax
	
	call kernel_schedule
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
