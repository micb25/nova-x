/*
   
    Debug Console von MB  
    einfach TAB drücken, während Nova-X läuft und es wird zur Debug Console gesprungen
    
*/

#ifndef _DEBUG_H
#define _DEBUG_H

/* die Funktionen der Debug Console */
void DEBUG_print(char *str);
void DEBUG_init();
void DEBUG_cls();
void DEBUG_scroll();
void DEBUG_printchar(char ch);
void DEBUG_gotoxy(unsigned char xp, unsigned char yp);
void DEBUG_textcolor(unsigned char col);
void DEBUG_setbackground(unsigned char col);
unsigned short DEBUG_getcursorxpos();
unsigned short DEBUG_getcursorypos();
void switchtoshell();
void switchtodebugconsole();

static int isdebugconsole = 0;

#endif
