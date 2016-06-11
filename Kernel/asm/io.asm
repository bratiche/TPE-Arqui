GLOBAL read_port
GLOBAL write_port

section .text

;read_port(port)
read_port:
	push rbp
	mov rbp, rsp
	
	mov rdx, rdi
	in al, dx	

	mov rsp, rbp
	pop rbp
	ret

;write_port(port, data)
write_port:
	push rbp
	mov rbp, rsp

	mov rdx, rdi    
	mov rax, rsi
	out dx, al  

	mov rsp, rbp
	pop rbp
	ret