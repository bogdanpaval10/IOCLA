section .data
    extern len_cheie, len_haystack

section .text
    global columnar_transposition

;; void columnar_transposition(int key[], char *haystack, char *ciphertext);
columnar_transposition:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha 

    mov edi, [ebp + 8]   ;key
    mov esi, [ebp + 12]  ;haystack
    mov ebx, [ebp + 16]  ;ciphertext
    ;; DO NOT MODIFY

    ;; TODO: Implment columnar_transposition
    ;; FREESTYLE STARTS HERE

    xor eax, eax                        ; eax = 0, se itereaza ciphertext
    xor ecx, ecx                        ; ecx = 0, se parcurge vectorul key

iterate_key:
    xor edx, edx                        ; edx = 0, se parcurge haystack cu ratia len_cheie

iterate_haystack:
    push edx                            ; se salveaza edx pe stiva
    imul edx, [len_cheie]               ; se imnulteste edx cu ratia
    add edx, [edi + 4 * ecx]            ; se pozitioneaza pe key pt a afla coloana de parcurs
    cmp edx, [len_haystack]             ; daca edx >=  lungimea haystack-ului
    pop edx
    jge continue

    push edx                            ; se salveaza edx pe stiva
    imul edx, [len_cheie]               ; se imnulteste edx cu ratia
    add esi, [edi + 4 * ecx]            ; se pozitioneaza pe caracterul de la pozitia key[ecx]
    mov dh, [esi + edx]                 ; se muta in dh caracterul de la haystack[key[ecx] + edx]
    mov [ebx + eax], dh                 ; ciphertext[eax] primeste caracterul din dh
    mov esi, [ebp + 12]                 ; haystack

    inc eax
    pop edx                             ; se aduce valoarea initiala a lui edx de pe stiva
    push edx                            ; se salveaza edx pe stiva
    imul edx, [len_cheie]               ; se imnulteste edx cu ratia
    
    cmp edx, [len_haystack]             ; daca edx < lungimea haystack-ului
    pop edx                             ; se aduce in edx valoarea initiala
    inc edx
    jb iterate_haystack

continue:
    inc ecx
    cmp ecx, [len_cheie]                ; daca ecx < lungimea vectorului key
    jb iterate_key
    

    ;; FREESTYLE ENDS HERE
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY