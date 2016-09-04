[BITS 32]

SECTION .text

;-------------------------------------------------------------------------------

; Share API 

;-------------------------------------------------------------------------------



; Hier werden die exportierbaren Funktionen angegeben. u.a. die C-APIs

; f�r eine Beschreibung der APIs, die extra Doku lesen   



GLOBAL _enableA20

GLOBAL _sleep

GLOBAL _nxinport

GLOBAL _nxoutport

GLOBAL _reboot

GLOBAL _CPUInfo                 

GLOBAL _writeallmem

GLOBAL _readallmem

GLOBAL _systemstop

GLOBAL _gettickscount
GLOBAL _getcr

GLOBAL _setFPUPM


;-------------------------------------------------------------------------------

; Nova-X API

;-------------------------------------------------------------------------------



_gettickscount:

            CLI

            push bp	        	 ; Standart Teil. kA warum	      			

            mov bp,sp			 ; Standart Teil. kA warum	

	        sub sp,0x40    

	        push gs

   	   	    mov eax, 0x38

   	        mov gs, eax

            mov eax,[gs:0x100]   ; Die aktuelle Ticks-Anzahl

            pop gs

	        mov sp,bp            ; Standart Teil

	        pop bp               ; Standart Teil

            STI

        ret



;------------------------

; Diese Funktion stoppt das System, es kann aber noch debugged werden

;------------------------

_systemstop:

        pusha

        xor eax, eax

        .l: cmp eax, 0x1581

        jnz .l

        popa

ret





;------------------------

; Schreibt an Param1 Param2 (Beides Int) in den Speicher. Dazu wird das Big Segment benutzt.

;------------------------

_writeallmem:

            push bp	        	 ; Standart Teil. kA warum	      			

            mov bp,sp			 ; Standart Teil. kA warum	

	        sub sp,0x40    

	        mov edx, [bp+6]

	        mov ecx, [bp+6+4]

	        push es

	        push di

	        mov ax, 0x30

	        mov es, ax

	        mov edi, edx

	        mov eax, ecx

	        stosd

	        pop di

	        pop es

	        mov sp,bp            ; Standart Teil

	        pop bp               ; Standart Teil

	        ret



;------------------------

; Liest aus dem Speicher bei Param1 (Int) einen Int aus und gibt diesen zur�ck. Daf�r wird

; das Big Segment benutzt.

;------------------------

_readallmem:

            push bp	        	 ; Standart Teil. kA warum	      			

            mov bp,sp			 ; Standart Teil. kA warum	

	        sub sp,0x40    

	        mov edx, [bp+6]

	        push ds

	        push si

	        mov ax, 0x30

	        mov ds, ax

	        mov esi, edx

	        xor eax, eax

	        lodsd

            pop si

	        pop ds

	        mov sp,bp            ; Standart Teil

	        pop bp               ; Standart Teil

	        xor edx, edx

	        ret









;------------------------

; NXOutport

; Sendet einen Wert an einen bestimmten Port



_nxoutport:

            push bp	        	 ; Standart Teil. kA warum	      			

            mov bp,sp			 ; Standart Teil. kA warum	

	        sub sp,0x40    

	        mov edx, [bp+6]

	        mov eax, [bp+6+4]

 	        out dx, al

	        mov sp,bp            ; Standart Teil

	        pop bp               ; Standart Teil

	        ret



;------------------------

; NXInPort

; Liest einen Wert von einem Port



_nxinport:

            push bp	        	 ; Standart Teil. kA warum	      			

            mov bp,sp			 ; Standart Teil. kA warum	

	        sub sp,0x40    

	        mov edx, [bp+6]

	        xor eax, eax

 	        in al, dx

	        mov sp,bp            ; Standart Teil

	        pop bp               ; Standart Teil

	        ret

	        

;------------------------

; BiosDisk

; Diese Funktion l�st den Interrupt 13 aus. F�r detailierte Angaben siehe Nova-X API

; Dokumentation



_biosdisk : push bp	        	 ; Standart Teil. kA warum	      			

            mov bp,sp			 ; Standart Teil. kA warum	

	        sub sp,0x40     

	        

            mov ah,[bp+4]     	 ; ah = gew�nschter Befehl

            mov dl,[bp+6]        ; dl = Ger�te-Nr.

            mov dh,[bp+8]        ; dh = Head-Nr.

            mov ch,[bp+0x0A]        ; ch = Track-Nr.

            mov cl,[bp+0x0C]        ; cl = Sektor-Nr.            

            mov al,[bp+0x0E]        ; al = Anz. der zu lesenden Sektoren

            mov bx,[bp+0x10]        ; bx = Addresse, wohin Sektor gelesen wird

            int 0x13             ; Interrupt ausl�sen

            

            mov al, ah           ; Vorbereitung zur Parameter�bergabe (Status)

            mov ah, 0x00

            mov esi, eax

            mov edx, eax

	        mov sp,bp            ; Standart Teil

	        pop bp               ; Standart Teil

	        ret

          

;----------------------

; Diese Funktion liefert 1. den Hersteller-String der CPU und 2. die max. Anzahl an CPUID-Anfragen (h�chster EAX wert), die man auf 
; diesem PC machen kann.
; CPUID kann noch viel mehr, wurde hier aber nicht integriert!
; Bitte auch die Doku Beachten!

_CPUInfo:
    push bp			
    mov bp, sp		
    xor eax, eax
    mov eax, [bp + 4]				; eax wird dem Wert, der �bergeben wird zugewiesen
    cmp eax, 0h 				; Wenn eax = 99 -> Nur MaxEAX Wert f�r CPUId senden
    jz .hersteller				 
    cpuid
    mov esi, eax
    pop bp
    ret
  .hersteller: 
    cpuid 					; Gibt Infos �ber CPU				
    mov esi, CPUID_Buf			; In EBX, EDX und ECX sind Infos als String gespeichert
    mov [si], ebx				; Diese werden in CPUID geschrieben
    mov esi, CPUID_Buf+4
    mov [si], edx
    mov esi, CPUID_Buf+8
    mov [si], ecx
    add si, 4
    mov [si], byte 0
    mov esi, CPUID_Buf			; Die Addy von CPUID_Buf wird zur�ckgegeben ( x = CPUID (0) ) -> x = Addy von CPUID_Buf
    mov eax, esi                             ; Das 
    pop bp
    ret


;----------------------

; Section:     Kernel

; Unit:        System.asm

; Lib:         API

; Name:        reboot()

; Parameters:  None;

; Result:      None;

; Description:

;   This function reboots the PC.

; Written by: thesame, 01.06.03 

; 3 Verschieden M�glichkeiten, den PC zu rebooten :)



_reboot:

         cli           ; Deactivate Interrupts

         mov al, 0xFE

         out 0x64, al

         in al, 0x92   ; Get system access port

         iodelay       ; Wait until it's really done.

         or al, 1      ; Set Bit "0" to reboot more quickly

         out 0x92, al  ; Do it!

         

                mov eax,cr0                  ; F�r den Fall das 1. Versuch gescheitert ist

                mov ebx, 0x1                 ; Hier noch eine Methode

                not ebx

                and eax, ebx

                mov cr0,eax                  ; Aus dem PMode raus, Real Mode starten



                jmp 0xFFFF:0x0000            ; Real Mode Boot Methode

                ret

;--------------------

; sleep()



_sleep:   

       push bp          ;SLEEP COMMAND										

	   mov bp,sp

	   sub sp,0x40

	   mov eax, 0x38

	   mov gs, eax

	   mov eax,[bp+6]       ; eax = Sekundenanzahl

	   xor edx, edx

	   mov ecx, 0x64        

	   mul ecx              ; Sekundenanzahl * 100 [Hz]

	   mov ecx,[gs:0x100]   ; Die aktuelle Ticks-Anzahl

	   add eax, ecx         ; Aktuelle Ticks + zu wartenden Ticks

	   int 0x20

	   .loop

	   mov ecx,[gs:0x100]

	   cmp eax, ecx

	   ja .loop             ; Wiederhohle die Schleife, bis Ticks erf�llt sind

	   mov sp,bp

	   pop bp

    ret 

; liest eines der Control Register
_getcr:
            push bp	        	 ; Standart Teil. kA warum	      			
            mov bp,sp			 ; Standart Teil. kA warum	
	        sub sp,0x40    
	        mov dh, [bp+6]
	        or dh, dh
	        jz .cor0
	        dec dh
	        jz .ende
	        dec dh
	        jz .cor2
	        dec dh
	        jz .cor3
	        dec dh
	        jz .cor4
	        xor eax, eax
	        jmp short .ende
.cor0:
            mov eax, cr0
            jmp short .ende
.cor2:
            mov eax, cr2
            jmp short .ende
.cor3:
            mov eax, cr3
            jmp short .ende
.cor4:
            mov eax, cr4
.ende:
	        mov sp,bp
	        pop bp
	        ret



;-------------------------------------------------------------------------------

; Data

;-------------------------------------------------------------------------------

adrCreatDSstring     db 'Creating Data-Segment              : ', 0x00   

adrsDescSetstring    db 'Setting Up Descriptors             : ', 0x00

adrOKstring          db 'OK',0x0D, 0x0A,0x00

fname db 'LOADER  BIN'

savedstack db 2

scanfbuf db 64

firstboot db 2

CPUID_Buf DB '            ',0

timebuf db '        ',0

membuf db '     ',0

datebuf db '            ',0

intf db '    ',0

buffer1 db 0x0000

buffer2 db 0x0000

