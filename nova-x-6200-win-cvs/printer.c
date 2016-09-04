/*
	LPT1 & LPT2 Druckerfunktionen von MB Version v0.1
	Letzte Änderung: 18.08.2003
*/

#include "printer.h"

#define LPT1 1
#define LPT2 2
#define LPT3 3
#define OK 0
#define PRINTERERROR -80

short  LPT_dataport   [3] = {0x0278, 0x0378, 0x3BC};
short  LPT_statusport [3] = {0x0279, 0x0379, 0x3BD};
short  LPT_controlport[3] = {0x027A, 0x037A, 0x3BE};
NXbool LPT_exists     [3] = { false,  false, false};

NXbool isPrinterPortAvailable(NXint lptan) {
    int i;
    if (lptan < 1 || lptan > 3)
    {
        printf("unknown LPT Port!\n");
        return false;
    }
    nxoutport(LPT_dataport[lptan], 0);
    for (i=0; i<2000; i++) nxinport(0x80);
    if (nxinport(LPT_dataport[lptan]) == 0x00) return true; 
        else return false;
}

void initprinter() {  /* funktioniert perfekt! */
    int i, av = 0;
    printf("Checking LPT Ports...\n");
    for (i=1; i<4; i++)
        if (isPrinterPortAvailable(i)) { LPT_exists[i] = true; av++; }
    if (!av) printf("no LPT Ports found!\n");
    if (av == 1) printf("1 LPT Port found: ");
    if (av > 1) printf("%d LPT Ports found: ", av);
    if (av)
    {
        if (LPT_exists[1] == true) printf("LPT1 ");
        if (LPT_exists[2] == true) printf("LPT2 ");
        if (LPT_exists[3] == true) printf("LPT3");
        printf("\n");
    }
}
