section .text

string_length:
	xor rax, rax
.loop:
	cmp byte [rdi+rax], 0
	jz .end
	inc rax
	jmp .loop
.end:
	ret
	
print_newline:
    mov rdi, 0xA
	call print_char
    ret
    
print_char:
	push rdi
	mov rsi, rsp
	mov rax, 1
	mov rdx, 1
	mov rdi, 1
	syscall
	pop rdi
	ret

print_string:
	push rdi
	call string_length
	pop rsi
	mov rdx, rax
	mov rax, 1
	mov rdi, 1
	syscall
	ret

print_int:
    test rdi, rdi
	jns .plus
	jmp .minus
.plus:
	call print_uint
    ret
.minus:
	push rdi
	mov rdi, '-'
	call print_char
	pop rdi
	neg rdi
	jmp .plus

print_uint:
	mov rax, rdi
	xor r8, r8
	mov rbx, 10
.loop:
	xor rdx, rdx
	div rbx
	push rdx
	inc r8
	test rax, rax
	jnz .loop
	mov rax, 1
	mov rdx, 1
	mov rdi, 1
.print:
	pop r10
	add r10, '0'
	push r10
	mov rsi, rsp
	syscall
	pop r10
	dec r8
	test r8, r8
	jnz .print
	ret

parse_uint:
	xor r9, r9
	mov r9, 10d
   	xor rax, rax
	xor r8, r8
	xor r11, r11
.loop:
	xor r10, r10
	mov r10b, byte[rdi+r11]
	inc r11
.below:
	cmp r10b, '0'
	jb .end
.above:
	cmp r10b, '9'
	ja .end
.value:
	sub r10b, '0'
	mul r9
	add rax, r10
	inc r8
	jmp .loop
.end:
	mov rdx, r8
	ret

parse_int:
	xor r8, r8
	xor rax, rax
	cmp byte [rdi], '-'
	jz .minus
	call parse_uint
	ret
.minus:
	lea rdi, [rdi+1]
	call parse_uint
	test rdx, rdx
	jz .end
	neg rax
	inc rdx
	ret
.end:
	xor rdx, rdx
	ret
	
string_equals:
    mov al, byte [rdi]
    cmp al, byte [rsi]
jne .fail
    inc rdi
    inc rsi
    test al, al
    jnz string_equals
    mov rax, 1
    ret
.fail:
    xor rax, rax
    ret

read_char:
	xor rax, rax
	push rax
	mov rax, 0
	mov rsi, rsp
	mov rdi, 0
	mov rdx, 1
	syscall
	pop rax
	ret

read_word:
    push r14
    xor r14, r14 

    .A:
    push rdi
    call read_char
    pop rdi
    cmp al, ' '
    je .A
    cmp al, 10
    je .A
    cmp al, 13
    je .A 
    cmp al, 9 
    je .A
    test al, al
    jz .C

    .B:
    mov byte [rdi + r14], al
    inc r14

    push rdi
    call read_char
    pop rdi
    cmp al, ' '
    je .C
    cmp al, 10
    je .C
    cmp al, 13
    je .C 
    cmp al, 9
    je .C
    test al, al
    jz .C
    cmp r14, 254
    je .C 

    jmp .B

    .C:
    mov byte [rdi + r14], 0
    mov rax, rdi 
   
    mov rdx, r14 
    pop r14
    ret

string_copy:
    push rdi
    push rsi
    call string_length
    xor r10, r10
    xor r11, r11
    pop rsi
    pop rdi
.loop:
    cmp rax, r10
    jae .copy
    jmp .end
.copy:
    mov r11, [rdi+r10]
    mov qword [rsi], r11
    inc rsi
    inc r10
    jmp .loop
.end:
    ret
   


print_no_word:
    mov rdi, word_buffer
    call print_string
    mov rdi, no_word
    call print_string
    ret
    
cfa:
    add rdi, 9
    call string_length
    add rdi, rax
    add rdi, 2
    mov rax, rdi
    ret
    
find_word:
    xor eax, eax
    mov rsi, [last_word]
.loop:
    push rdi
    push rsi
    add rsi, 9
    call string_equals
    pop rsi
    pop rdi
    test rax, rax
    jnz .found
    mov rsi, [rsi]
    test rsi, rsi
    jnz .loop
    xor rax, rax
    ret
.found:
    mov rax, rsi
    ret

