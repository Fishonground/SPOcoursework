%define pc r12
%define w r13
%define rstack r14

%include "macro.inc"
%include "util.inc"
;%include "kernel.inc"   ;  Minimal word set is here
%include "words.inc"    ;  Predefined words are here

section .data
xt_exit: 	    dq exit
stub:           dq 0
xt_interpreter: dq .interpreter
.interpreter:   dq interpreter_loop
last_word:      dq link
rsp_b:          dq 0
no_word: 	db ' : There is no such word', 10, 0
here: 		dq dict

section .text
global _start
_start:
        mov rstack, rs + 65536 * word_size
        mov [rsp_b], rsp
        mov pc, xt_interpreter
        jmp next

compilation:
    	cmp qword [state], 1
    	jne interpreter_loop
        mov rdi, word_buffer
        call read_word
        call find_word                  ; try as defined word 
        test rax, rax
    	jz .parse_number               ; if word is not in dict - try parse as number
    	mov rdi, rax
    	call cfa                       ; else rax <- xt_word
    	cmp byte [rax - 1], 1          ; check as immediate
    	je .immediate               
    	mov rdi, [here]			; else add xt to here
        mov [rdi], rax              	
        add qword [here], word_size
    	jmp compilation

.parse_number:
    	mov rdi, word_buffer
    	call parse_int

    	test rdx, rdx
    	jz .no_word

        mov rdi, [here]
    	cmp qword [rdi - word_size], xt_branch	; check if prev == branch/branch0
        je .branch
        cmp qword [rdi - word_size], xt_branch0
        je .branch

    	mov rdi, [here]			; else add xt_lit
    	mov qword [rdi], xt_lit     	
    	add qword [here], word_size
    	
.branch:
        mov rdi, [here]			; add num
        mov [rdi], rax
        add qword [here], word_size
        jmp compilation
        
.immediate:
    	mov [stub], rax
    	mov pc, stub
    	jmp next
    	
.no_word:
        call print_no_word
    	mov pc, xt_interpreter
        jmp next
        
interpreter_loop:
        cmp qword [state], 0
        jne compilation
        mov rdi, word_buffer
        call read_word 
        call find_word              ; try as defined word
        test rax, rax
        jz .parse_number            ; if word is not in dict - try parse as number
        mov rdi, rax
        call cfa                    ; else rax <- xt_word
        mov [stub], rax
        mov pc, stub                ; ptr to next ex token = found xt_word
        jmp next
        
.parse_number:
        mov rdi, word_buffer
        call parse_int
        test rdx, rdx
        jz .no_word
        push rax
        mov pc, xt_interpreter
        jmp next
        
.no_word:
        call print_no_word
        mov pc, xt_interpreter
        jmp next

next:
    	mov w, pc		; ptr to current xt_word
    	add pc, word_size		; set pc as ptr to next xt (xt_interpreter / (xt_word or xt_exit) for colon)
    	mov w, [w]		; ptr to word_impl or docol
    	jmp [w]			; jmp to word_impl or docol

exit:
        mov pc, [rstack]	; restore ptr to next xt
        add rstack, word_size		
        jmp next

