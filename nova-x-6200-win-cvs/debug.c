/*
   
    Debug Console von MB  
    einfach ESCAPE drücken, während Nova-X läuft und es wird zur Debug Console gesprungen

    Bitte alles so lassen wie es ist, auch sowas wie "0xB8000 + 8192 + 320" !!!
    (Dient zum besserem Verständnis)
    
*/

#ifndef _DEBUG_C_ 
#define _DEBUG_C_

#include "debug.h"

static unsigned int DEBUG_cursorpos = 0x00;
static unsigned char DEBUG_textcol = 0x07;

void DEBUG_cls()
{
    unsigned int abc;
    for (abc = 0; abc < 4096; abc+=2)
    {
        writeallmem(0xB8000 + 8192 + abc, 0x00); // Zeichen
        writeallmem(0xB8000 + 8192 + abc + 1, DEBUG_textcol); // Farbe
    }
}

void DEBUG_printchar(char ch)
{
    writeallmem(0xB8000 + 8192 + DEBUG_cursorpos, ch);
    writeallmem(0xB8000 + 8192 + DEBUG_cursorpos + 1, DEBUG_textcol);
    DEBUG_cursorpos += 2;
}

void DEBUG_print(char *str)
{
    int abc = 0; 
    unsigned int cpos = 0; unsigned int xpos = 0; unsigned int ypos = 0;
    while (true)
    {
        if (*(str+abc) == 0) break;
        if (*(str+abc) == '\n') 
        {
                xpos = DEBUG_getcursorxpos();
                ypos = DEBUG_getcursorypos();
                DEBUG_gotoxy(1, ypos+1);
        } else DEBUG_printchar(*(str+abc));
        DEBUG_scroll();
        abc++;
    }
}

void DEBUG_init()
{
    //Init
    DEBUG_cls();
    DEBUG_textcolor(12);
    DEBUG_print("Nova-X Debug Console v0.1\n");
    DEBUG_textcolor(15);
    DEBUG_print("-------------------------\n\n");
    DEBUG_textcolor(7);
    DEBUG_print("Press TAB to leave the Debug Console!\n");
}

void DEBUG_gotoxy(unsigned char xp, unsigned char yp)
{
    yp--; xp--; if (yp > 24) yp = 24; if (xp > 79) xp = 79;
    DEBUG_cursorpos = ((yp * 160) + (2 * xp));
}

unsigned short DEBUG_getcursorxpos()
{
    return ((DEBUG_cursorpos - ((DEBUG_cursorpos / 160) * 160)) / 2) + 1;
}

unsigned short DEBUG_getcursorypos()
{
    return (DEBUG_cursorpos / 160) + 1;
}

void DEBUG_textcolor(unsigned char col) /* setzt die Textfarbe für die DC */
{
    DEBUG_textcol = (DEBUG_textcol & 0xF0) + col;
}

void DEBUG_setbackground(unsigned char col) /* setzt die Hintergrundfarbe für die DC */
{
    DEBUG_textcol = (DEBUG_textcol & 0x0F) + (col *  16);
}

void DEBUG_scroll() /* Debug Console scrollen, dabei bleiben die ersten beiden Zeilen stehn */
{
    unsigned int abc;
    if (DEBUG_getcursorypos() > 24)
        {
        for (abc = 0; abc < 3840; abc++)
        {
                writeallmem(0xB8000 + 8192 + 320 + abc, readallmem(0xB8000 + 8192 + 480 + abc));
        }
        DEBUG_gotoxy(1,24);
        }
}

void switchtoshell()
{
    isdebugconsole = 0;
    nxoutport(0x3D4, 0x0C);
    nxoutport(0x3D5, 0);
    nxoutport(0x3D4, 0x0D);
    nxoutport(0x3D5, 0);
}

void switchtodebugconsole()
{
    isdebugconsole = 1;
    nxoutport(0x3D4, 0x0C);
    nxoutport(0x3D5, 16);
    nxoutport(0x3D4, 0x0D);
    nxoutport(0x3D5, 0);
}

#endif
