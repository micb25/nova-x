[BITS 32]
SECTION .text

GLOBAL _sqrt2

_sqrt2:
 push bp
 mov bp,sp
 xor eax, eax
 mov edx, [bp+6]
 
 fld dword [edx]
  
 mov sp,bp
 pop bp
 
 fst dword [eax]
 
 ret
