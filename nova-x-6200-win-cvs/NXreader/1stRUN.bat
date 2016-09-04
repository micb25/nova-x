ECHO off
ren compile.xFS compile.bat
cd ..
copy nasmw.exe NXreader\nasmw.exe
copy Jloc.exe  NXreader\jloc.exe
copy tcc.exe NXreader\tcc.exe

copy nxFS.h NXreader\nxFS.h
copy novaX.c NXreader\novaX.c
copy novaX.h NXreader\novaX.h

cd NXreader
echo Now you can compile this modul. Type "compile"
del 1stRun.bat
