section .bss
rs:             resq 65536
u_mem:          resq 65536
dict: 		resq 65536
word_buffer:    resb 1024
state:          resq 1	

section .text

; 											Минимальный набор слов


; 											Стековые комбинаторы


native "drop", drop
    pop rax
    xor rax, rax
    jmp next

native "swap", swap
    pop rdx
    pop rax
    push rdx
    push rax
    jmp next

native "dup", dup
    push qword [rsp]
    jmp next


; 											Арифметические операции


native "+", plus
    pop rdx
    pop rax
    add rax, rdx
    push rax
    jmp next

native "-", minus
    pop rdx
    pop rax
    sub rax, rdx
    push rax
    jmp next
  
native "*", mul
    pop rdx
    pop rax
    imul rdx
    push rax
    jmp next

native "/", div
    pop rcx
    pop rax
    cqo
    idiv rcx
    push rax
    jmp next

native "%", mod
    pop rcx
    pop rax
    cqo
    idiv rcx
    push rdx
    jmp next

native "=", equal
    pop rax
    pop rcx
    cmp rax, rcx
   	jz .eq
    push 0
   	jmp next
.eq:
    push 1
    jmp next

native "<", ls
    pop rax
    pop rcx
    cmp rcx, rax
    jl .less
    push 0
    jmp next
.less:
    push 1
    jmp next    
    
    
;	 										Логические операции       
    
    
native "not", not
    pop rax
    test rax, rax
    setz al
    movzx rax, al
    push rax
    jmp next

native "and", and
    pop rax
    and [rsp], rax
    jmp next

native "or", or
    pop rax
    or [rsp], rax
    jmp next

native "land", land
    pop rax
    pop rcx
    test rax, rax
    jz .no
    push rcx
    jmp next
.no:
    push rax
    jmp next

native "lor", lor
    pop rax
    pop rcx
    test rax, rax
    jnz .yes
    push rcx
    jmp next
.yes:
    push rax
    jmp next


; 											Работа со стеком возвратов


native ">r", to_rstack
    pop rax
    sub rstack, 8
    mov qword [rstack], rax
    jmp next

native "r>", from_rstack
    mov rax, [rstack]
    push rax
    add rstack, 8
    jmp next

native "r@", nondestr_rstack
    push qword [rstack]
    jmp next
    
    
; 											Работа с памятью


native "@", read
    pop rax
    push qword [rax]
    jmp next

native "!", write
    pop rax
    pop rdx
    mov [rax], rdx
    jmp next

native "c!", write_c
    pop rax
    pop rdx
    mov [rax], dl
    jmp next

native "c@", read_c
    pop rax
    movzx rax, byte [rax]
    push rax
    jmp next

native "execute", execute
    pop rax
    mov w, rax
    jmp [rax]


; 											Управление выполнением


native "docol", docol
        sub rstack, word_size		
        mov [rstack], pc	; save ptr to next xt
        add w, word_size	    	
        mov pc, w	    	
        jmp next

native "branch", branch
    mov pc, [pc]
    jmp next

native "branch0", branch0
    pop rax
    test rax, rax
    jnz .skip
    mov pc, [pc]
    jmp next
.skip:
    add pc, 8
    jmp next


; 											Служебные


native "lit", lit
    push qword [pc]
    add pc, 8
    jmp next
    
native "sp", sp
    push rsp
    jmp next
        
        
        
native "bye", bye
    mov rax, 60
    xor rdi, rdi
    syscall

native ".", printi
    pop rdi
    call print_int
    jmp next
    
native ".S", show
    mov rax, rsp
.loop:
    cmp rax, [rsp_b]
    jz next
    mov rdi, [rax]
    push rax
    call print_int
    call print_newline
    pop rax
    add rax, 8
    jmp .loop

    
native "count", count
    pop rdi
    call string_length
    push rax
    jmp next

native "word", word
    pop rdi
    call read_word
    push rdx
    jmp next
    
    
native "syscall", syscall
    pop r9
    pop r8
    pop r10
    pop rdx
    pop rsi
    pop rdi
    pop rax
    syscall
    push rax
    push rdx
    jmp next

native "cfa", cfa
    pop rdi
    call cfa
    push rax
    jmp next

native "find_word", find_word
    pop rdi
    call find_word
    push rax
    jmp next

native 'save_lw', save_lw
  push last_word
  jmp next

native "here", here
    push qword [here]
    jmp next

native ",", comma
    mov rax, [here]
    pop qword [rax]
    add qword [here], 8
    jmp next

native "'", tick, 1
    mov rdi, word_buffer
    call read_word
    call find_word

    test rax, rax
    jz .no_word

    mov rdi, rax
    call cfa
   
    cmp qword[state], 1		; if interpret push xt
    jne .interpret

    mov rdi, [here]			; else adding xt_lit & xt to word is defined by ':'
    mov qword [rdi], xt_lit             	
    add qword [here], 8

    mov rdi, [here]
    mov [rdi], rax              	
    add qword [here], 8
    jmp next
.no_word:
    call print_no_word
    jmp next 
.interpret:
    push rax
    jmp next

native ":", col
    mov qword [state], 1     	; compil mode now
    
    mov rax, [here]
    mov rcx, [last_word]

    ; define header
    mov [rax], rcx          	; link to previous word

    mov [last_word], rax    	; update last word
    add rax, word_size

    mov byte[rax], 0		; \0-name-\0
    inc rax

    push rax
    mov rdi, word_buffer
    call read_word
    pop rax
    mov rsi, rax
    call string_copy        	; define name

    mov rax, rsi
    mov byte[rax], 0		; flag
    inc rax                 	

    ; define ex token
    mov qword [rax], docol_impl   	; docol
    add rax, 8
    mov [here], rax
    jmp next

native ";", semicol, 1
    mov qword [state], 0
    mov rax, [here]
    mov qword [rax], xt_exit    ; add xt_exit to end
    add rax, word_size
    mov [here], rax
    jmp next
