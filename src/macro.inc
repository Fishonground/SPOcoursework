%define link 0
%assign word_size 8

%macro colon 3
    section .data
    w_%2:
        previous %2
 	db 0
        db %1, 0
        db %3
    xt_%2:
        dq docol
%endmacro

%macro colon 2
    colon %1, %2, 0
%endmacro

%macro previous 1
    dq link
    %define link w_%1
%endmacro

%macro native 3
    section .data
    w_%2:
        previous %2
        db 0
        db %1, 0
        db %3
    xt_%2:
        dq %2_impl
    section .text
    %2_impl:
%endmacro

%macro native 2
    native %1, %2, 0
%endmacro
