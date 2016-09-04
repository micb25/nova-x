[BITS 32]
SECTION .text

GLOBAL _enablefloppymotor       ; Diskettenmotor starten
GLOBAL _disablefloppymotor      ; Diskettenmotor anhalten
GLOBAL _isirq6                  ; Gibt zurück, ob IRQ6 aufgerufen wurde
GLOBAL _clearirq6               ; Löscht das IRQ6 Zeichen

; DMA + Laufwerk A:
_enablefloppymotor:
        pusha
        mov al,0x1c
        mov dx,3F2h       
        out dx,al  
        popa
        ret
        
; Alle Laufwerke ausschalten aber ohne reset
_disablefloppymotor:
        pusha
        mov al,00000100b
        mov dx,3F2h
        out dx,al
        popa
        ret

; Überprüft ob an 0x38:0x105 ein Bit durch den IRQ6 (Interrupt26 - FDC Interrupt) gesetzt wurde.
_isirq6:
        mov ax, 0x38
        mov gs, ax
        xor eax, eax
        mov al, [gs:0x199]
        ret        

; Setzt das FDC IRQ6 Signal Bit auf 0
_clearirq6:
        int 0x26
        pusha
        mov ax, 0x38
        mov gs, ax
        mov byte [gs:0x199], 0x00
        popa
        STI
        ret

