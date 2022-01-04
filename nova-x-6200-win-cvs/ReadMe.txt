Nova-X History
--------------------

--------------------
Nova-X 0.0.6 Build 6200 CVS
	12-18.8.03... ongoing
***************************
Hallo.

Diese Notiz nur, um die Verwirrung um Versionen etwas einzudämmen.
Hier steht jetzt auch keine neue Errungenschaft (obschon es davon
einige gibt). Sinnvollerweise liest man für die aktuellen Veränderungen
und Erweiterungen die Datei UpdatesCVS.txt

mfg sseitz
---------------------

Ein grösserer Versionssprung

---------------------
Nova-X 0.0.6 Build 6100
   	15.7.03 - 17:15
***********************
Hallo.

Heute präsentieren wir euch das neuste Release : Nova 0.0.6 X  Built 6100. 
Leider funktionierte die letzte Version bei vielen auf einem echten PC 
nicht (im Emulator aber schon)  dies sollte bei dieser Version nicht der Fall sein. 
Erfreulich sind die vielen neuen und wichtigen Änderungen.
Alle Programmierer bitte ich diese Version zu verwenden, 
damit wir alle auf einem Stand sind!
Folgendes hat sich verändert:
->	Verbesserter Bootvorgang  Hardwareerkennung :)
->	Floppy Disk Treiber-Ansätzte (mit DMA!)
->	malloc()/free()  kleines Memory Management
->	time-Funktionen->	neue Expection Handler
->	keyboard-Map wurde erweitert (Sonderzeichen)
->	printf()  Funktion 
->	stdarg.h kann verwendet werden
->	Pointer ++ Problem wurde behoben
->	Dynamische Positionierung des Data-Segments in der system.bin
->	strcmp() wurde gefixt
->	format.c wurde überarbeitet
->	u.v.m.

Meiner Meinung nach eines der besten Releases! 

Wir hoffen, dass demnächst unser CVS Server steht und wir somit besser parallel 
an Nova X programmieren können.
- Nicht vergessen die install.bat zu editieren! 
- PS.: Wir haben eine neue Dokumentation: http://www.visual-mb.de/nova-x/doku/
Euer Nova-X Team
-----------------------

-----------------------
Nova-X 0.0.6 Build 6099
   	27.6.03 - 18:00
************************
Hallo. 

Heute präsentieren wir euch die erste Nova X Version, die nicht mehr den veralteten
tcc Compiler und Jloc Linker verwendet, sondern gcc und ld. Durch diesen Umstieg ist
es uns nun auch möglich Nova X unter Linux zu entwickeln. 

Veränderungen:
-> kompletter Umstieg auf GCC und LD
-> BOCHS kompatibel
-> Alle Boot Bugs behoben
-> Keyboard Treiber: Es wurde eine KeyboardMap eingeführt (keine "Switch-Case" Anweisung mehr)
-> viele kleine Änderungen an Funktionen
-> Memory Mangager - Ansätze
 
Der GCC Compiler unterscheidet sich zum Teil stark vom TCC. 
Es muss folgendes beachtet werden:
- "..." : Die 3 Punkte können nicht mehr in Schleifen benutzt werden
- Bei "Switch-Case" - Anweisungen mit mehr als 4 Case Befehlen, kann es zu Fehlern kommen!

mfg Hornet
-------------------

Ein kleiner Versionssprung

-------------------

Nova-X 0.0.6 Build 6002
************************
Hallo.

Mit leichter Verspätung präsentieren wir euch heute Build 6002. Die Entwicklung
hat über 3 Wochen gedauert - aber es hat sich gelohnt. Nun bootet Nova-X komplett im 
Protected Mode und die wichtigsten APIs wurden portiert. Aber es ist noch eine Menge zu
tun. So muss noch ein ein Fopyy + HDD Treiber geschrieben werden, um das Dateisystem 
nutzen zu können.

Veränderungen:
-> Protected Mode - komplett -
-> Bochs kompatibel
-> Timer wurde integriert
-> Sleep() Funktion
-> protokollierter Bootvorgang
-> "stottern" des Diskettenlaufwerks wurde behoben!
--> booten geht schneller
-> vieles mehr

Einige Funktionen sind von mir nicht mehr integriert worden - aus Zeitgründen (z.B. die Registry).
Ich bitte die einzelnen Programmierer dies zu tun.

Zu beachten ist außerdem noch, dass sich einige APIs geändert haben (getchar,...) um der Norm
zu entsprechen! Vielleicht kann ja ein sich langweilender Programmierer schlecht oder ungenau beschriebene
Funktionen kommentieren.

Was einige C-Coder sicherlich freuen wird, ist der stark geschrumpfte Assembler-Bereich. Ich habe mich
bemüht, soviel wie möglich in C zu schreiben.

Was ist, was wird: 
Mit dieser Version wird, auch wenn nicht für jeden sichtbar, ein großer Schritt,
in Richtung Multitasking, GUI und ähnlichem gemacht. Aber es ist noch mehr nötig.
In nächster Zeit werden wir uns auf das Paging konzentrieren müssen, was der letzte Grundstein
für das Memory Managment ist. 

PS.: Es sind noch einige Bugs vorhanden!

by Vitali "Hornet" Müller - 25.02.2003 15:00
---------|	|---------

Nova-X 0.0.6 Build 6001
************************
Hallo.

Wir haben, wie angekündigt, xOS in Nova-X umbenannt. 

Außerdem sind einige neue Veränderungen und Verbesserungen hinzugekommen. 
Nova-X kann nun für die Programmierer in den Protected Mode geschaltet werden, 
um Protected-Mode APIs zu entwickeln. Wir hoffen die Portierung auf den 
Protected Mode so schnell wie möglich abschließen zu können.

Veränderungen:
-> xPad und registry.c wurden verbessert
-> xFS wurde in nxFS umbennant
-> eine neue Version von nxFS (mit Doku!)
--> fread und fopen API (Aplha)
-> xScan : Hiermit können Programmierer die Scan-Codes für bestimmte Tasten bestimmen
-> protmode : Startet den Protected Mode (Beenden nur durch reset möglich)
--> Int10 wurde z.T. in den Protected Mode konvertiert (Int20)
-> \Docu\ - Dir : Enthält Dokumentationen o.ä.
-> Einige anderen Funktionen/Module wurden umbennant
-> NovaX.dev für den Bloodshed C++ Editor (To-Do-Liste und mehr)!

by Vitali "Hornet" Müller - 02.02.2003 22:00
----------

xOS 0.0.6 FINAL

Änderungen im Design, Registry mit registrierbaren Modulen.

Funktionen:
-> xReg
-> xReg Scan
-> xPad "GUI"
-> Diverse PatchesWeitere Fixes:
-> Colorpatch im Reader
-> xPad nun Farbfähig <- wird wegen kompletter Änderung von xPad auf einen späteren Release verschoben.
-> xPad registriert Typ xtxt für xPad
-> Farbpatch im xOS Start- Logoby shaft
-----------

xOS 0.0.6 PRE

Dies ist die letzte Version vor "xOS 0.0.6". 

Wie schon zuvor wurdein dieser Version weiter das xFS ausgebaut. Es befindet sich nun ein txt
Reader auf dem xFS, mit dem es möglich ist, die gnulicense.txt zu lesen. Außerdem sind wieder
neue APIs hinzugefügt worden. 

Änderungen:
+ Neue APIs: WhereX, WhereY, CopyIntoMemory, ReadFromMemory, RunCodeFromMemory, Quit
+ formatxFS wurde verbessert: in der xFS.fil befindet sich eine Liste, in der alle auf das xFS
  zu kopierende Dateien aufgeführt sind.
+ xFS wurde weiter ausgebaut
+ Beenden des Splash Screens ist nun möglich
+ Interrupt 25 beendet von nun ein an Programm und kehrt zu xOS zurück
+ Modul: xOSReader für txt Dateien (Konsole: "GNU")
+ strings.c wurde verbessert

Die API DOkumentation wird auch demnächst upgedatet. Bitte macht uns auf Fehler irgend
welcher Weise aufmerksam. Danke :)

Beachtet bitte die Bugs.txt

by Vitali 'Hornet' Müller 18.01.2003 - 23:15
-------

Build 5008

In dieser Version wurde das xFS weiter ausgebaut und es gibt wieder neue
APIs. Ich möchte aber darauf hinweißen, dass diese Version unter Zeit-
druck von mir zusammengesetzt wurde und deshalb nicht so ausgiebig
getestet bzw. de-bugged werden konnte.

Also hier die Änderungen:
+ Bugfixes im xPad-Grungerüst
+ Bereinigung der system.asm von unnötigen APIs
+ 20 KB system.bin Grenze wurde entfernt
+ Oberfläche wurde leicht Verändert
+ Bugfixes in einigen APIs
+ shutdown API
+ listdir-Befehl
+ Splash-Screen frage wurde abgeschafft
--- Splash Screen kann durch "splash" in der konsole aufgerufen werden
--- ist aber leider noch nicht beendbar. Wird die ESC-Taste gedrückt, so 
--- rebootet der PC.
+ BETA Versionen von run-API und getfilename-API
+ gnulic.txt befindet sich nun auf dem xFS. Es wird demnächst einen Reader
--- in xFS dafür geben!
+ format.c Quellcode wurde erstmal rausgenommen

by Hornet 12.01.2003 - 21:46
-------

Build 5007

Endlich gibts xFS. Zwar kann es noch nicht mehr als booten, aber es wird
mehr - versprochen :)

Ausserdem einige neue APIs sowie Bugfixes u.v.m.
- Verbesserung der xSysInfo
+ Verzögerungsrate und Wiederhohlungsrate der Tastatur
+ Achtung- Bug! Bei öfterem ausführen von sys info, erscheint die Splash-Screen Frage.
+ Kleinigkeiten- getmemsize() und getextram() wurden umbenannt in getbasememsize() und getextmemsize()
- Neue APIs+getkeyboarddelay+getkeyboardrate
- xPad - Grundgerüst
- Entfernen der Splash-API
- xFS:
  + format.exe - erstellt ein Image im xFS Format
  + RawWrite - Schreibt Image auf die Diskette
  + Image bzw. Diskettengröße im moment auf 250 KB beschränkt,
    um schnelleres testen zu ermöglichen
  + Splash-Screen funktioniert nicht mehr (nächste Build)
  + Komplett neue und kommentierte Boot.asm (Bootsektor)
- Achtung: Die boot.asm beschränkt im moment die größe der system.bin auf 20 KB (wir sind bei 16KB)!
  Kurz vorm releasen ist dieser fehler aufgetreten, er sollte in der nächsten Build behoben werden.

by Hornet 05.01.2003 - 17:50
--------

Build 5006:

In meinen Augen, eine sehr gute Build.

Die Veränderungen kurz zusammengefasst.
- Credits auf Englisch (TheSame)
- Logo ausgelagert (Shaft)
- xSysInfo erweitert (TheSame, Hornet)
   + CMOS-Date
   + Base RAM
   + Extended RAM
   + CMOS Time (Sekunden fix)
- IntToStr-API (Hornet)
- Auf Deutsches Keyboard umgestellt: 'y' <-> 'z'  (Hornet)
- String-Funktionen in der string.c (Hannibal)  [funktionalität noch nicht erprobt :) ]

by Hornet 29.12.2002 - 22:51
-----

Build 5005:

Einige kleine Veränderungen an der credits.c sowie system.c.
Die manual-pages wurden ausgelagert.

by Hornet
