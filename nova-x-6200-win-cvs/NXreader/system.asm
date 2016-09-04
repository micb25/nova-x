; INFOS ÜBER INTERRUPTS GIBT ES HIER: http://www.clipx.net/ng/bios/ng1d0.php


[BITS 32]
SEGMENT 

;-------------------------------------------------------------------------------
; Share API 
;-------------------------------------------------------------------------------

         			               ; Hier werden die exportierbaren Funktionen angegeben. u.a. die C-APIs
     	               ; für eine Beschreibung der APIs, die extra Doku lesen   
GLOBAL _reboot  
GLOBAL _graph
GLOBAL _text
GLOBAL _scanf
GLOBAL _cls
GLOBAL _strdef
GLOBAL _CPUInfo
GLOBAL _gotoxy
GLOBAL _textcolor
GLOBAL _textbackground
GLOBAL _getch
GLOBAL _getbasememsize
GLOBAL _getcmostime
GLOBAL _getcmosdate
GLOBAL _inttostr
GLOBAL _getextmemsize
GLOBAL _getkeyboardrate
GLOBAL _getkeyboarddelay
GLOBAL _biosdisk
GLOBAL _chrtostr
GLOBAL _CopyIntoMemory
GLOBAL _ReadFromMemory
GLOBAL _RunCodeFromMemory
GLOBAL _shutdown
GLOBAL _wherex
GlOBAL _wherey
GLOBAL _quit

EXTERN _main

	     			                   	; Das passiert, sofort nach dem Bootvorgang!
            mov si, textcolorm          ; textcolorm enthält die Vor+Hintergrundfarbe! 
            mov ax, 0x07	          	; z.B. "17" -> 1 = Hintergrundfarbe ; 7 = Vordergrundfarbe
            mov [si], ax                ; speichern der Farbe!
            jmp _main                   ; Nova-X wird gestartet

;-------------------------------------------------------------------------------
; Nova-X API
;-------------------------------------------------------------------------------

;----------------------
; Ruft Interrupt 25 auf.
; Int 25 kehrt zu Nova-X zurück
; Written & Comments by hornet
 
_quit:
        INT 0x25
        ret
;----------------------
; Erstellt die Nova-X Interrupts
; Die Interrupt Vektor Tabelle (IDT) wird modifiziert.
; Written & Comments by hornet

_creatinginterrupts:
                CLI                    ; Interrupts sperren
                XOR AX,AX
                mov bx, ds
                MOV DS,AX              ; Segmentadresse der Interrupt-Vektor-Tabelle

                ; Interrupt Vektor Tabelle schreiben
                
                ; Interrut 25 : Rückkehr zu Nova-X
                mov si, 0x94           ; [4*25h] = 94h 
                mov ax, returntoNX  
                MOV [DS:SI], ax
                mov ax, SEG returntoNX
                mov si, 0x096          ; [4*25+2] = 96h
                MOV [DS:SI],CS
                ; ----
                mov ds, bx
                STI                     ; Interrupts wieder zulassen
                RET


;----------------------
; Hält die CPU an.
; Bessere Lösung in Arbeit.
; Written & Comments by thesame
 
_shutdown:
  cli
  hlt
  ret

;----------------------
; Springt zum Memoryberreich 0x1000 * x, wo sich normalerweiße
; durch LoadFileIntoMemory in den Speicher geladene Dateien befinden
; Written & Comments by Hornet

_RunCodeFromMemory:
                mov ax, sp
                mov [savedstack], ax    ; Stack wird in savedisk gespeichert
                push bp	        	 ; Standart Teil. kA warum	      						
                mov bp,sp			 ; Standart Teil. kA warum	
	            sub sp,0x40     
	        
                mov ax,[bp+4]       ; ax = Addresse im Speicher 0x1000 * ax - Segment
                inc ax
                mov cx, 0x1000
                xor dx, dx
                mul cx
      	        mov sp,bp            ; Standart Teil
     	        pop bp               ; Standart Teil
                mov es, ax
                mov ds,ax
                push ax 
                mov ax,0x00
                push ax
                RETF                    ; <--- Jumpen zur Addresse
                
;------------------------
; ReadFromMemory
; Kopiert einen Memory-Bereich in einen Puffer
; siehe Dokumentation und "CopyIntoMemory"

_ReadFromMemory:
            push bp	        	 ; Standart Teil. kA warum	      					
            mov bp,sp			 ; Standart Teil. kA warum	
	        sub sp,0x40     
            pusha                ; Vorrsichtshalber alles Sichern

            mov ax, [bp+8]
            mov di, ax            ; DI = Addresse vom Buffer
            
                mov ax,[bp+0x04]       ; ax = Segment-Addresse im Memory
                inc ax
                mov cx, 0x1000
                xor dx, dx
                mul cx
                mov dx, ds             ; DS wird gesichert in DX!
                mov ds, ax             ; DS = Segmendaddresse
                    
            mov ax, [bp+6]
            mov SI, ax            ; SI = Offset im Memory


            mov ax, [bp+0x0A]
            mov cx, ax            ; SI = Offset im Memory

            mov ax, 0x1000
            mov es, ax
            CLD                 ; DF = 0
            rep movsb           ; Aus dem Speicher wird gelesen... DS:SI -> ES:DI
            mov ds, dx          ; Sicherung wird wiederhergestellt
            popa       
   	        mov sp,bp            ; Standart Teil
	        pop bp               ; Standart Teil
	        ret

;----------------------
; Durch Int 25 ausgelöste Funktion
; die zu Nova-X zurückkehrt
; Written & Comments by Hornet
        
returntoNX:         
                mov ax, 0x8000     ; setzten der Seg. Addresse von system.bin
                mov es, ax
                mov ds, ax
                push ax            
                mov ax, .exit     ; Setzten von Offeset
                push ax
                RETF               ; <--- Jumpen zu Nova-X
        
                .exit: 
                mov ax, [savedstack]   ; Stack wiederherstellen
                mov sp, ax  
                call _text             ; Keyboard-Buffer löschen    
                mov ah, 0x01       
                int 0x16               ; Überprüfen ob ein Zeichen im Buffer liegt
                jnz .deletechar
                jmp .continue
                .deletechar:
                mov ax, 0x00
                int 0x16           ; Zeichen löschen
                jmp .exit
                .continue:
                
                RET                ; Zurück

;------------------------
; CopyIntoMemory
; Kopiert einen Buffer in den Memory-Bereich 0x1000:0000
; siehe Dokumentation

_CopyIntoMemory:
            push bp	        	 ; Standart Teil. kA warum	      						
            mov bp,sp			 ; Standart Teil. kA warum	
	        sub sp,0x40     
	        
            mov ax,[bp+6]       ; BlockNr. in ax
            mov DI, ax          ; von DS:SI wird gelesen
            mov dx, es          ; ES wird in DX gesichert
            mov ax,[bp+4]     	; AX = Start-Lese Position
            mov si, ax          ; nach ES:DI wird geschrieben
            
                mov ax,[bp+0x0A]       ; ax = Addresse im Speicher 0x1000 * ax - Segment
                inc ax
                mov cx, 0x1000
                xor dx, dx
                mul cx
                mov es, ax
            
            mov ax,[bp+8]
            mov cx, ax          ; Blockgröße
            CLD                 ; DF = 0
            rep movsb           ; Speicher wird geschrieben
            mov ax, cs
            mov es, ax
   	        mov sp,bp            ; Standart Teil
	        pop bp               ; Standart Teil
	        ret





;------------------------
; ChrToStr
; Diese Funktion wandelt einen Int Wert in ein String Zeichen um.

_chrtostr : push bp	        	 ; Standart Teil. kA warum	      						textcolor
            mov bp,sp			 ; Standart Teil. kA warum	
	        sub sp,0x40     
	        
            mov dh,[bp+4]     	 ; dh = Integerwert
            mov ax, buffer1
            mov [buffer1], dh     ; IntWert wird in einen Buffer geschrieben
            xor dx, dx            ; Buffer wird mti \0 terminiert
            mov [buffer1+1], dx
            mov si, intf         ; Addresse vom Buffer wird zurückgegebn
            mov dx, ax
	        mov sp,bp            ; Standart Teil
	        pop bp               ; Standart Teil
	        ret


;------------------------
; BiosDisk
; Diese Funktion löst den Interrupt 13 aus. Für detailierte Angaben siehe Nova-X API
; Dokumentation

_biosdisk : push bp	        	 ; Standart Teil. kA warum	      						textcolor
            mov bp,sp			 ; Standart Teil. kA warum	
	        sub sp,0x40     
	        
            mov ah,[bp+4]     	 ; ah = gewünschter Befehl
            mov dl,[bp+6]        ; dl = Geräte-Nr.
            mov dh,[bp+8]        ; dh = Head-Nr.
            mov ch,[bp+0x0A]        ; ch = Track-Nr.
            mov cl,[bp+0x0C]        ; cl = Sektor-Nr.            
            mov al,[bp+0x0E]        ; al = Anz. der zu lesenden Sektoren
            mov bx,[bp+0x10]        ; bx = Addresse, wohin Sektor gelesen wird
            int 0x13             ; Interrupt auslösen
            
            mov al, ah           ; Vorbereitung zur Parameterübergabe (Status)
            mov ah, 0x00
            mov si, ax
            mov dx, ax
	        mov sp,bp            ; Standart Teil
	        pop bp               ; Standart Teil
	        ret

;---------------------
; Ermittelt die Wiederholrate der Tastatur
; Written & Comments by thesame
 
_getkeyboardrate:
  mov ah, 0x03     ;
  mov al, 0x06     ; Wenn Int 16h aufgerufen wird und in al
  int 0x16         ; 6h steht und in ah 3h, gibt dieser Interrupt in bl
  mov al, bl       ; die Wiederholrate zurück. Damit die C-Funktion auch den
  xor ah, ah       ; ermittelten Wert zurückgibt, schreiben wir ihn in die
  mov dx, ax       ; Register ax, dx und si. (xor ah, ah macht das gleiche,wie
  mov si, ax       ; mov ah, 0x00!)
  ret              ; und kehren zum Code zurück.
 
;----------------------
; Ermittelt die Verzögerung der Tastatur
; Written & Comments by thesame
 
_getkeyboarddelay:
  mov ah, 0x03     ;
  mov al, 0x06     ; Wenn Int 16h aufgerufen wird und in al
  int 0x16         ; 6h steht und in ah 3h, gibt dieser Interrupt in bh
  mov al, bh       ; die Anschlags-Verzögerung der Tastatur zurück.
  xor ah, ah       ; Damit die C-Funktion diesen Wert zurück gibt, schreiben wir
  mov dx, ax       ; ihn in ax, dx und si. (xor ah, ah macht das gleiche,wie
  mov si, ax       ; mov ah, 0x00!)
  ret              ; und kehren zum Code zurück.



;------------------------
; Textcolor
; Ich weiß, dieses sieht kompliziert aus. Die Farbe eines Zeichens wird nur durch
; einen Wert bestimmt. z.B. "1C". Dabei ist 1 = die Hintergrundfarbe und C die Textfarbe
; Man muss die Buchstaben nun trennen, indem man schaut wie oft 1C durch 10 teilbar ist (ein mal);
; und dann rechnet man ein mal  10 + gewünschte Hintergrundfarbe.
; Was einfacheres ist mir nicht eingefallen :)

_textcolor: push bp	        	 ; Standart Teil. kA warum	      						textcolor
            mov bp,sp			 ; Standart Teil. kA warum	
	        sub sp,0x40     
            mov si, [textcolorm] ; SI = Colorwert 1. Stelle = Hintergrund;2.Stelle=Textfarbe
            mov ax, si        
            mov dx, 0x10	     ; Durch was soll geteilt werden? = 10h
            div dl      	     ; Nur der akt. Hintergrundwert wird bestimmt -> in AL
            mov ah, 0x00         ; ah muss für mutliplikation 0 sein
            mov dx, 0x10
            mul dl     	         ; aktueller Hintergrundwert multipliziert mit 10h
            mov dx,[bp+4]     	 ; dx = gewünschter Farbwert 
            add ax, dx           ; AX = akt. Hintergrundfarbe mit gewünschter Textfarbe addieren
            mov [textcolorm], ax ; Farbe wird gespeichert.
	        mov sp,bp            ; Standart Teil
	        pop bp               ; Standart Teil
	        ret

;------------------------
; Siehe "auch" _textcolor

_textbackground:     											          ;backgroundcolor
            push bp	  		; Standart Teil. kA warum	      
	    mov bp,sp			; Standart Teil. kA warum	
	    sub sp,0x40     
            mov ax, [textcolorm]   	; SI = Colorwert 1. Stelle = Hintergrund;2.Stelle=Textfarbe
            mov dx, 0x10                ; Durch was soll geteilt werden? = 10h
            DIV dl      	        ; Nur der akt. textcolor-Wert wird bestimmt -> in AH (=Rest)
            mov al, ah                  ; akt. textcolor in Al!
            mov ah, 0h            	; ah muss für mutliplikation 0 sein         
            push ax
            mov dx, 0x10
	    mov al,[bp+4]     	     	; al = gewünschte hintergrundfarbe                      
            MUL dl     	                ; gewünschter Hintergrundwert multipliziert mit 10h 
            pop dx
            Add ax, dx            	; AX = gewünschte Hintergrundfarbe mit Textfarbe addieren
            mov [textcolorm], ax  	; Farbe wird gespeichert.
	    mov sp,bp                   ; Standart Teil
	    pop bp                      ; Standart Teil
	    ret
            

;------------------------
; GetExtMemSize-API
; Gibt den Extended RAM wieder, bis zu einer Größe von 64 MB.
; 

_getextmemsize:
           xor ax, ax
           mov ah, 0x88
           int 0x15 		; RAMgröße in KB in AX
           mov [membuf], ax
           JC .error		; Zur Fehlersuche
           cmp Al, 0x00
           jnz .ok 
           cmp ah, 0x80
           jz .error
           cmp ah, 0x86
           jz .error
           .ok:			; Alles OK
           mov si, [membuf]
           ret
           .error
           xor si, si
           ret

;------------------------
; IntToStr-API
; Komplizierte API :) Wandelt einen Integer-Wert in einen String und übergibt die Addresse
; Funktionsweiße: Hex-Integer Wert wird solange durch 0x0A geteilt, bis der Int kleiner als 0x0A ist.
; Dies ist eine Stelle des Strings. Der Modulwert aus HexIntWert und 0x0A^x wird nun gespeichert und als
; neuer Int Wert angesehen und wieder solange durch 0x0A geteilt.... Dies passiert in einer Schleife.
; Ich glaube es bringt nicht sehr viele, wenn ich hier alles ausführlich beschreiben. Man muss
; den Weg selber kennen. Es gibt auch viele möglichkeiten, das zu machen.


_inttostr: 
           push bp	        	 ; Standart Teil. kA warum	      						
           mov bp,sp			 ; Standart Teil. kA warum	
	   mov dx,[bp+4]    		 ; dx = Integer-Wert (als Hex!)
           pusha             		 ; Alles Register sichern
           mov [buffer1], dx         ; Buffer1 entählt immer den aktuellen Hex-Wert
           xor di, di        		 ; di = 0 = zähler der Bigloops = Anzahl der geschriebenen Ziffern
           mov si, 1                     ; SI: 0x0A hoch SI = größter Teiler, bei dem der Wert unter 0x0A ist!
           mov bx, 0                     ; Sicherung um SI zu überprüfen
           mov cx, 0x0A      
               
.bigloop:  mov bx, si                    ; BX WIRD aktualisiert und erniedrigt.
           dec bx
           mov dx, [buffer1]
           mov si, 0                     ; si = 0
           mov cx, 0x0A        
           call divloop            
           cmp si, bx			 ; Es wird überprüft ob SI und bx übereinstimmen. SI bekommt beim 1. Durchlauf einen wert         
           jnb .nopadding                ; dieser Wert wird bei jedem durchlauf um eins kleiner. Ist dies nicht der Fall (SI ist auf
                                         ; einmal um 3 kleiner[als BX bzw. letzten Durchlauf]) werden '0' geschrieben und 
             push ax                     ; zwar n mal (n = BX - SI)
             mov cx, 0x30
             mov dx, si
.pudding:     inc si
              mov [intf+di], cx
              inc di
              cmp bx, si
              jnz .pudding
             mov si, dx
             pop ax

.nopadding:mov dx, [buffer1]   ;IntWert hohlen                      

           call maxteil

.endloop:  mov dx, [buffer1]                        ; Hohlen des Ursprungswerts
           mov cx, 0    
           xchg dx, ax     
           xchg dx, cx 			 ; cx = Teiler, AX = IntWert
           div cx                        ; DX = IntWert mod Teiler
           call savein
           mov [buffer1], dx                       ; Neuer Ursprungswert bzw. IntWert wird geschrieben
           cmp si, 0                     
           jnz .bigloop 
           cmp dx, 0x0A                  ; Nochmal ne Sicherung: Wenn der IntWert unter 0x0A ist, wird
           jnb .bigloop                  ; beendet

           add dx, 0x30                  ; die erste (bzw. letzte Stelle - wie man es sieht) wird geschrieben
           mov [intf+di], dx             ; hier wirds geschrieben.
           mov dx, [buffer1]             ; Es ist noch was auf dem Stack gewesen.
           popa                          ; Alle register wiederherstellen
 	   mov bp, sp                    ; Standart
	   pop bp
           mov si, intf                  ; Addresse wird übergeben
           mov dx, si                    ; Sicherung
           mov ax, si
	   ret
	   
divloop:      cmp dx, 0X0A
              jnb .bypass1
              mov si, 0
              ret
.bypass1:     mov ax, dx                 ; ax = IntWert
              xor dx, dx                                          
              div cx                     ; IntWert wird durch 0x0A geteilt! AX = IntWert / 0x0A; 
              inc si                     
              mov dx, ax	         ; ax = dx 
              cmp ax, 0x0A               ; Wenn ax kleiner als 0x0A ist, dann wid die Schleife beendet.
              jnb divloop  
              ret

maxteil:   mov cx, 1
           mov bx, 0x0A
           mov ax, 1
.loop3     mul bx                    ; Ausrechnen von 0x0A ^ SI -> AX 
           inc cx
           cmp cx, si
           jbe .loop3                ; Ende der Schleife.
           ret

savein:    add ax, 0x30                  ;AX wird in ASCII Code umgewandelt
           cmp ax, 0x30                  ;Der String soll nicht mit '0' anfangen.
           jnz .write
           cmp dx, 0x0A                  ; Fall AX = '0' und erste Stelle im String, wird nicht geschrieben
           jnb .bypass2
           ;cmp di, 0
           ;jz .nowrite 
.bypass2:  mov si, 0
           jmp .nowrite                  ; DI wird dafür erniedrigt, wird später wieder erhöht.
.write:    mov [intf+di], ax
           inc di                        ; nächste Stelle im String
.nowrite   ret

;--------------------------------

;----------------------
; Diese Funktion liefert 1. den Hersteller-String der CPU und 2. die max. Anzahl an CPUID-Anfragen (höchster EAX wert), die man auf 
; diesem PC machen kann.
; CPUID kann noch viel mehr, wurde hier aber nicht integriert!
; Bitte auch die Doku Beachten!

_CPUInfo:push bp				; Standart Teil								CPUInfo
         mov bp,sp				; 
         mov eax, 0x00
         mov ax,[bp+4]				; AX wird dem Wert, der übergeben wird zugewiesen
         cmp ax, 0h 				; Wenn ax = 99 -> Nur MaxEAX Wert für CPUId senden
         jz .hersteller				 
         cpuid
         mov si, ax
         pop bp
         ret
.hersteller: 
         cpuid 					; Gibt Infos über CPU				
         mov si, CPUID_Buf			; In EBX, EDX und ECX sind Infos als String gespeichert
         mov [si], ebx				; Diese werden in CPUID geschrieben
         mov si, CPUID_Buf+4
         mov [si], edx
         mov si, CPUID_Buf+8
         mov [si], ecx
         mov si,CPUID_Buf			; Die Addy von CPUID_Buf wird zurückgegeben ( x = CPUID (0) ) -> x = Addy von CPUID_Buf
         mov ax, si                             ; Das 
         pop bp
         ret

;----------------------
; Hiermit wechselt Nova-X in den Graphik-MOdus. Dieser wird bis jetzt nicht oft verwendet.
; Für weitere Infos siehe Doku.
; 

_graph:  mov ah,0x00		       ;GRAPH COMMAND								   	Graph
	 mov al,12h
	 int 0x10
	 ret

;----------------------
; Der text Modus. Wird sehr oft verwendet!
; Für weitere Infos siehe Doku.
; 

_text:	 mov ah,0x00		       ;TEXT COMMAND									Text
	 mov al,3h
	 int 0x10
	 ret
	 
;----------------------
; GotoXY - API
; Für weitere Infos siehe Doku.
; Setzt den Cursor auf die übergeben X,Y Koordinaten.

_gotoxy: push bp                       ; Standart Teil									GotoXY
         mov bp, sp                    ; Standart Teil
         sub sp, 0x40                  ; Standart Teil
         mov ah, 0Fh		       ; Video-Mode herausfinden
         int 10h                       ; BH = aktuelle Display-Page
         mov dl, [bp+4]		       ; gewünschte X-Koordinate
         mov dh, [bp+6]                ; gewünschte Y-Koordinate
         dec dl                        ; X + Y um 1 erniedrigen. Ursprung des Bilschirms 1,1!
         dec dh
         mov ah, 02h                   ; Int 10 vorbereiten
         int 10h                       ; Setzte den Coursor
         mov sp, bp                    ; Standart Teil
         pop bp                        ; Standart Teil
         ret

;----------------------
; WhereX - API
; Für weitere Infos siehe Doku.
; Bestimmt die X Position des Cursors

_wherex: 
         mov ah, 0x03
         mov bh, 0x00     ; Video-Page
         int 0x10         ; in DL ist die X-Position
         mov dh, 0x00
         inc dx
         mov si, dx
         mov dx, si
         mov ax, si
         ret

;----------------------
; wherey - API
; Für weitere Infos siehe Doku.
; Bestimmt die Y Position des Cursors

_wherey: mov ah, 0x03
         mov bh, 0x00     ; Video-Page
         int 0x10         ; in DH ist die Y-Position
         mov dl, dh
         mov dh, 0x00
         inc dx
         mov si, dx
         mov dx, si
         mov ax, si
         ret


;----------------------
; Printf - API
; Für weitere Infos siehe Doku.
; Eine der wichtigsten APIs - Jetzt ohne Teletype-Mode!!!!

_printf: push bp		        ;PRINTF COMMAND									Printf
	 mov bp,sp		        ; Standart Teil
	 mov bl,3		        ; Standart Teil
	 sub sp,0x40			; Standart Teil
	 mov bx,[bp+4]                  ; BX = Zeiger zum String
	 mov si, bx                     ; SI = Zeiger zum String
	 call print                     ; Print unterfunktion wird aufgerufen
	 mov sp,bp			; Standart Teil
	 pop bp				; Standart Teil
	 ret

;----------------------
; Reboot - API
; Tja, so kann man den PC rebooten :)

_reboot: jmp 0xffff:0x0000


;----------------------
; getch - API  
;
; Für weitere Infos siehe Doku
; Funktionsbeschreibung: Gibt eine gedrückte Taste zurück. Ist dies
; eine Nicht ASCII Taste (ASCII Code 0) dann wird der SCAN Code zurückgegeben!
; 18.01.2003 Modifkation by Hornet: Parameterübergabe
; ist Parameter = 0 dann gibt ASCII Code, wenn 1 dann SCAN Code

_getch:  push bp                       ; Standart Teil									GotoXY
         mov bp, sp                    ; Standart Teil
         sub sp, 0x40                  ; Standart Teil
        xor ax, ax                     ; AX = 0										getch
        int 16h                        ; Warten auf Tastendruck -> AL = ASCII Code, AH = SCAN-CODE
        call dekeyb
        mov bx,[bp+4]                ; BX = Parameter
      	 mov sp,bp			        ; Standart Teil
       	 pop bp			        	; Standart Teil
        cmp bx, 0x01                   ; Wenn BX <> 1 ist, dann zu .normal (ASCI Code)
        jnz .normal         
        mov al, ah                     ; Taste = "Special"-Taste, also Scan-Code in AL!
        mov ah, 0x00                   ; Um Scan-Code und ASCII Code unterscheiden zu können, wird zum ScanCode 255 addiert
        mov dx, 0xFF                   ; Dies kann dann kein ASCII Zeichen sein, da es größer als 255 ist! 
        add ax, dx
        mov si, ax                     ; Scan-Code + 255 wird zurückgegeben
        mov dx, ax
        ret
.normal:mov ah, 0h                     ; AH wird 0
        mov si, ax                     ; AX = SI, Si wird zurückgegeben.
        ret


;----------------------
; scanf - API
; Für weitere Infos siehe Doku.
; Eine der wichtigsten APIs

_scanf: 
        mov si, scanfbuf               ; scanfbuf: ein Buffer, wird die Zeicheneingabe- SI = Zeiger auf den Buffer		scanf
.begin  xor ax, ax                     ; AX = 0 <- vorbereitung zum Zeichen-Einlesen                      
        int 16h                        ; Warte auf Tastendruck: AL = ASCII Code
        call dekeyb
        cmp al, 8h                     ; Teste, ob AL (Taste) Backspace-taste war
        jnz .notbackspace              ; Keine backspace-taste dann jumpe

        cmp si, scanfbuf               ; --- Backspace-Procedure ---
        jz .begin                      ; Ist SI = Addresse von scnafbuf, dann kein Backspace! So wird zu weites löschen vermieden!
        dec si                         ; SI - 1 (vorheriges Zeichen)
        push ax                        ; --
        xor ax, ax                     ; AX = 0
        mov [si], ax                   ; Vorheriges Zeichen wird mit 00 überschrieben, somit gelöscht
        pop ax                         ; --
             pusha				; ---- Cursor wird um 1 zurückgeschobe, und der Inhalt wird gelehrt
             mov ah, 03h			; Cursor-Position auslesen: DL = Spalte des Cursors
             int 10h				
             dec dl                             ; Spalte des Curors (DL) - 1
             mov ah, 02h                        ; Setzten der neueun Cursor Position
             int 10h                            ; 
             mov al,' '				; Die Position wird mit ' ' überschrieben, damit es auch optisch aussieht, 
             call printchar                     ; als ob das Zeichen gelöscht wurde. 
             mov ah, 03h                        ; Cursor wird wieder um 1 zurückgeschoben (nach dem printchar)
             int 10h
             mov ah, 02h
             dec dl
             int 10h                            ; Setzten des Cursors
             popa                               

        jmp .begin
.notbackspace: 
        cmp ah, 0x1C		 	; Ist AH (ScanCode) = Enter-Taste
        jz endscan                      ; Dann Ende        
        cmp al, 0h                      ; Ist AL = 0 (nicht ASCII Code Taste), dann 
        jz .begin                       ; Ignoriere sie
        Call printchar                  ; Zeichen erscheint auf dem Bildschirm 
        mov [si], al                    ; Zeichen (AL) wird in ScanfBuf geschrieben
        inc si                          ; Si wird erhöht(= nächstes Zeichen)
        jmp .begin                      ; Schleife
endscan:                                ; Wenn "Enter" gedrückt wurde
        xor ax, ax                      ; AX = 0
        mov [si], ax			; letztes Zeichen in [Si] bzw. Scanfbuf wird 0 (Ende des Strings);
        mov ax,scanfbuf			; AX = Zeiger auf Scanfbuf
        mov si, ax			; AX wird zurückgegeben
        ret

;----------------------
; CLS - API
; Für weitere Infos siehe Doku.
; Auch eine wichtige API

_cls:	    			       ; CLEAR COMMAND [mod by Hornet]								CLS
        mov ah, 0Fh		       ; Video-Mode herausfinden
        int 10h                        ; AL = Video-Modus Nr.
        mov ah,0x00                    ; Neuen Video-Modus setzten
	int 0x10                       ; Mit der Modus nr. AL
	ret
	
;----------------------
; strdef - API
; Diese API liefert einen Zeiger zum String.
; 

_strdef:push bp 		       ;STRDEF COMMAND										strdef
	mov bp,sp
	sub sp,0x40
	mov bx,[bp+4]
	mov si,bx
	mov ax,si
	mov dx,si
	mov sp,bp
	pop bp
	ret

;-----------------------
; Liest die CMOS-Zeit aus
; Comments by thesame
; fixed by Hornet

_getcmostime:      mov ah, 0x02         ; AX muss 2 sein
                   int 0x1A             ; Über diesen Interrupt wird die Systemzeit ermittelt.
                   pusha                ; Alle Register auf den Stack schreiben.
                   mov dx, 0x10         ; 10 auf DX schreiben
                   mov al, CH           ; Die Stunden auf AL schreiben
                   mov ah, 0x00         ; 0 auf AH schreiben
                   div dl               
                   add ah, 0x30         ; Zu Ergebnis und Rest noch 48 (=30h) dazuaddieren,
                   add al, 0x30         ; damit wir den ASCII-Wert der Zahl bekommen
                   mov [timebuf], al    ; Erste Ziffer der Stunden in erstes Zeichen von timebuf schreiben.
                   mov [timebuf+1], ah  ; Zweite Stundenziffer in zweites Zeichen von timebuf.
                   mov al, 0x3A         ; Doppelpunkt nach Stunde schreiben (ASCII-Zeichen 58 = 3Ah) (HH:)
                   mov [timebuf+2], al
                   
                   popa                 ; Stack in alle Register schreiben.
                   pusha                ; Alle Register in Stack schreiben
                   mov dx, 0x10         ; DX 10 zuweisen
                   mov al, CL           ; AL Minuten zuweisen
                   mov ah, 0x00         ; AH 0 zuweisen
                   div dl
                   add ah, 0x30         ; um 30h erhöhen, damit es ASCII-Wert gibt.
                   add al, 0x30         ; auch bei AL
                   mov [timebuf+3], al  ; Erste Minutenziffer in viertes Zeichen von timebuf schreiben
                   mov [timebuf+4], ah  ; Zweite Ziffer in fünftes
                   mov al, 0x3A         ; al ASCII-Code für Doppelpunkt zuweisen
                   mov [timebuf+5], al  ; Sechtes Zeichen in timebuf Doppelpunkt schreiben (HH:MM:)
                   
                   popa                 ; Stack in alle Register schreiben
                   pusha                ; Alle Register in Stack schreiben
                   mov ch, dh           ; DH = sekunden
                   mov dx, 0x10         ; DX 10 zuweisen
                   mov al, ch           ; AL Sekunden zuweisen
                   mov ah, 0x00         ; AH 0 zuweisen
                   div dl
                   add ah, 0x30         ; Wieder 30h dazuaddieren, damit es einen gültigen ASCII-Code gibt.
                   add al, 0x30         ; Auch bei der zweiten Ziffer der Sekunden
                   mov [timebuf+6], al  ; 1. Sekundenziffer ins 7. Zeichen von timebuf
                   mov [timebuf+7], ah  ; 2. Sekundenziffer ins 8. Zeichen von timebuf (HH:MM:SS)
                   popa                 ; Stack in alle Register schreiben
                   mov si, timebuf      ; Für den Rückgabewert der Funktion timebuf auf SI,
                   mov ax, timebuf      ; AX
                   mov dx, timebuf      ; und DX schreiben.
                   ret                  ; Zum eigentlichen Code zurückkehren.

;-----------------------
; Liest das CMOS Datum aus
; Comments by thesame
; fixed by Hornet

_getcmosdate: 
 	 mov ah, 0x4          ; AH muss 4 sein. 
	  int 0x1A             ; Über diesen Interrupt wird das Systemdatum ermittelt. 
	  pusha                ; Alle Register auf den Stack schreiben. 
	  mov al, DL           ; AL Tage zuweisen 
	  mov dx, 0x10         ; DX 16 (=10h) zuweisen 
	  mov ah, 0x00         ; AH 0 zuweisen 
	  div dl	 
	  add ah, 0x30         ; zu 1. Tagesziffer um 48 erhöhen, damit es einen gültigen ASCII-Wert gibt. 
	  add al, 0x30         ; Gleiches Verfahren mit der 2. Tagesziffer 
	  mov [datebuf], al    ; Erstem Zeichen von datebuf 1. Tagesziffer zuweisen 
	  mov [datebuf+1], ah  ; Gleiches Verfahren mit der 2. Tagesziffer und dem zweiten Zeichen von datebuf. 
	  mov al, 0x2F         ; AL Ascii-Code für Splash (/) zuweisen. 
	  mov [datebuf+2], al  ; 3. Zeichen von datebuf al, also Slash zuweisen (TT/MM/YY) 
  
       	  popa                 ; Stack in alle Register schreiben 
  	  pusha                ; Alle Register in Stack schreiben 
	  mov al, DH           ; AL Monat zuweisen. 
	  mov dx, 0x10 
	  mov ah, 0x00 
	  div dl 
	  add ah, 0x30 
	  add al, 0x30 
	  mov [datebuf+3], al  ; Monat und Slash in datebuf schreiben. 
	  mov [datebuf+4], ah 
	  mov al, 0x2F 
	  mov [datebuf+5], al 
  
  	  popa 
  	  pusha 
 	  mov dx, 0x10 
	  mov al, CH 
	  mov ah, 0x00 
	  div dl 
	  add ah, 0x30 
	  add al, 0x30 
	  mov [datebuf+6], al 
	  mov [datebuf+7], ah 
  
	  popa 
	  pusha 
	  mov dx, 0x10 
	  mov al, CL 
	  mov ah, 0x00 
	  div dl 
	  add ah, 0x30 
	  add al, 0x30 
	  mov [datebuf+8], al 
	  mov [datebuf+9], ah 
	  popa 
	  mov si, datebuf 
	  mov ax, datebuf 
	  mov dx, datebuf 
	  ret


;----------------------
; Liefert die Größe des RAMS

_getbasememsize:   int 0x12             ; Interrupt 18 (12h) zum ermitteln der RAM-Größe aufrufen.
                   mov [membuf], ax
                   mov dx, ax           ; Das Ergebis steht in AX. Vorsichtshalber AX noch in DX
                   mov si, ax           ; und SI schreiben.
                   mov ax, si
                   ret                  ; Zum eigentlichen Code zurückkehren.


;-------------------------------------------------------------------------------
; Internal Functions
;-------------------------------------------------------------------------------

;-----------------
; Unterfunktion
; Funktion gibt das Zeichen in AL aus, und verschiebt den cursor um einen nach rechts.

printchar:
         mov ah, 0x09		
         mov bl, [textcolorm]    		; Farbe wird berücksichtigt!						printchar
         mov cx, 1h
         int 10h
         Call NextColumn
         ret

;-----------------
; Unterfunktion
; siehe "printf"
; Funktion ist eine Art Schleife, bei jedem Durchlauf wird ein Zeichen gezeichnet.

print:	 mov al, [si] 				; si = Zeiger auf zu schreibenden String; AL = 1 Zeichen des Strings	print
	 inc si                                 ; Si + 1 = nächstes Zeichen
	 cmp al, 0x00                           ; ist AL = 0 -> Ende des Strings, dann 
	 jz pret                                ; beende die Funktion		""
         cmp al, 0x0A  				; Wenn AL = "\n" (nächste Zeile), dann         
         jz .nextz 				; springe zu .nextz
         cmp al, 0x0D 				; Wenn AL = "\r" (kA was das ist), dann 
         jz print                               ; ingoriere es :)
         MOV AH,0x09				; Modus 09h Int 10h stellen
         MOV Bl, [textcolorm]                   ; BL = Farbe des Chars (auch Hintergrundfarbe)
         mov bh, 0				; Video-Page = 0 = aktive Page (?)
         mov cx, 1h                             ; Buchstabe soll 1 mal geschrieben werden
	 int 0x10            			; Zeichen setzten
         call NextColumn                        ; Cursor wird um 1 nach rechts verschoben!
         jmp print                              ; Schleife
.nextz:  call NextRow                           ; Cursor spring in eine neue Zeile
         jmp print                              ; Schleife
pret:	 ret                                    ; Wenn AL = 0 dann hier ende


;-----NEXTROW------------
; Unterfunktion
; siehe "print"
; Funktionsbeschreibung: 1. aktuelle Cursor-Position auslesen, 
; dann Cursor in die nächste Zeile setzten.

NextRow  :pusha				; Immer ganz gut. Es werden alle wichtigen Register gepusht!			nextrow
          mov ah, 03h                   ; Das Auslesen der Position des Cursors wird vorbereitet.
          mov bh, 0                     ; Display-Page = 0 = aktive Page
          int 10h                       ; AUslesen der Position: DH = Zeile, DL = Spalte
          inc dh                        ; Zeile wird um eins erhöht
          cmp dh, 0x19                  ; Ist Zeile nicht 25, dann
          jnz .noscrolling 		; springe zu .weiter

; Wenn Zeile = 25 (ganz unten), muss der Screen nach oben gescrollt werden! Funktion: AH=06h - Int10h
	  mov ah, 06h                   ; Vorbereitung zu Scrollen
          mov al, 1h                    ; Anzahl der Zeilen zu Scrollen = 1
          mov bh, [textcolorm]          ; Hier wir ein "Fenster" angegeben, mit der linken oberen Ecke 
          mov ch, 0			; und der unteren rechten Ecke! Dieses wird dann gescrollt!
          mov cl, 0
          mov dh, 0x19                  ; Untere lnke Ecke
          mov dl, 0x51                  ; Untere rechte Ecke
          int 10h                       ; Scrolle den Schirm
          mov dh, 0x18                  ; Dh = unterste Zeile des Screens

.noscrolling:				; Setzten des Cursors wird vorbereitet
          mov ah, 02h
          mov bh, 0                     ; Display-Page = 0
          mov dl, 0h                    ; Spalten Nr = 0; Cursor soll zum Anfang der Zeile springen
          int 10h                       ; Setzten des Cursors
          popa                          ; Alle wichtigen Register werden wieder hergeholt (siehe weiter oben). 
          ret

;-----NEXTCOLUMN------------
; Unterfunktion
; siehe "print"
; Funktionsbeschreibung: 1. aktuelle Cursor-Position auslesen, 
; dann Cursor in die nächste Spalte setzten.

NextColumn:
          pusha				; Immer ganz gut. Es werden alle wichtigen Register gepusht!			nextcolumn
          mov ah, 03h			; Cursors auslesen
          int 10h			; "
          inc dl			; DL (= Spalten Nr. des Curors) wird erhöht
          mov ah, 02h                   ; Cursor setzten
          int 10h			; "
          popa                    	; Alle wichtigen Register werden wieder hergeholt (siehe weiter oben). 
          ret

;-----DeKEyB------------
; Unterfunktion
; siehe scanf und getch
; Funktionsbeschreibung: wird die Z bzw. Y-Taste
; gedrückt so werden diese Zeichen vertauscht.


dekeyb: cmp al, 'Z'
        jz makey
        cmp al, 'z'
        jz makey
        cmp al, 'Y'
        jz makex
        cmp al, 'y'
        jz makex
        ret
makey:  sub al, 1
        ret
makex:  add al, 1
        ret

;-------------------------------------------------------------------------------
; Data
;-------------------------------------------------------------------------------

textcolorm db ' '
fname db 'LOADER  BIN'
savedstack db 2
scanfbuf db 64
firstboot db 2
MsgComplete DB ' done',0
CPUID_Buf DB '            ',0 
timebuf db '        ',0
membuf db '     ',0
datebuf db '            ',0
intf db '    ',0
buffer1 db 0x0000
buffer2 db 0x0000
