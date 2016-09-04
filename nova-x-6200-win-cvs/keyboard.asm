GLOBAL _getkeybbufsize 
GLOBAL _getkeybbufchar 
GLOBAL _initkeyb        ; Stellt das Keyboard f�r den PM ein

[BITS 32]


;----------------------
; Keyboard Einstellungen
; L�schen des Keyboard-Buffer
_initkeyb:
        pusha
        CLI
        mov eax, 0x38   ; Setzten der Register
        mov es, ax
        mov edi, 0x03
        xor eax, eax
        CLD
        mov ecx, 0x43   ; 0x43  Zeichen werden gel�scht
        rep stosw
        STI
        popa
        ret 

;----------------------
; Get Keyboard Buffer size
; Liefert die Anzahl der Scan Codes im Keybboard Buffer 
_getkeybbufsize:
            mov eax, 0x38
            mov gs, eax
            xor eax, eax
            mov al, [gs:4]
         ret

;----------------------
; Get Keyboard Buffer Char
; Liefert das n-te (Param) �bergebene Zeichen
_getkeybbufchar:
         push bp				; Standart Teil							
         mov bp,sp				
         xor eax, eax
         mov al,[bp+6]				; AX wird dem Wert, der �bergeben wird zugewiesen
         add eax, 0x05               ; Keybboard Buffer
         push si
         mov esi, eax
         mov eax, [gs:si]            ; Lesen des Zeichens
         pop si
         mov sp, bp
         pop bp
         ret 


