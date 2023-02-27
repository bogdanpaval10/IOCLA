; This is your structure
struc  my_date
    .day: resw 1
    .month: resw 1
    .year: resd 1
endstruc

section .text
    global ages

; void ages(int len, struct my_date* present, struct my_date* dates, int* all_ages);
ages:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edx, [ebp + 8]  ; len
    mov     esi, [ebp + 12] ; present
    mov     edi, [ebp + 16] ; dates
    mov     ecx, [ebp + 20] ; all_ages
    ;; DO NOT MODIFY

    ;; TODO: Implement ages
    ;; FREESTYLE STARTS HERE

    xor eax, eax

iterate_vector:
    mov bh, [esi + my_date.day]                     ; bh = zi_prezent
    mov bl, [esi + my_date.month]                   ; bl = luna_prezent
    mov ecx, [esi + my_date.year]                   ; ecx = anul_prezent
    cmp [edi + 8 * eax + my_date.day], bh           ; compara zi_nastere cu zi_prezent
    jbe lower_day
    dec bl                                          ; luna_prezent--

lower_day:                                          ; daca zi_nastere < zi_prezent
    cmp [edi + 8 * eax + my_date.month], bl         ; compara luna_nastere cu luna_prezent
    jbe calculate_age
    dec ecx                                         ; anul_prezent--

calculate_age:
    sub ecx, [edi + 8 * eax + my_date.year]         ; an_prezent - an_curent
    cmp ecx, 0                                                          
    jg add_age                                      ; daca data nasterii este dupa data prezenta
    mov ecx, 0

add_age:
    mov edx, ecx                                    ; edx = varsta
    mov ecx, [ebp + 20]                             ; all_ages
    mov [ecx + 4 * eax], edx                        ; all_ages[i] = varsta

    inc eax
    cmp eax, [ebp + 8]                              ; daca eax < dimensiune vector
    jne iterate_vector
    
    
    ;; FREESTYLE ENDS HERE
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY
