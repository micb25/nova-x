[BITS 16]       ; Wir sind noch nicht im Protected Mode! Also 16 BIT benutzten


GLOBAL _enterProtectedMode          ;*** Real Mode Funktionen ***
GLOBAL _setUpInterruptHandler
GLOBAL _CopyIntoMemory          

GLOBAL _setFPUPM   

EXTERN _putchar
EXTERN _main
EXTERN _reboot
EXTERN __endofdata
EXTERN __startofdata

EXTERN _main                        ; Der C Kernel (ProtMode)
EXTERN _StartProtectedMode          ; C Funktion (in ProtMode.c)
EXTERN _exc0                        ; Die Exception Handler in der exceptions.c
EXTERN _exc1
EXTERN _exc2
EXTERN _exc3
EXTERN _exc4
EXTERN _exc5
EXTERN _exc6
EXTERN _exc7
EXTERN _exc8
EXTERN _exc9
EXTERN _exc10
EXTERN _exc11
EXTERN _exc12
EXTERN _exc13
EXTERN _exc14
EXTERN _exc15
EXTERN _exc16
EXTERN _excxx

GLOBAL _messageDescriptorSetup      
GLOBAL _sys16
GLOBAL _messageOK                   ; Zeigt "OK" auf den Bildschirm an

; **** EINSPRUNGPUNKT ****
        CLI                         ; Setzen des Stacks
        mov eax,0x7000
        mov ss,eax
        mov sp,0xFFFF
        mov ah, 0x00
        mov dl, 0x80
        int 0x13
        call _messageOK             ; Kernel wurde gestartet - anzeigen von "OK"!          
        call createds               ; Erstellt das Daten-Segment für den Protected Mode   

        STI
         jmp _StartProtectedMode    ; In die ProtMode.c jumpen
         
         
_sys16:
pusha
xor ax, ax
.ka:
cmp ax, 0x16
jnz .ka
popa
ret
         
;----------------------
; createds
; Diese Funktion sollte später mit dem Paging wegfallen!
; Written & Comments by hornet

createds:
        mov esi, adrCreatDSstring    ; Ausgeben des Textes "Creating Data-Segment: "
        push ds
        mov eax, 0x8000
        mov ds, ax
        call message
        pop ds
        pusha 
        mov ax, 0x8000                       ; Der Start des Data-Segments im Image wird durch das Symbol __startofdata
        mov ds, ax                           ; markiert-- Das Data-Segment wird nun nach 0x2000:0x0000 kopiert
        mov esi, __startofdata               ; Darauf wird ein PM Deskriptor gesetzt.
        mov ax, 0x3000
        mov es, ax
        mov eax, __endofdata
        sub eax, esi
        mov ecx, eax
        mov di, 0x0000
        rep movsw                    ; Kopieren vom DS Bytes
        popa
        call _messageOK             ; Ausgeben von OK
        mov ax, 0x3000               ; Datensegment setzten 
        mov ds, ax
ret

_setFPUPM

    fsetpm
    ret

;------------------------
; _messageDescriptorSetup
; Zeigt den String "Setting Up Descriptors" an und springt f die nächste Zeile
; String wird im Real Mode im Teleteype Mode geschrieben
                
_messageDescriptorSetup:
                pusha
                mov esi, adrsDescSetstring
                mov ax, 0x8000
                mov ds, ax
                call message
                popa
                mov ax, 0x3000
                mov ds, ax
                ret

;------------------------
; _messageOK
; Zeigt den String "OK" an und springt in die nächste Zeile
; String wird im Real Mode im Teleteype Mode geschrieben

_messageOK:
            pusha           ; Sichern
            mov esi, adrOKstring
            mov ax, 0x8000
            mov ds, ax      ; DS: SI = Adresse des Strings
            call message    ; Schreiben
            popa            ; Wiederherstellen
            ret


;------------------------
; CopyIntoMemoryRM
; Kopiert einen Buffer in den Speicher - vor dem Protected Mode
; bekannt unter CopyIntoMemoryRM

_CopyIntoMemory:
            push bp	        	 ; Standart Teil. kA warum	      						
            mov bp,sp			 ; Standart Teil. kA warum	
	        sub sp,0x40     
	        xor ax, ax
            mov ax,[bp+0x0a]       ; BlockNr. in ax
            mov DI, ax          ; von DS:SI wird gelesen
            mov dx, es          ; ES wird in DX gesichert
            mov ax,[bp+6]     	; AX = Start-Lese Position
            mov si, ax          ; nach ES:DI wird geschrieben
            
                mov ax,[bp+0x12]       ; ax = Addresse im Speicher 0x1000 * ax - Segment
                inc ax
                mov cx, 0x1000
                xor dx, dx
                mul cx
                mov es, ax
            
            mov ax,[bp+0xE]
            mov cx, ax          ; Blockgröße
            CLD                 ; DF = 0
            rep movsb           ; Speicher wird geschrieben
            mov ax, cs
            mov es, ax
   	        mov sp,bp            ; Standart Teil
	        pop bp               ; Standart Teil
	        ret 2

;----------------------
; Setting Up Interrupt Descript Table
; Diese Funktion erstellt das Interrupt Handler Segment (Interrupt Handler Segment: Selektor 24), 
; welches die Interrupt bzw. Exceptions Handler enthält. Dazu werden die einzelnen Routinen kopiert,
; Written & Comments by hornet
_setUpInterruptHandler:       

            call copyIDT            ; Routinen bis Interrupt 20
            
            ;*****************************************
            ;***** Die eigenen Interrupt Routinen ****
            ;*****************************************
            
            ;**** Interrupt 0x20 ****
            mov di, 0x2000 ; (0x20 * 100byte)
            mov ax, cs          ; Interrupt20 nach 0x60000 (64KB Interrupt Handler Segment) kopieren
            mov ds, ax
            mov ax, 0x6000
            mov es, ax
            mov cx, 0x100       ; Interrupt Handler darf höhsten 256 BYTE groß sein!!!
            mov si, interrupt20 ; Position des Handlers 
            rep movsw           ; Kopieren des Codes
            
            ;**** Interrupt 0x20 ****
            mov di, 0x2100 ; (0x29 * 100byte)
            mov ax, cs          ; Interrupt20 nach 0x60000 (64KB Interrupt Handler Segment) kopieren
            mov ds, ax
            mov ax, 0x6000
            mov es, ax
            mov cx, 0x100       ; Interrupt Handler darf höhsten 256 BYTE groß sein!!!
            mov si, interrupt21 ; Position des Handlers 
            rep movsw           ; Kopieren des Codes
            
            ;**** Interrupt 0x26 ****
            mov di, 0x2200 ; (0x26 * 100byte)
            mov ax, cs          ; Interrupt26 nach 0x60000 (64KB Interrupt Handler Segment) kopieren
            mov ds, ax
            mov ax, 0x6000
            mov es, ax
            mov cx, 0x100       ; Interrupt Handler darf höhsten 256 BYTE groß sein!!!
            mov si, interrupt26 ; Position des Handlers 
            rep movsw           ; Kopieren des Codes


            ;**** Interrupt 0x50 ****
            mov di, 0x2300 ; (0x50 * 100byte)
            mov ax, cs          ; Interrupt20 nach 0x60000 (64KB Interrupt Handler Segment) kopieren
            mov ds, ax
            mov ax, 0x6000
            mov es, ax
            mov cx, 0x100       ; Interrupt Handler darf höhsten 256 BYTE groß sein!!!
            mov si, interrupt50 ; Position des Handlers 
            rep movsw           ; Kopieren des Codes
            ret 2

;----------------------
; Starten des Protected Modes
; Es werden die letzten Vorbereitungen getroffen und zum Schluss wird der PM aktiviert
; Written & Comments by hornet
_enterProtectedMode:
            push bp	        	 ; Standart Teil. kA warum	      						
            mov bp,sp			 ; Standart Teil. kA warum	
	        sub sp,0x40     
	        xor ax, ax
            mov eax,[bp+0x06]       ; BlockNr. in ax
            mov [adrBCK], ax
            xor ax, ax            
                
                ; *** Neuprogrammierne des Interrupt Controllers.
                call msgReCodingIC
                CLI                
                ; Neuinit der Interrupt-Controller, sowie neu Setzen des
                ; Interrupt Mappings
                           	mov al,00010001b    ; kaskadierte Interrupt-Controller
                           	out 020h,al         ; und Flankentriggerung an beider Controller
                           	out 0a0h,al
        call __wait
                 ; neuer IRQ-Vektor für den ersten Controller einstellen
                           	mov al,0x20          ; neuer Wert => IRQ0..7 -> INT20h..27h
                           	out 021h,al

                 ; neuer IRQ-Vektor für den zweiten Controller einstellen
                	        mov al,0x28          ; neuer Wert => IRQ8..15 -> INT28h..2Fh
                	        out 0a1h,al

        call __wait
                  ; Kaskadierung beider Controller einstellen
	                        mov al,00000100b    ; Kaskadierung über IRQ2
	                        out 021h,al
	                        mov al,00000010b    ; Kaskadierung über IRQ2
	                        out 0a1h,al
        call __wait
                    ; PIC auf Intel-Umgebung und manuelle Int.- Beendigung einstellen
                            mov al,00000001b
                                               out 021h,al
                           out 0a1h,al
	
        call __wait
    
                   call msgOK                               ; IC wurde umprogrammiert
               

                call msgLoadingGDT  
    		    mov ax, 0x5000 ; Setzten auf das PMSS
		        mov ds, ax

                LGDT [0]            ; Laden des GDT-Zeigers
                CLI                 ; Interrupts aus
                call msgOK

; ****  Starten des Protected Mode *****
		        call msgStartingPM		
		        call setPMCursor ; Die aktuelle Cursor-Pos. wird für den PM gespeichert
		        ; **** Setzten des Timers auf 100Hz *****

		        CLI  
                mov bx, [adrBCK]
                mov ax, 0x5000  ; DS auf PMSS 
                mov es, ax
                mov ds, ax

		            ; PIC für Zähler 0 auf 100 HZ initialisieren
		        cli
		        xor ax, ax
		        mov al,36h
        	    out 43h,al
   		        xor ax, ax
		        mov al,9Bh
		        out 40h,al
     	        mov al,2Eh
		        out 40h,al

LIDT [BX]     ; Laden des IDT-Zeigers
                                             
    ; Protected Mode aktivieren
                mov eax,cr0
                or eax,1
               mov cr0,eax
               
                mov ax, 0x40
                mov ds, eax    
                mov eax, 0x38
                mov gs, eax 
                mov eax,0x20          ; Setzten des Stacks
                mov ss,eax
                mov eax, 0xFFFFF
                mov sp, ax
                mov bp, ax

   ; "FAR-JMP" zu Selektor:Offset (entspricht jmp 8:Main)


                STI 
                db 0x66                      
                db 0eah 
                dd _main                ; JUMP zum richtigen 32 BIT Kernel 
                dw 8  

             
; *** Eine Funktion die ein wenig Zeit totschlägt ***
__wait:

        push ecx
        mov ecx,4096
.here:  loop .here
        pop ecx
        ret 
              
              
;*********************
; Diese Funktion speichert die aktuelle Cursor-Position
; für den Protected Mode
;*********************
setPMCursor: 
            pusha
            mov ah, 0x03
            mov bh, 0x00
            int 0x10            ; Auslesen der Position
            xor ax, ax          ; Unmwandeln der Position in PM-Position
            xor cx, cx
            mov al, dh
            mov cl, dl
            xor dx, dx
            mov bx, 0x50
            mul bx
            add ax, cx
            mov bx, 0x02
            mul bx
            mov cx, 0x4000      ; 0x4000 = Nova-X System Segment
            mov gs, cx
            mov [gs:0], ax      ; Speichern der Position
            popa
            ret 


;******* Kopiert die Standart Interrupt Routinen in das Interrupt Handler Segment
copyIDT:
            xor bx, bx  
.loop: 
            mov ax, cs  ; Codebereich kopieren
            mov ds, ax
            mov ax, 0x6000
            mov es, ax
            mov ax, 0x100
            xor dx, dx
            mul bx
            mov di, ax
            mov cx, 0x27
            cmp bx, 0x00
            jnz .il1
            mov si, Except0
            jmp .end
.il1        cmp bx, 0x01
            jnz .il2
            mov si, Except1
            jmp .end
.il2        cmp bx, 0x02
            jnz .il3
            mov si, Except2
            jmp .end
.il3        cmp bx, 0x03
            jnz .il4
            mov si, Except3
            jmp .end
.il4        cmp bx, 0x04
            jnz .il5
            mov si, Except4
            jmp .end
.il5        cmp bx, 0x05
            jnz .il6
            mov si, Except5
            jmp .end
.il6        cmp bx, 0x06
            jnz .il7
            mov si, Except6
            jmp .end
.il7        cmp bx, 0x07
            jnz .il8
            mov si, Except7
            jmp .end
.il8        cmp bx, 0x08
            jnz .il9
            mov si, Except8
            jmp .end
.il9        cmp bx, 0x09
            jnz .il10
            mov si, Except9
            jmp .end
.il10       cmp bx, 0x0A
            jnz .il11
            mov si, Except10
            jmp .end
.il11       cmp bx, 0x0B
            jnz .il12
            mov si, Except11
            jmp .end
.il12       cmp bx, 0x0C
            jnz .il13
            mov si, Except12
            jmp .end
.il13       cmp bx, 0x0D
            jnz .il14
            mov si, Except13
            jmp .end
.il14       cmp bx, 0x0E
            jnz .il15
            mov si, Except14
            jmp .end
.il15       cmp bx, 0x0F
            jnz .il16
            mov si, Except15
            jmp .end
.il16       cmp bx, 0x10
            jnz .ilxx
            mov si, Except16
            jmp .end
.ilxx     
            mov si, Exceptxx
            jmp .end
        
.end    
             rep movsw
             inc bx
             cmp bx, 0x20
             jnz NEAR .loop
ret

; *************************************
; Exception-Handler!
; *************************************
Except0:                        ; ******** EXCEPTION 0
                CLI
                push dword 0
                db 0x66                      
                db 0eah 
                dd _exc0       ; far jmp zum C Exception Handler
                dw 8  
                iretd

Except1:                        ; ******** EXCEPTION 1
                CLI
                push dword 0
                db 0x66                      
                db 0eah 
                dd _exc1       ; far jmp zum C Exception Handler
                dw 8  
                iretd     
                
                                      
Except2:                        ; ******** EXCEPTION 2
                CLI
                push dword 0
                db 0x66                      
                db 0eah 
                dd _exc2       ; far jmp zum C Exception Handler
                dw 8  
                iretd                

Except3:                        ; ******** EXCEPTION 3
                CLI
                push dword 0
                db 0x66                      
                db 0eah 
                dd _exc3       ; far jmp zum C Exception Handler
                dw 8  
                iretd                
                
Except4:                        ; ******** EXCEPTION 4
                CLI
                push dword 0
                db 0x66                      
                db 0eah 
                dd _exc4       ; far jmp zum C Exception Handler
                dw 8  
                iretd                

Except5:                       ; ******** EXCEPTION 5
                CLI
                push dword 0
                db 0x66                      
                db 0eah 
                dd _exc5       ; far jmp zum C Exception Handler
                dw 8  
                iretd                
                
Except6:                       ; ******** EXCEPTION 6
                CLI
                push dword 0
                db 0x66                      
                db 0eah 
                dd _exc6       ; far jmp zum C Exception Handler
                dw 8  
                iretd                
                
Except7:                       ; ******** EXCEPTION 7
                CLI
                push dword 0
                db 0x66                      
                db 0eah 
                dd _exc7       ; far jmp zum C Exception Handler
                dw 8  
                iretd                

Except8:                       ; ******** EXCEPTION 8
                CLI
                push dword 0
                db 0x66                      
                db 0eah 
                dd _exc8       ; far jmp zum C Exception Handler
                dw 8  
                iretd                
               
Except9:                       ; ******** EXCEPTION 9
                CLI
                push dword 0
                db 0x66                      
                db 0eah 
                dd _exc9       ; far jmp zum C Exception Handler
                dw 8 
                iretd                
                
Except10:                       ; ******** EXCEPTION 10
                CLI
                push dword 0
                db 0x66                      
                db 0eah 
                dd _exc10       ; far jmp zum C Exception Handler
                dw 8  
                iretd                

Except11:                       ; ******** EXCEPTION 11
                CLI
                push dword 0
                db 0x66                      
                db 0eah 
                dd _exc11       ; far jmp zum C Exception Handler
                dw 8  
                iretd                

Except12:                       ; ******** EXCEPTION 12
                CLI
                push dword 0
                db 0x66                      
                db 0eah 
                dd _exc12       ; far jmp zum C Exception Handler
                dw 8  
                iretd                

Except13:                       ; ******** EXCEPTION 13
                CLI
                push dword 0
                db 0x66
                db 09ah 
                dd _exc13       ; far jmp zum C Exception Handler
                dw 8  
                iretd                

Except14:                       ; ******** EXCEPTION 14
                CLI
                push dword 0
                db 0x66                      
                db 0eah 
                dd _exc14       ; far jmp zum C Exception Handler
                dw 8  
                iretd                

Except15:                       ; ******** EXCEPTION 15
                CLI
                push dword 0
                db 0x66                      
                db 0eah 
                dd _exc15       ; far jmp zum C Exception Handler
                dw 8                  
                iretd                

Except16:                       ; ******** EXCEPTION 16
                CLI
                push dword 0
                db 0x66                      
                db 0eah 
                dd _exc16       ; far jmp zum C Exception Handler
                dw 8                  
                iretd                

Exceptxx:                       ; ******** EXCEPTION xx
                CLI
                push dword 0
                db 0x66                      
                db 0eah 
                dd _excxx       ; far jmp zum C Exception Handler
                dw 8                  
                iretd        
;*************
; Interrupt Handler 0x20!
; Der Timer Interrupt
; Dieser Interrupt wird 100x in der Sekunde aufgerufen.
; Und ist dem Real Mode Timer sehr ähnlich
; Addresse 0x38:0x100 ist wichtig!
;**************

interrupt20:
        pusha
        mov ax, 0x38
        mov gs, ax
        mov eax, [gs:0x100]     ; Erhöhen des Zählers, und speichern
        inc eax
        mov [gs:0x100], eax
        mov al, 0x20            ; IRQ ist zu ende (muss da hin :) )
        out 0x20, al
        popa
        iretd
        
;*************
; Interrupt Handler 0x21!
; Der Keyboard Interrupt
; Dieser Interrupt wird bei einem Tastendruck aufgerufen.
; Es wird dabei der Scancode (make oder brake Code) ausgelesen,
; und in den Tastaturpuffer geschrieben
; Der Keyboard-Buffer sieht anders aus, als bei Windows!
; Addresse 0x38:0x100 ist wichtig!
;**************
interrupt21:
        pusha
        mov ax, 0x38        ; Vorbereiten
        mov gs, ax
        xor ax, ax
        mov al, [gs:4]      ; Anzahl der Scancodes im Keybboard Buffer 0x38:0x04
        add ax, 0x05
        cmp ax, 0x85        ; Ist der Kebboard-Buffer voll (85 Codes) ??
        jae .fullbuffer     
        inc ax              
        mov bx, ax
        mov dx, 0x60
        in al, dx           ; Es wird der Scancode ausgelesen!
        mov [gs:bx], al     ; Scancode wird in den Buffer geschrieben
        sub bx, 0x05
        mov [gs:4], bx      ; 0x38:0x04 (Anzahl der Scancodes) wird geupdated.

        .fullbuffer:
 ;       /* TODO (hornet#1#): Beep Funktion - Wenn Tastaturpuffer voll ist */
        
        jmp .end
        
        .end:
        mov al, 0x20        ; IRQ beendet
        out 0x20, al
        popa
        iretd

;*************
; Interrupt Handler 0x50!
; Der Print Interrupt
; Dieser Interrupt soll den Real Mode Interrupt 0x10 ersetzen!
;**************
interrupt50:         
CLI
                cmp ah, 0x09        ; 0x09 Zeichen ausgeben
                jz .putchar
                cmp ah, 0x02        ; Cursor Setzten
                jz .setcursor
                cmp ah, 0x03
                jz .readcursor       ; Cursor auslesen
                jmp .end             ; Kein passender AH Wert gefunden
                                
.readcursor:    mov bx, 0x38
                mov gs, bx
                mov ax, [gs:0] ; Cursor ist die Aktuelle Cursorpistion, dargestellt als Zeiger in den VideoRAM
                xor dx, dx
                mov bx, 0xA0    ; 160 = 2 * 80. 2 da Zeichen aus ASCII Code und Attrbut besteht. 80 Zeichen / Zeile
                div bx
                mov cx, ax      ; CX = AX = Zeilenposition
                mov ax, dx                   
                xor dx, dx
                mov bx, 0x02
                div bx          ; AL = AX = Spaltenposition

                mov dh, cl      ; Übergabe der Werte in DL, DH
                mov dl, al
                jmp .end

.setcursor:     mov bx, 0x38
                mov gs, bx
                mov bx, 0xA0    ; Funktionsweiße siehe. readcursor
                xor ax, ax      ; gewünschte Position in DH und DL
                mov al, dh
                xor cx, cx
                mov cl, dl
                xor dx, dx
                mul bx
                add ax, cx
                add ax, cx
                mov [gs:0], ax ; Cursor-Pos wird upgedatet
                jmp .end

.putchar:       
                mov ah, bl
                mov bx, 0x10          ; Video RAM Selektor
                mov es,bx   
                mov di, [gs:0] ; Welche Position soll geschrieben werden
                stosw
.end:           sti          
                iretd


;Floppy IRQ6, wenn ausgelöst dann wird bei 0x38:0x105 1 eingetragen
interrupt26:
        PUSHA
        mov ax, 0x38
        mov gs, ax
        mov byte [gs:0x199], 1
        mov al, 0x20
        out 0x20, al
        POPA
        iretd


;********************************************************************************
; MESSAGE Routinen
;********************************************************************************

msgReCodingIC:                                ; Gibt den String "Reprogramming Interrupt Controller" aus
                pusha
                mov si, adrReCodingIC
                mov ax, 0x8000
                mov ds, ax
                call message
                popa
                ret                

msgLoadingGDT:                      ; Gibt den String "Loading GDT" aus
                pusha
                mov si, adrLoadingGDT
                mov ax, 0x8000
                mov ds, ax
                call message
                popa
ret

msgOK:                                ; Gibt den String "OK" aus
                pusha
                mov si, adrOKstring
                mov ax, 0x8000
                mov ds, ax
                call message
                popa
                ret

msgStartingPM:                         ; Gibt den String "Switching to Protected Mode" aus
                 pusha
                 mov si, adrStartingPM
                 mov ax, 0x8000
                 mov ds, ax
                 call message
                 popa
                 ret

message:        ; Gibt einen String an Adresse DS:SI aus.
        lodsb                   ;Nächstes Zeichen laden 
        or al,al                ;Falls String zu ende ist
        jz FINISHED             ; "
        mov ah,0x0E             ; BIOS Teletype-Modus
        mov bx,0x0007           ; Farbe setzten
        int 0x10                ; Schreiben
        jmp message
FINISHED:
        ret


adrReCodingIC db 'Reprogramming Interrupt Controller : ', 0x00   
adrLoadingGDT db 'Loading GDT                        : ', 0x00
adrStartingPM db 'Initializing PIC-Timer and PM      : ', 0x00
adrCreatDSstring     db 'Creating Data-Segment              : ', 0x00   
adrsDescSetstring    db 'Setting Up Descriptors             : ', 0x00
adrOKstring          db 'OK',0x0D, 0x0A,0x00
adrBCK db 0x00, 0x00, 0x00, 0x00

