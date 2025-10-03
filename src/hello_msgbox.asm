extern MessageBoxA
extern ExitProcess
section .data
	title db "Hello",0
	message db "Hello world from NASM!!",0
section .text
	global main
main:
	sub rsp,40
	mov rcx,0
	mov rdx,message
	mov r8,title
	mov r9d,0
	call MessageBoxA
	
	mov ecx,0
	call ExitProcess
