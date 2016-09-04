; Nova X - OS

; Copyright (C) 2003  Nova-X Team (www.nova-x.net)

; This program is free software; you can redistribute it and/or

; modify it under the terms of the GNU General Public License

; as published by the Free Software Foundation; either version 2

; of the License, or (at your option) any later version.

; This program is distributed in the hope that it will be useful,

; but WITHOUT ANY WARRANTY; without even the implied warranty of

; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the

; GNU General Public License for more details.

; You should have received a copy of the GNU General Public License

; along with this program; if not, write to the Free Software

; Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

; -----------

; Versions-History am Ende der Datei

; -----------

GLOBAL _putchar    

GLOBAL _gotoxy

GLOBAL _textcolor

GLOBAL _textbackground

GLOBAL _initvideo

GLOBAL _scrolls

GLOBAL _wherex

GLOBAL _wherey

GLOBAL _cls

GLOBAL _setCursorPosition

GLOBAL _fixSSandDSBug



[BITS 32]



;----------------------

; fixSSandDSBug

; Manchmal vertauscht gcc SS und DS register

; Das hier macht die stdarg.h funktionsfähig



_fixSSandDSBug:

            push bp	        	 ; Standart Teil.       			

            mov bp,sp			 ; Standart Teil.

	        sub sp,0x40     

	        xor eax, eax

	        mov eax, [bp+6]         ; AL = auszugebendes Zeichen

	        mov ecx, [SS:eax]

	        mov eax, ecx

	        mov sp,bp               ; Standart Teil

	        pop bp                  ; Standart Teil

ret 



;----------------------

; Video Einstellung

; Es wird die Standart Text bzw. Hintergrundfarbe gewählt.

_initvideo:

            pusha

            mov eax, 0x38

            mov gs, eax

            mov al, 0x07

            mov [gs:2], al      ; 0x07 = Schwarz + Weiß

            popa

            int 0x20

            int 0x21

            ret 



;----------------------

; CLS - API

; Für weitere Infos siehe Doku.

; Auch eine wichtige API



_cls:	    			       ; CLEAR COMMAND [ProtMode]								CLS

            pusha

            CLI

            mov eax, 0x10

            mov es, eax

            xor edi, edi

            mov eax, 0x0700

            mov ecx, 0x7D0

            rep stosw          ; Der Video-RAM wird überschrieben, mit 0x0007

            mov eax, 0x38

            mov gs, eax

            xor eax, eax         ; Cursor setzten

            mov [gs:0], ax

            STI

            popa

            call _setCursorPosition

            ret 





;----------------------

; WhereX - API

; Für weitere Infos siehe Doku.

; Bestimmt die X Position des Cursors



_wherex: 

         mov ah, 0x03

         mov bh, 0x00     ; Video-Page

         int 0x50         ; in DL ist die X-Position

         mov dh, 0x00

         inc edx

         mov eax, edx

         ret 



;----------------------

; wherey - API

; Für weitere Infos siehe Doku.

; Bestimmt die Y Position des Cursors



_wherey: mov ah, 0x03

         mov bh, 0x00     ; Video-Page

         int 0x50         ; in DH ist die Y-Position

         mov dl, dh

         mov dh, 0x00

         inc edx

         mov eax, edx 

         ret 



;----------------------

; putchar - API

; Gibt ein Zeichen auf dem Bildschirm aus :)



_putchar:

            push bp	        	 ; Standart Teil.       			

            mov bp,sp			 ; Standart Teil.

	        sub sp,0x40     

	        mov eax, 0x38

	        mov gs, eax

	        mov ax, [bp+6]         ; AL = auszugebendes Zeichen

            MOV AH,0x09				; Modus 09h Int 10h stellen

            MOV Bl, [gs:2]          ; BL = Farbe des Chars (auch Hintergrundfarbe)

            mov bh, 0				; Video-Page = 0 = aktive Page (?)

            mov cx, 1h              ; Buchstabe soll 1 mal geschrieben werden

	        int 0x50             	; Zeichen setzten

	        xor eax, eax

	        mov ax, [gs:0]          ; Cursor verschieben

	        inc ax

	        inc ax

	        mov [gs:0], ax

            call _scrolls           ; Wenn nötig, dann Scrolle

	        call _setCursorPosition  ; Cursor updaten

	        mov sp,bp               ; Standart Teil

	        pop bp                  ; Standart Teil

ret 



;------------------------

; Textcolor

; Ich weiß, dieses sieht kompliziert aus. Die Farbe eines Zeichens wird nur durch

; einen Wert bestimmt. z.B. "1C". Dabei ist 1 = die Hintergrundfarbe und C die Textfarbe

; Man muss die Buchstaben nun trennen, indem man schaut wie oft 1C durch 10 teilbar ist (ein mal);

; und dann rechnet man ein mal  10 + gewünschte Hintergrundfarbe.

; Was einfacheres ist mir nicht eingefallen :)

; Last Update: 13.08.2003 - 11:15

; Funktion arbeitet anstatt mit Dividieren nun mit AND (PIII-Fix) !

_textcolor: push bp	        	 ; Standart Teil. kA warum	      						textcolor

            mov bp,sp			 ; Standart Teil. kA warum	

	        sub sp,0x40     

	        mov eax, 0x38

	        mov gs, eax

	        xor eax, eax

            mov ax, [gs:2] ; ax = Colorwert 1. Stelle = Hintergrund;2.Stelle=Textfarbe

            and ax, 11110000b    ; Setzt die 2. Stelle auf 0x00

            mov dx,[bp+6]     	 ; dx = gewünschter Farbwert 

            add eax, edx         ; ax = akt. Hintergrundfarbe mit gewünschter Textfarbe addieren

            mov [gs:2], al ; Farbe wird gespeichert.

	        mov sp,bp            ; Standart Teil

	        pop bp               ; Standart Teil

	        ret 



;------------------------

; Siehe "auch" _textcolor

; Last Update: 13.08.2003 - 11:15

; Funktion arbeitet anstatt mit Dividieren nun mit AND (PIII-Fix)!


_textbackground:     											       

            push bp	  	    	; Standart Teil. kA warum	      

	        mov bp,sp	        ; Standart Teil. kA warum	

	        sub sp,0x40     

	        mov eax, 0x38

	        mov gs, eax

	        xor eax, eax

                mov ax, [gs:2]   	; AX = Colorwert 1. Stelle = Hintergrund;2.Stelle=Textfarbe

                and ax, 00001111b       ; Die 1. Stelle wird auf 0 gesetzt

	        mov dx,[bp+6]     	; dx = gewünschte hintergrundfarbe                      
	        
	        shl dx, 4               ; dx wird um 4 Bits nach rechts verschoben       

                add eax, edx       	; eax = gewünschte Hintergrundfarbe mit Textfarbe addieren

                mov [gs:2], ax  	; Farbe wird gespeichert.

	        mov sp,bp               ; Standart Teil

	        pop bp                  ; Standart Teil

	        ret 





;----------------------

; GotoXY - API

; Für weitere Infos siehe Doku.

; Setzt den Cursor auf die übergeben X,Y Koordinaten.



_gotoxy: push bp                       ; Standart Teil									GotoXY

         mov bp, sp                    ; Standart Teil

         sub sp, 0x40                  ; Standart Teil

         mov bh, 0x00

         mov dl, [bp+6]  		       ; gewünschte X-Koordinate

         mov dh, [bp+6+4]                ; gewünschte Y-Koordinate

         dec dl                        ; X + Y um 1 erniedrigen. Ursprung des Bilschirms 1,1!

         dec dh

         mov ah, 02h                   ; Int 10 vorbereiten

         int 0x50                       ; Setzte den Coursor

         call _setCursorPosition

         mov sp, bp                    ; Standart Teil

         pop bp                        ; Standart Teil

         ret 







;*********************

; Die Funktion setzt die Cursor Position durch Programmieren des Videocontrollers

; Dazu wird auf die Ports 0x3D4 und 0x3D5 zugegriffen

; Die Cursorposition wird in 2 Byte Große Einzelteile aufgespalten und dann gesendet.

;*********************



_setCursorPosition:

        pusha

        CLI

        mov ax, 0x38

        mov gs, ax

        mov ebx, 0x02        ; Position umwandeln

        xor edx, edx

        xor eax, eax

        xor ecx, ecx

        mov ax, [gs:0]      ; AX = gewünschte Cursor Position

        div bx

        mov bx, 0x100

        xor dx, dx

        div bx

        mov cx, ax          ; In CX und BX befinden sich die Einzelteile

        mov bx, dx



        mov dx, 0x3D4 

        mov ax, 0x0F

        OUT DX,AX   



        mov dx, 0x3D5 

        mov ax, bx         ; 1. Teil der Cursor-Position

        OUT DX,AL   



        mov dx, 0x3D4 

        mov ax, 0x0E

        OUT DX,AL   



        mov dx, 0x3D5 

        mov ax, cx         ; 2. Teil der Cursor Position

        OUT DX,AL   

        STI

        popa         

        ret



;*********************

; Diese Funktion überprüft die aktuelle Cursor-Position.

; Ist diese ausserhalb des Bildschirms wird gescrollt.

; Dazu wird der VideoRAM ab der 1 Zeile nach oben "verschoben"

;*********************

_scrolls:       xor edi, edi

                mov di, [gs:0]

                cmp di, 0xFA0       ; Ist Cursor noch auf dem Bildschirm

                ja .scrolling

                jmp .end            ; Es braucht nicht gescrollt werden

.scrolling      pusha

                mov eax, 0x10        ; Video Descriptor

                mov ds, eax

                mov es, eax

                xor edi, edi

                mov esi, 0xA0        ; Begin der 2. Zeile

                mov ecx, 0xF00       ; Größe des VideoRAMS ohne erste Zeile

                CLD

                CLI

                rep movsw           ; Verschiebe des RAMs

                STI

                mov eax, 0x38        ; Die Standart Register setzten

                mov gs, eax

                mov eax, 0x40

                mov ds, eax

                mov edi, 0x00        ; Tja, was hab ich denn hier gemacht????

                mov [ds:0], di

                xor eax, eax

                mov eax, [gs:0]

                sub ax, word 0xA0   ; Cursor setzten

                mov [gs:0], ax

                call _setCursorPosition

                popa

.end                

                ret


; --------------------------------

; Versions-History

; - Original geschrieben von Hornet

; - 13.08.2003 - 11:15 : MB's Veränderungen an der textcolor und textbackground (Hornet)

; --------------------------------

