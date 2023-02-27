section .text
    global rotp

;; void rotp(char *ciphertext, char *plaintext, char *key, int len);
rotp:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edx, [ebp + 8]  ; ciphertext
    mov     esi, [ebp + 12] ; plaintext
    mov     edi, [ebp + 16] ; key
    mov     ecx, [ebp + 20] ; len
    ;; DO NOT MODIFY

    ;; TODO: Implment rotp
    ;; FREESTYLE STARTS HERE

    xor eax, eax

next:
    dec ecx                 ; scade ecx pana cand nu ajunge la 0
    mov bl, [esi + eax]     ; bl = plain[i]
    mov bh, [edi + ecx]     ; bh = key[len - i - 1]
    xor bl, bh              ; xor plain[i], key[len - i - 1]
    mov [edx + eax], bl     ; muta in ciphertext[i] rezultatul xor-ului

    inc eax                 ; creste eax pana ajunge < len
    cmp ecx, 0
    jnz next

    ;; FREESTYLE ENDS HERE
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY