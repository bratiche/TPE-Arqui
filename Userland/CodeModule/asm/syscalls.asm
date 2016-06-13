GLOBAL write
GLOBAL read
GLOBAL exit

read:
	push rbp
	mov rbp, rsp

	mov rax, 3		; syscall id
	mov rbx, rdi	; fd
	mov rcx, rsi	; buffer
					; rdx = size
	int 80h		

	leave
	ret

write:
	push rbp
	mov rbp, rsp
	
	mov rax, 4
	mov rbx, rdi	; fd
	mov rcx, rsi	; buffer
					; rdx = size
	int 80h

	leave
	ret

exit:
	push rbp
	mov rbp, rsp

	mov rax, 1
	mov rbx, rdi
	int 80h

	leave
	ret

