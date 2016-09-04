[BITS 32]

GLOBAL _tss_update_gdt
GLOBAL _tss_testt

; Speichern des GDT Eintrages, der mit LGDT geladen wurde
_tss_update_gdt:
   CLI
   pusha
   mov ax, 0x28
   mov ds, ax
   SGDT [0]  
   popa
   STI
   ret




