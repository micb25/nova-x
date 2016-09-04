@ ECHO OFF

:: In den folgenden 3 Zeilen bitte die Verzeichnisse für die Programme angeben
::
set gcclink="C:\DJGPP\bin\gcc.exe"
set ldlink="C:\DJGPP\bin\ld.exe"
set nasmlink="C:\nasmw.exe"
::
::
IF NOT EXIST %gcclink% goto nogcc
IF NOT EXIST %ldlink% goto nold
IF NOT EXIST %nasmlink% goto nonasm
IF NOT EXIST ".\rawwrite\rawwritewin.exe" goto noraw
del system.bin
del log.txt
del formatxFS.exe
cls
echo. 
echo  ##############################################################################
echo  #                               nova-x installer v0.2                        #
echo  ##############################################################################
echo.
echo                                   nova-X CVS release
echo.
echo Creating bin-files...
%nasmlink% -oboot.bin boot.asm
echo done...
echo Creating o-files...
%nasmlink% -f aout -osystem1.o system.asm
%nasmlink% -f aout -oprotmode.o protmode.asm
%nasmlink% -f aout -ovideo.o video.asm
%nasmlink% -f aout -ofdc.o fdc.asm
%nasmlink% -f aout -okeyboard.o keyboard.asm
%nasmlink% -f aout -omm.o mm.asm
%nasmlink% -f aout -omath.o math.asm
%nasmlink% -f aout -otss.o tss.asm
echo done...
echo Creating LOADME.COM...
%nasmlink% loadme.asm -o loadme.com
echo done...
echo Compiling C-Code

%gcclink% -fforce-mem  -fomit-frame-pointer -c -fwritable-strings -Wpointer-arith -fno-builtin    "system.c" -o "system2.o" 
%gcclink% -c -fwritable-strings "protmode.c" -o "pmc.o" 
%gcclink% -c -fwritable-strings "credits.c" -o "credits.o" 
%gcclink%  "formatxFS\format.c" -o "formatxFS.exe"

echo done... 
echo.
echo please insert an empty disk into drive A: and press any key!
PAUSE
echo Linking Files...

%ldlink% -script=linker.txt -n -s -o "SYSTEM.bin" 

echo done... 
echo Creating Image File "disk.img"
formatxFS.exe
echo Writing image to floppy

:
:	ACHTUNG!! WENN AUTOMATISCH AUF DISKETTE GESCHRIEBEN WERDEN SOLL,
:	DANN "ECHO" entfernen!
:
rawwrite\rawwritewin.exe --write disk.img
:
:
echo nova-x Installation complete...
echo.
echo for more support visit: http://www.nova-x.net
echo.
GOTO ende

:nogcc
echo Error! GCC path is wrong! Please edit the install.bat!
GOTO ende

:nold
echo Error! LD path is wrong! Please edit the install.bat!
GOTO ende

:nonasm
echo Error! NASM path is wrong! Please edit the install.bat!
GOTO ende

:noraw
echo Error! RAWWRITE path is wrong! Please edit the install.bat!
GOTO ende

:ende
PAUSE
