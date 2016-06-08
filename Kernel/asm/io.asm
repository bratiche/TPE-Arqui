global read_port
global write_port

;read_port(port)
read_port:
	push rbp
	mov rbp, rsp
	
	mov rdx, [rbp + 8]
	in al, dx	

	mov rsp, rbp
	pop rbp
	ret

;write_port(port, data)
write_port:
	push rbp
	mov rbp, rsp

	mov rdx, [rbp + 8]    
	mov al, [rbp + 12]  
	out dx, al  

	mov rsp, rbp
	pop rbp
	ret