[BITS 32]

GLOBAL _mmwritemementry
GLOBAL _mmreadmementry
GLOBAL _setPTEintoMEM
GLOBAL _setPaging
GLOBAL _clearfreemem
EXTERN _systemstop
GLOBAL _getDeskriptorFromMem
GLOBAL _getGDTFromMem

_setPTEintoMEM:
         push bp				; Standart Teil							
         mov bp,sp			
         push ds
         mov ax, 0x30
         mov ds, ax
         mov eax,[bp+6]			; AX wird dem Wert, der übergeben wird zugewiesen (Pointer)
         mov ecx,[bp+0x0a]
         mov [ds:eax], ecx
         pop ds
         mov sp, bp
         pop bp
         ret

_clearfreemem:
         push bp				; Standart Teil							
         mov bp,sp			
         push es
         mov ax, 0x30
         mov es, ax
         mov edi, 0x1000
         xor eax, eax
         mov ecx, 0x800
         rep stosd
         pop es
         mov sp, bp
         pop bp
         ret


_getDeskriptorFromMem:
         push bp				; Standart Teil							
         mov bp,sp			
         push es
         push ds
         mov ax, ds
         mov es, ax
         mov ax, 0x28
         mov ds, ax
         xor eax, eax
         mov ax, [bp+0x0a]
         mov bx, 0x08
         mul bx
         mov esi, eax
         mov eax, [bp+0x0e]
         sub eax, 0x50000
         add esi, eax
         xor eax, eax
         mov ax, [bp+0x06]         
         mov edi, eax
         mov cx, 0x04
         rep movsw
         pop ds
         pop es
         mov sp, bp
         pop bp
         ret


_getGDTFromMem:
         push bp				; Standart Teil							
         mov bp,sp			
         xor eax, eax
         mov ax, [bp+0x06]
         SGDT [eax]
         pop bp
         ret


_setPaging:
         push bp				; Standart Teil							
         mov bp,sp			
         mov eax, 0x10000
         mov cr3, eax
         
         ; PG Bit (Bit 31) setzen um Paging zu aktivieren
          mov ebx,cr0
          or ebx,80000000h
          mov cr0,ebx
          
          mov ax, 0x50
          mov ds, ax
         
         jmp 0x48:.go
.go:         
         pop bp
         ret

;------------------------
; Diese Funktion liest einen Memory Manager HEADER Eintrag aus dem Ram.
; Dieser Eintrag ist 20 Byte lang und wird direkt in einen Buffer geschrieben.
; Übergeben wird 1. der Buffer 2. die Adresse, wo der Header ist
;------------------------
_mmreadmementry:
         push bp				; Standart Teil							
         mov bp,sp			
         push ds	            ; Ein wenig sichern
         push es
         push edi
         mov ax, ds
         mov es, ax             ; Es wird direkt in eine Variable kopiert, deshalb muss es auch auf DS
         mov eax,[bp+6]			; AX wird dem Wert, der übergeben wird zugewiesen (Pointer)
         mov esi, eax           ; Es wird nach DS:SI geschrieben
         xor eax, eax
         mov eax,[bp+0x0a]
         mov edi, eax           ; Es wird von ES:DI gelesen
         mov cx, 0x14           ; 0x14 = 20 Byte schreiben
         rep movsb                   
         pop edi                ; Alles wieder zurückhohlen
         pop es
         pop ds
         mov sp, bp
         pop bp
         ret 

;------------------------
; Diese Funktion schreibt einen Memory Manager HEADER Eintrag in den Speicher
; Dieser Eintrag ist 20 Byte lang und wird durch die einzeln übergebenen Paramter geschrieben.
; Übergeben wird der Header - bestehend aus 4 long und 2 short ints - sowie die Adresse, an die geschrieben werden soll.
;------------------------        
_mmwritemementry:
         push bp				; Standart Teil							
         mov bp,sp			
         push ds	            ; Alles sichern
         push es
         push edi
         mov ax, ds             ; Es soll von einer Variable ins DS geschrieben werden
         mov es, ax
         mov eax,[bp+6]				; AX wird dem Wert, der übergeben wird zugewiesen
     
         mov edi, eax           ; EDI = Adresse, wohin geschrieben werden soll
         mov eax,[bp+0x0a]      ; Auslesen der einzelnen Variablen mit anschließendem schreiben
         CLD  
         stosd       
         mov eax,[bp+0x0e]
         stosd
         mov eax,[bp+0x12]
         stosd
         mov eax,[bp+0x16]
         stosd
         xor eax, eax
         mov ax,[bp+0x1A]
         stosw
         mov ax,[bp+0x1C]
         stosw
         xor eax, eax           ; OK, das wars. Nun Register wiederherstellen und zurück.
         pop edi
         pop es
         pop ds
         mov sp, bp
         pop bp
         ret 
