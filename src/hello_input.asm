default rel                ; << Enable RIP-relative addressing automatically
extern GetStdHandle
extern WriteConsoleA
extern ReadConsoleA
extern ExitProcess

section .data
    prompt db "Enter your name: ", 0
    prompt_len equ $ - prompt

    output db "Hello, ", 0
    output_len equ $ - output

section .bss
    buffer resb 64
    bytesRead resq 1
    bytesWritten resq 1

section .text
	global main
main:
	sub rsp,40
	
	mov ecx,-11
	call GetStdHandle
	mov rbx,rax

	mov rcx,rbx
	lea rdx,[rel prompt]
	mov r8d,prompt_len
	lea r9,[rel bytesWritten]
	sub rsp,32
	call WriteConsoleA
	add rsp,32

	mov ecx,-10
	call GetStdHandle
	mov rcx,rax
	lea rdx,[rel buffer]
	mov r8d,64
	lea r9,[rel bytesRead]
	sub rsp,32
	call ReadConsoleA
	add rsp,32
	
	mov rcx,rbx
	lea rdx,[rel output]
	mov r8d,output_len
	lea r9,[rel bytesWritten]
	sub rsp,32
	call WriteConsoleA
	add rsp,32

	mov rcx,rbx
	lea rdx,[rel buffer]
	mov r8d,64
	lea r9,[rel bytesWritten]
	sub rsp,32
	call WriteConsoleA
	add rsp,32
	
	mov ecx,0
	call ExitProcess


