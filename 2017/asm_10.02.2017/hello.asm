;SECTION .data
;msg     db "Hello, world!",0xa  
;len     equ $ - msg
;SECTION .text
;global _start
;_start:                      ; Точка входа в программу
;        mov eax, 4      ; 'write' системный вызов
;        mov ebx, 1
;        mov ecx, msg    ; Указатель на данные
;        mov edx, len    ; Количество данных
;        int 0x80        ;Вызов ядра
;        mov eax, 1      ; '_exit' системный вызов
;        mov ebx, 0      ; Возвращаем 0 (все хорошо)
;        int 0x80        ; Вызов ядра

SECTION .data
    msg db "Hello, world!",0
    fmt db "%s",0Ah
SECTION .text
    GLOBAL _start ; точка входа в программу
    EXTERN printf ; внешняя функция библиотеки Си
    _start:
    push msg ; второй параметр - указатель на строку
    push fmt ; первый параметр - указатель на формат
    call printf ; вызов функции
    add esp, 4*2 ; очистка стека от параметров
    mov eax, 1 ; системный вызов № 1 — sys_exit
    xor ebx, ebx ; выход с кодом 0
    int 80h ; вызов ядра

;
;section .text
;global _start
;s:
;    db 'Hello, world!', 0ah
;l equ $-s
;_start:
;    mov rax,1
;    mov rdi,1
;    mov rsi,s
;    mov rdx,l
;    syscall
;    mov rax,60
;    xor rdi,rdi
;    syscall
