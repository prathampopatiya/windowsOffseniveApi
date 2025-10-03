default rel

extern ExitProcess
extern WinExec

section .data
	cmd db "cmd.exe",0
section .text
	sub rsp,40
	mov rcx,cmd
	mov edx,1
	call WinExec

	mov ecx,0
	call ExitProcess
