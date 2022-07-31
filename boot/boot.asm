mbrseg equ 0x7c0     ;启动扇区存放段地址
loadseg equ 0x800  ;loader segment address
kernel equ 0x1000 ; kernel segment address

jmp   start
times 0x30 db 0 
message:       db 'boot successfully', 0x0d, 0x0a,'$'

start:
	mov ax, mbrseg
	mov ds, ax
	mov ax, mbrseg 
	mov ss, ax
	mov sp, message - 1
	mov bp, sp
	mov si, message
	call printstr
	call read_loader 

	jmp loadseg:0

printstr:                  ;显示指定的字符串, 以'$'为结束标记
      mov al,[si]
      cmp al,'$'
      je disover
      mov ah,0eh
      int 0x10
      inc si
      jmp printstr
disover:
      ret

read_loader: 
	   mov ax, loadseg ;segment
	   mov es, ax
	   mov bx, 0 ; offset
	   mov ch, 0 ;zhumian 
	   mov dh, 0 ; citou 
	   mov cl, 2 ; read from which sanmian
	   mov ah, 2 ; read disk
	   mov al, 4 ; num of sanmian
	   mov dl, 0 ; soft disk
	   int 0x13
	   ret 

times 510-($-$$) db 0
db 0x55,0xaa






