#include "keyboard.h"
#include "debug.h"

char Keyboardmap[150][2];

/* Variabeln die intern verwendet werden */
unsigned char
    buf_size,
    aktchar,
    num,
    ende,
    entered_char,
    special_key = 0,
    shift_stat = 0,
    nchars,
    num_stat = 1,
    scroll_stat = 0,
    caps_lock_stat = 0;
    
int gets(unsigned char *fmt) {
    entered_char = 1;
    nchars = 0;
    while (entered_char != 13) {
        entered_char = getchar();
        if (entered_char == 9) /* TAB - Debug Console */
                { if (isdebugconsole == 1) switchtoshell(); else switchtodebugconsole(); }
        if (entered_char == 8 && nchars > 0) { /* BACKSPACE Taste */
            gotoxy(wherex() - 1, wherey());
            putchar(' ');
            gotoxy(wherex() - 1, wherey());
            *fmt = 0;
            fmt--;
            nchars--;
        } 
        if (entered_char > 31) { /* Zeichen ausgeben */
            putchar(entered_char);
            *fmt = entered_char;      /* Zeichen ausgeben */
            fmt++; 
            nchars++;
        }
    }
    *fmt = 0;
    return nchars;
}

/* Funktion wird nur intern verwendet */
void setLEDs(
    unsigned char shift,
    unsigned char num,
    unsigned char scroll)
{ /* Setzt die Leuchtdioden auf der Tastatur */
    int temp = 0;
    temp = temp + shift;        
    temp = temp << 1;
    temp = temp + num;
    temp = temp << 1;
    temp = temp + scroll;
    nxoutport(0x60, 0xED);   /* An den Tastatatur-Controller senden */
    nxoutport(0x60, temp);
    nxoutport(0x60, 0xF4);
}

/* Hier werden Tasten wie Shift, Num, o.ä. bestimmt und ausgeführt */
void checkspecialkeys() {
    if ((aktchar == SCANCODE_LEFT_SHIFT) || (aktchar == SCANCODE_RIGHT_SHIFT)) {  /* Shift Taste ist unten */
        aktchar = 0;
        shift_stat = 1; 
        num++;
        setLEDs(caps_lock_stat, num_stat, scroll_stat);
    } else if (aktchar == SCANCODE_SHIFT_LOCK) { /* Shift Lock Taste ist unten */
        aktchar = 0;
        shift_stat ^= 1; /* Shift-Status umtauschen! */
        caps_lock_stat ^= 1;
        num++; 
        setLEDs(caps_lock_stat, num_stat, scroll_stat);
    } else if ((aktchar == SCANCODE_LEFT_SHIFT + 128) || (aktchar == SCANCODE_RIGHT_SHIFT + 128)) { /* Wenn Shift Taste losgelassen wurde */
        aktchar = 0;
        shift_stat = 0;  /* Shift Modus aus */
        num++; 
        setLEDs(caps_lock_stat, num_stat, scroll_stat);
    } else if (aktchar == SCANCODE_LEFT_CTRL || aktchar == SCANCODE_RIGHT_CTRL) { /* Control Taste */
        aktchar = 0;
        num++;      
    } else if (aktchar == SCANCODE_ALT || aktchar == SCANCODE_ALT_GR) { /* Alt Taste */
        aktchar = 0;
        num++;      
    } else if (aktchar == 224) {  /* Spezial Taste z.B. Pfeiltaste */
        special_key = 1;
    } else if (aktchar == SCANCODE_NUM) {      /* NUM taste */
        num_stat ^= 1;
        num++;      
        setLEDs(caps_lock_stat, num_stat, scroll_stat);
    } else if (aktchar == SCANCODE_SCROLL) {
        scroll_stat ^= 1;
        num++;      
        setLEDs(caps_lock_stat, num_stat, scroll_stat);
    } 
}

/* getchar Funktion */
/* Liest ein Zeichen aus dem Tastaturbuffer ein */
/* Rückgabewert ist der ASCII Code des Zeichens */
/* Gibt es keinen ASCII Code, ist ist der Rückgabewert 0 */
unsigned char getchar() {
    setCursorPosition();    /* Cursor setzten */
    ende = 0;               /* Alles auf null */
    num = 0;
    aktchar = 0; 
    special_key = 0;
    
    while (ende != 1) {
        buf_size = getkeybbufsize();
        if (num > buf_size) {
            initkeyb();
            num = 0;
        }
        if (buf_size != 0) {    /* Ist ein Code im Buffer??? */
            aktchar = getkeybbufchar(num + 1);  /* Scancode lesen */
            if (special_key != 0) { 
                if (aktchar == 53) {
                    aktchar = SCANCODE_NB_DIV;
                } else if (aktchar == 28) {
                    aktchar = SCANCODE_RETURN;
                } else {
                    aktchar = 0;
                }      /* Mache nix */
                num++;
                special_key++; 
                if (special_key > 3)
                    special_key = 0;      /* Bei einer Spezal Taste werden mehrere codes gesendet! */
            }
            checkspecialkeys(); 
            if ((aktchar != 0 && aktchar < 128) || (aktchar == 180)) {
                /* Ist es ein "normales" Zeichen */
                if (aktchar >= 71 && aktchar <= 83) {
                    aktchar = Keyboardmap[aktchar][num_stat + 1];
                } else
                    aktchar = Keyboardmap[aktchar][shift_stat + 1]; /* ASCII Code des Make Codes bestimmen */
                //printf("\naktchar: %d",aktchar); 
                ende = 1; /* Ende der Schleife */
                num = 0;
                initkeyb(); /* Keyboard-Buffer löschen */ }
                num++;
        }
    } /* WHILE Schleife */
    return aktchar;  /* ASCII Code zurückgeben */
}

/**
 * Converts a scancode-code to an ASCII-character
 * @param	FirstValue	
 *			SeconsValue		CTRL, ALT, SHIFT, etc.
 * @return	Returns the ASCII-character
 */

void initkbmap() {
    Keyboardmap[30][2] = ASCII_LARGE_A;
    Keyboardmap[30][1] = ASCII_SMALL_A; 

    Keyboardmap[48][2] = ASCII_LARGE_B;
    Keyboardmap[48][1] = ASCII_SMALL_B; 

    Keyboardmap[46][2] = ASCII_LARGE_C;
    Keyboardmap[46][1] = ASCII_SMALL_C; 

    Keyboardmap[32][2] = ASCII_LARGE_D;
    Keyboardmap[32][1] = ASCII_SMALL_D; 

    Keyboardmap[18][2] = ASCII_LARGE_E;
    Keyboardmap[18][1] = ASCII_SMALL_E; 
    
    Keyboardmap[33][2] = ASCII_LARGE_F;
    Keyboardmap[33][1] = ASCII_SMALL_F; 
    
    Keyboardmap[34][2] = ASCII_LARGE_G;
    Keyboardmap[34][1] = ASCII_SMALL_G; 
    
    Keyboardmap[35][2] = ASCII_LARGE_H;
    Keyboardmap[35][1] = ASCII_SMALL_H; 
    
    Keyboardmap[23][2] = ASCII_LARGE_I;
    Keyboardmap[23][1] = ASCII_SMALL_I; 
    
    Keyboardmap[36][2] = ASCII_LARGE_J;
    Keyboardmap[36][1] = ASCII_SMALL_J; 
    
    Keyboardmap[37][2] = ASCII_LARGE_K;
    Keyboardmap[37][1] = ASCII_SMALL_K; 
    
    Keyboardmap[43][2] = 39;
    Keyboardmap[43][1] = '#'; 
    
    Keyboardmap[27][2] = '*';
    Keyboardmap[27][1] = '+'; 
    
    Keyboardmap[13][2] = '´';
    Keyboardmap[13][1] = '´'; 
    
    Keyboardmap[12][2] = '?';
    Keyboardmap[12][1] = 'ß'; 
    
    Keyboardmap[38][2] = ASCII_LARGE_L;
    Keyboardmap[38][1] = ASCII_SMALL_L; 
    
    Keyboardmap[50][2] = ASCII_LARGE_M;
    Keyboardmap[50][1] = ASCII_SMALL_M; 
    
    Keyboardmap[49][2] = ASCII_LARGE_N;
    Keyboardmap[49][1] = ASCII_SMALL_N; 
    
    Keyboardmap[24][2] = ASCII_LARGE_O;
    Keyboardmap[24][1] = ASCII_SMALL_O; 
    
    Keyboardmap[25][2] = ASCII_LARGE_P;
    Keyboardmap[25][1] = ASCII_SMALL_P; 
    
    Keyboardmap[16][2] = ASCII_LARGE_Q;
    Keyboardmap[16][1] = ASCII_SMALL_Q; 
    
    Keyboardmap[19][2] = ASCII_LARGE_R;
    Keyboardmap[19][1] = ASCII_SMALL_R; 
    
    Keyboardmap[31][2] = ASCII_LARGE_S;
    Keyboardmap[31][1] = ASCII_SMALL_S; 
    
    Keyboardmap[20][2] = ASCII_LARGE_T;
    Keyboardmap[20][1] = ASCII_SMALL_T; 
    
    Keyboardmap[22][2] = ASCII_LARGE_U;
    Keyboardmap[22][1] = ASCII_SMALL_U; 
    
    Keyboardmap[47][2] = ASCII_LARGE_V;
    Keyboardmap[47][1] = ASCII_SMALL_V; 
    
    Keyboardmap[17][2] = ASCII_LARGE_W;
    Keyboardmap[17][1] = ASCII_SMALL_W; 
    
    Keyboardmap[45][2] = ASCII_LARGE_X;
    Keyboardmap[45][1] = ASCII_SMALL_X; 
    
    Keyboardmap[44][2] = ASCII_LARGE_Y;
    Keyboardmap[44][1] = ASCII_SMALL_Y; 
    
    Keyboardmap[21][2] = ASCII_LARGE_Z;
    Keyboardmap[21][1] = ASCII_SMALL_Z; 
    
    Keyboardmap[2][2] = ASCII_EXCLAMATION_MARK;
    Keyboardmap[2][1] = ASCII_1;
    
    Keyboardmap[3][2] = ASCII_QUOTATION_MARK;
    Keyboardmap[3][1] = ASCII_2;
    
    Keyboardmap[4][2] = ASCII_PARAGRAPH;
    Keyboardmap[4][1] = ASCII_3;
    
    Keyboardmap[5][2] = ASCII_DOLLAR;
    Keyboardmap[5][1] = ASCII_4;
    
    Keyboardmap[6][2] = ASCII_PER_CENT;
    Keyboardmap[6][1] = ASCII_5;
    
    Keyboardmap[7][2] = ASCII_AND_MARK;
    Keyboardmap[7][1] = ASCII_6;
    
    Keyboardmap[8][2] = ASCII_SLASH;
    Keyboardmap[8][1] = ASCII_7;
    
    Keyboardmap[9][2] = ASCII_SPECIAL_1;
    Keyboardmap[9][1] = ASCII_8;
    
    Keyboardmap[10][2] = ASCII_SPECIAL_2;
    Keyboardmap[10][1] = ASCII_9;
    
    Keyboardmap[11][2] = ASCII_EQUAL_MARK;
    Keyboardmap[11][1] = ASCII_0;
    
    Keyboardmap[26][2] = ASCII_LARGE_UE;
    Keyboardmap[26][1] = ASCII_SMALL_UE;
    
    Keyboardmap[40][2] = ASCII_LARGE_AE;
    Keyboardmap[40][1] = ASCII_SMALL_AE;
    
    Keyboardmap[39][2] = ASCII_LARGE_OE;
    Keyboardmap[39][1] = ASCII_SMALL_OE;
    
    Keyboardmap[51][2] = ASCII_SEMICOLON;
    Keyboardmap[51][1] = ASCII_COMMA;
    
    Keyboardmap[180][2] = ASCII_DOUBLE_POINT;
    Keyboardmap[180][1] = ASCII_POINT;
    
    Keyboardmap[53][2] = '_';
    Keyboardmap[53][1] = ASCII_SUBTRACT;
    
    Keyboardmap[86][2] = ASCII_BIGGER;
    Keyboardmap[86][1] = ASCII_SMALLER;
    
    Keyboardmap[15][2] = ASCII_HORIZONTAL_TAB;
    Keyboardmap[15][1] = ASCII_HORIZONTAL_TAB;
    
    Keyboardmap[1][2] = ASCII_ESCAPE;
    Keyboardmap[1][1] = ASCII_ESCAPE;
    
    Keyboardmap[28][2] = ASCII_CARRIAGE_RETURN;
    Keyboardmap[28][1] = ASCII_CARRIAGE_RETURN;
    
    Keyboardmap[57][2] = ASCII_SPACE;
    Keyboardmap[57][1] = ASCII_SPACE;
    
    Keyboardmap[14][2] = ASCII_BACKSPACE;
    Keyboardmap[14][1] = ASCII_BACKSPACE;
    
    Keyboardmap[SCANCODE_NB_1_END][1] = ASCII_END_OF_TEXT;
    Keyboardmap[SCANCODE_NB_1_END][2] = ASCII_1;
    
    Keyboardmap[SCANCODE_NB_2_DOWN][1] = 0;
    Keyboardmap[SCANCODE_NB_2_DOWN][2] = ASCII_2;
    
    Keyboardmap[SCANCODE_NB_3_PDOWN][2] = ASCII_3;
    Keyboardmap[SCANCODE_NB_3_PDOWN][1] = 0;
    
    Keyboardmap[SCANCODE_NB_4_LEFT][2] = ASCII_4;
    Keyboardmap[SCANCODE_NB_4_LEFT][1] = 0;
    
    Keyboardmap[SCANCODE_NB_5][2] = ASCII_5;
    Keyboardmap[SCANCODE_NB_5][1] = 0;
    
    Keyboardmap[SCANCODE_NB_6_RIGHT][2] = ASCII_6;
    Keyboardmap[SCANCODE_NB_6_RIGHT][1] = 0;
    
    Keyboardmap[SCANCODE_NB_7_POS1][2] = ASCII_7;
    Keyboardmap[SCANCODE_NB_7_POS1][1] = 0;
    
    Keyboardmap[SCANCODE_NB_8_UP][2] = ASCII_8;
    Keyboardmap[SCANCODE_NB_8_UP][1] = 0;
    
    Keyboardmap[SCANCODE_NB_9_PUP][2] = ASCII_9;
    Keyboardmap[SCANCODE_NB_9_PUP][1] = 0;
    
    Keyboardmap[SCANCODE_NB_0_INS][2] = ASCII_0;
    Keyboardmap[SCANCODE_NB_0_INS][1] = 0;
    
    Keyboardmap[SCANCODE_NB_COMMA][2] = ',';
    Keyboardmap[SCANCODE_NB_COMMA][1] = 0;
    
    Keyboardmap[SCANCODE_NB_ENTER][2] = 13;
    Keyboardmap[SCANCODE_NB_ENTER][1] = 13;
    
    Keyboardmap[SCANCODE_NB_ADD][2] = '+';
    Keyboardmap[SCANCODE_NB_ADD][1] = '+';
    
    Keyboardmap[SCANCODE_NB_SUB][2] = ASCII_SUBTRACT;
    Keyboardmap[SCANCODE_NB_SUB][1] = ASCII_SUBTRACT;
    
    Keyboardmap[SCANCODE_NB_MUL][2] = '*';
    Keyboardmap[SCANCODE_NB_MUL][1] = '*';
    
    Keyboardmap[SCANCODE_NB_DIV][2] = '/';
    Keyboardmap[SCANCODE_NB_DIV][1] = '/';
}
