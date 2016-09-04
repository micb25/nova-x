;----------------------
; boot.asm for Nova-X with xFS
; by xOS-Dev-Team
; thx to Matthew Vea & Scott Billingsley
; comments are in german
; by Vitali 'Hornet' Mueller
;----------------------

BITS 16              ; Ist alles 16 Bit
org	0x00             ; Hier fängt alles an

jmp 07C0h:starter         ; Goto segment 07C0
starter:
jmp begin_boot          ; Den Data-Bereich überspringen

OEM_ID                  db 'nova-x01'           ; Identifikation

;----       
; Daten-Area
;----
msgBoot         db 'Bootsector      : OK', 0x0D,0x0A,0x00          ; Bootmessages
msgKernel       db 'Loading Kernel  : ', 0x00          ; 
msgstart        db 'Starting Kernel : ', 0x00          ; 
msgOK           db 'OK', 0x0D,0x0A,0x00          ; 
aktsektor       dw 0x0000                              ; Zähler der gelesen Sektoren (gesamt)
disksektor      dw 0x0000                              ; relativer Sektor auf der FD
disktrack       dw 0x0000                              ; relativer Track auf der FD
diskhead        dw 0x0000                              ; relativer Head auf der FD
dataentry       dw 0x0000                              ; Pos. zum nächsten DATA-Segment
highestbx       db 0x0000                              ; Schlussposition im Memory
bootfile        db 'system.bin',0x00                   ; Diese Datei wird gebootet
msgProgress     db ".",0x00                            ; kleiner Effekt :)
msgFailure      db "ERROR! Press any key to reboot..",0x00 ; Falls ein Fehler auftritt

        
;---- Hier gehts eigentlich los
begin_boot:
        cli                     ;Interrupts ausschalten während wir das hier setzen
            mov ax, cs
            mov ds, ax
            mov es, ax

        mov gs,ax
        mov ax, 0x8000
        mov fs,ax

;----- Einen Stack erstellen ----
        mov ax,0x7000
        mov ss,ax
        mov sp,0xFFFF
        sti                    ; Interupts aktivieren
        
        mov si,msgBoot         ; Bootmessage schreiben                    
        call message
        
        call resetdisk         ; Disk resetten
        mov si, msgKernel
        call message
        Call getbootfile       ; 2. Sektor wird gelesen
        
        call checksystem       ; 2. Sektor wird verifiziert
              
        call getsystemsys      ; Schreiben der komplette Bootfile in den Memory
        call nopinfos          ; Die 9 byte großen Data-Entrys werden entfernt
        mov ax, 0x7C0
        mov ds, ax
        mov si, msgOK
        call message
        mov si, msgstart
        call message
        
        mov ax, fs              ; setzten der Addresse von system.sys und jumpe
        push ax 
        xor ax, ax
        push ax

        RETF  ;<--- Jumpen zur system.sys

;------
; Funktion Message
; Zeigt einen ASCii String über das Bios vond er Position ES:DI an
;------
message:    
        lodsb                   ;Nächstes Zeichen laden 
        or al,al                ;Falls String zu ende ist
        jz FINISHED             ; "
        mov ah,0x0E             ; BIOS Teletype-Modus
        mov bx,0x0007           ; Farbe setzten
        int 0x10                ; Schreiben
        jmp message
FINISHED:
        ret
 
;--- Entfernen der Data-Entrys
nopinfos: 
          mov ax, fs
          mov ds, ax
          mov es, ax
          mov si, 0x09
          mov ax, [highestbx]
          add ax, 0x1000
          CLD
          xor di, di
.bloop:   mov cx, 0xFF7
          rep movsb 
          add si, 0x09
          mov ax, 0xF000       ; ACHTUNG !!! DAS MUSSS VERBESSERT WERDEN!!!!
          cmp si, ax           ; Ende der system.sys
          jb .bloop            ; wenn nicht, dann jumpe
          ret
          
          
          


checksystem: 
             mov bx, 0x09       ; ES:BX wurde auf beginn des Dateinamens gesetzte
.nextchar:   mov ax, [es:bx]    ; In dieser Schleife wird überprüft, ob es die bootfile
             push ax            ; in Sektor 2 ist
             mov dx, bootfile
             add dx, bx
             sub dx, 9
             mov ax, dx
             mov cx, [bootfile + bx - 9]  
             inc bx
             pop ax
             cmp cl, al
             jz .nextchar
             or al, al      ; Ist Dateiname zu ende
             jz .end
             jmp bootfail      ; Dateientspricht nicht der bootfile
.end:        mov bx, 0x111     ; Hohlen des Data-Sektors / Dataentrys
             mov ax, [es:bx]
             inc ax 		; AX erhöhen,da Bootsektor nicht mitgezählt wird
             mov [dataentry], ax
             xor  bx, bx
             ret

;---- resettet den Disk-Controller
resetdisk:
        pusha
        xor ah, ah
        xor dl, dl
        int 0x13           ;Disk-Controller resetten       
        popa
        ret

;------ Dataentry-Wert wird in eine Position auf der Diskette umgewandelt
configint: mov ax, [dataentry] ; Dataentry ist eine Position in der disk.img FIle
           mov cx, 0x12        ; Eine Diskette hat 12h = 18 Sektoren pro Spur  
           xor dx, dx
           div cx              ; Dataentry / 18 = AX Rest DX(=Sektornummer)
           or dx, dx        ; Es gibt Sektornummer 0 nicht,..
           jnz g_1  
           mov dx, 0x12        ; Wenn Sektornummer 0 dann stelle um auf Sektor-Nr. 12
           dec ax              ; und Head - 1
g_1:       mov [disksektor], dx ; Sektornummer wurde berechnet ->Sichern der Sektor-Nr.
           mov cx, 0x02        ; Herausfinden der Head nummer
           xor dx, dx
           div cx               ; AX / 2 = AX(=Tracknummer) Rest DX(=Headnummer)
           mov [diskhead], DX   ; AH = Diskettenseite
           mov [disktrack], AX  ; AL = Tracknummer
           ; Setzten von ES:BX - hier wird der Disketten-Sektor hingeschrieben
           mov cx, 0x0200
           mov ax, [aktsektor] ; Ein Sektor ist 512 Byte groß * Anz. der Sektoren = BX
           xor dx, dx
           mul cx
           xchg bx,ax
        ret
        
;------ In Sektor 2 muss die Bootdatei stehen! datentry vom ersten Block auslesen
getbootfile:
        mov ax, fs
        mov es, ax
        xor bx, bx    		; ES:BX wurde gesetzt 	
        mov ax, 0x0201		; Befehl lesen
        mov cx, 0x02        ; Starten bei Sektor 2
        xor dx, dx
        int 0x13            ; Es wird gelesen, Bereich ist in 8000:0 zu finden
        xor ah, ah          ; War lesen erfolgreich?
        jnz bootfail        ; wenn nicht, dann Bootvorgang abbrechen
        mov al, [es:bx]     ; Checken ob Sektor ein tFileEntry Sektor ist
        cmp al, 0x01
        jnz bootfail
        ret


;------ Bootvorgang wird abgebrochen
bootfail:mov si, msgFailure        ; Fehlermsg wird geschrieben
         call message
         xor ax, ax
         int 0x16                  ; warten auf Tastendruck
         INT 0x19                  ; Reboot
         ret

;---- Liest die system.sys in den Speicher
getsystemsys: 
        mov [aktsektor], byte 0x0000   ; Starten bei Sektor 0
              
.sektorreads:       
        ;call resetdisk        ; Disk resetten, damit alles gut geht
        call configint        ; Berechnen der Position auf der FD
        mov ax, 0x0201	      ; Befehl "Read Sector into Memory"
        mov ch, [disktrack]   ; Optionen setzten       
        mov cl, [disksektor] 
        mov dh, [diskhead]             
        xor dl, dl        ; Device Nr. = Diskettenlaufwerk
        int 0x13            ; Von Diskette lesen
                
        or ah, ah             ; War lesen erfolgreich?
        jnz bootfail            ; Wenn nicht, fail

        mov cx, [dataentry]  ; Dataennry wird erhöht, es soll 1 Segment gelesen werden
        inc cx               
        mov [dataentry], cx

        mov cx, [aktsektor]
        inc cx               ; Erhöhen von aktsektor
        mov [aktsektor], cx 
        mov ax, 0x08         ; Überprüfen ob ganzes Segment (8 Sektoren) eingelesen wurde
        xchg ax, cx
        xor dx, dx
        div cx
                
        or dx, dx        ; Wurde ganzes Segment eingelese?
        jnz .sektorreads    ; Wenn nicht: nächsten Sektor
               
        xor ah, ah        ; Bestimmen des nächsten Data-Blocks / Dataentrys
        dec ax
        mov cx, 0x1000
        xor dx, dx
        mul cx
        add ax, 5
        mov bx, ax
        
        mov ax, [es:bx]
        inc ax
        mov [dataentry], ax   ; Setzten des nächsten Data-Blocks
        
        mov ax, 0x01
        cmp [dataentry], ax    ; Wurde letzter Data-Block gelesen, dann ende
        jnz .sektorreads

        ret
        
	db	0x55, 0xAA		;2  byte boot signature
        
                 
