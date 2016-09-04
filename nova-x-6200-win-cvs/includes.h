/*

    Includes für den Kernel!
    
    Hier alle Files eintragen, die der Kernel beim Compilieren mit einbinden soll!

*/

// Typendefinitionen in Nova-X
#include "typedefs.h"
// Funktionen zur Ausgabe 
#include "video.c"
// Funktionen zur Eingabe
#include "keyboard.c"
// verschiedene Befehle
#include "commands.c"
//#include "manual.c"

// Befehle für die Stringverarbeitung
#include "string.c"
// Memory Management
#include "mm.c"
// FDC Treiber
#include "fdc.c"
// Exception-Handler
#include "exceptions.c"
// Zeitfunktionen
#include "time.c"
// CMOS Funktionen
#include "cmos.c"
// LPT Funktionen
#include "printer.c"
// CPU Funktionen
#include "cpu.c"
// Debug Console
#include "debug.c"
// mathematische Funktionen
#include "math.c"
// FDC Treiber
#include "nxFS.c" 
// #include "nsh.c"

// für den gewissen Sound ;)
#include "speaker.c"
