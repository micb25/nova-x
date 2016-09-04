#include "stdarg.h"
#include "time.h"
#include "video.h"

char strInt2StrBuffer[12]; // a little bit sloopy, but works fine 
va_list ap;
const char *array[31], *pap;
unsigned int vars;
int argno = 0;

/* Gibt ein Char binär aus */
void outputBinaryc(unsigned char bzahl) {
	if ((bzahl - 128) >= 0 ) {putchar('1'); bzahl = bzahl - 128;} else  {putchar('0');};
	if ((bzahl - 64) >= 0 )  {putchar('1'); bzahl = bzahl - 64;}  else  {putchar('0');};
	if ((bzahl - 32) >= 0 )  {putchar('1'); bzahl = bzahl - 32;}  else  {putchar('0');};
	if ((bzahl - 16) >= 0 )  {putchar('1'); bzahl = bzahl - 16;}  else  {putchar('0');};
	if ((bzahl - 8) >= 0 )   {putchar('1'); bzahl = bzahl - 8;}   else  {putchar('0');};
	if ((bzahl - 4) >= 0 )   {putchar('1'); bzahl = bzahl - 4;}   else  {putchar('0');};
	if ((bzahl - 2) >= 0 )   {putchar('1'); bzahl = bzahl - 2;}   else  {putchar('0');};
	if ((bzahl - 1) >= 0 )   {putchar('1'); bzahl = bzahl - 1;}   else  {putchar('0');};
}

/* Gibt ein Integer Binär aus */
void outputBinary(unsigned int bzahl) {
    outputBinaryc(bzahl / 256);
    outputBinaryc(bzahl-((bzahl / 256)* 256));
}

void outputBinaryl(unsigned long bzahl) {
    outputBinary(bzahl / 65535);
    outputBinary(bzahl - ((bzahl / 65535) * 65535));
}

/*  HEXDEZIMALE Zahlen ausgeben*/
void outputHexc(unsigned char hzahl) {
	char tempch;
	if (hzahl / 16 == 0) { putchar('0');tempch = 0;}
	if (hzahl / 16 == 1) { putchar('1');tempch = 1;}
	if (hzahl / 16 == 2) { putchar('2');tempch = 2;}
	if (hzahl / 16 == 3) { putchar('3');tempch = 3;}
	if (hzahl / 16 == 4) { putchar('4');tempch = 4;}
	if (hzahl / 16 == 5) { putchar('5');tempch = 5;}
	if (hzahl / 16 == 6) { putchar('6');tempch = 6;}
	if (hzahl / 16 == 7) { putchar('7');tempch = 7;}
	if (hzahl / 16 == 8) { putchar('8');tempch = 8;}
	if (hzahl / 16 == 9) { putchar('9');tempch = 9;}
	if (hzahl / 16 == 10) { putchar('A');tempch = 10;}
	if (hzahl / 16 == 11) { putchar('B');tempch = 11;}
	if (hzahl / 16 == 12) { putchar('C');tempch = 12;}
	if (hzahl / 16 == 13) { putchar('D');tempch = 13;}
	if (hzahl / 16 == 14) { putchar('E');tempch = 14;}
	if (hzahl / 16 == 15) { putchar('F');tempch = 15;}
	hzahl = hzahl - (16*tempch);
	if (hzahl == 0) { putchar('0');}
	if (hzahl == 1) { putchar('1');}
	if (hzahl == 2) { putchar('2');}
	if (hzahl == 3) { putchar('3');}
	if (hzahl == 4) { putchar('4');}
	if (hzahl == 5) { putchar('5');}
	if (hzahl == 6) { putchar('6');}
	if (hzahl == 7) { putchar('7');}
	if (hzahl == 8) { putchar('8');}
	if (hzahl == 9) { putchar('9');}
	if (hzahl == 10) { putchar('A');}
	if (hzahl == 11) { putchar('B');}
	if (hzahl == 12) { putchar('C');}
	if (hzahl == 13) { putchar('D');}
	if (hzahl == 14) { putchar('E');}
	if (hzahl == 15) { putchar('F');}
}

void outputHex(unsigned long hzahl) {
    if (hzahl > 0xFFFF)
        outputHex(hzahl / 0x10000); // Damit auch Long Zahlen dargestellt werden
    outputHexc(hzahl / 256);
    outputHexc(hzahl - ((hzahl / 256) * 256));
}

void outputHexl(unsigned long lzahl) {
    outputHex(lzahl / 65535);
    outputHex(lzahl - ((lzahl / 65535) * 65535));
}

/**
 * Outputs a string
 * @param 
 */
void outputString(char* strText) {  
    while (*strText)
        outputCharacter(*strText++);
}

void outputCharacter(char cCharacter) {
    if (cCharacter == '\n')
        gotoxy(1, wherey() + 1);
    else
        if (cCharacter == '\r') { }
        else putchar(cCharacter);
}

/**
 * Outputs an integer value
 * @param	iValue	the integer value what you'd like to output
 */
void outputInteger(long iValue) {
	/* data block */
	char *strString;
	unsigned long uiValue = 0; /* the positive value of iValue */
	long si;
	strString = &strInt2StrBuffer[12];
	/* setup */
	*strString = 0;
	strInt2StrBuffer[12] = 0;
	strInt2StrBuffer[11] = 0;
	if (iValue < 0) {uiValue = -iValue; outputCharacter('-');}else uiValue = iValue;
/*	ms();*/

	while (uiValue) {
	   	strString--;
		*strString = (uiValue % 10) + '0';
		uiValue = uiValue / 10;
	} 
	/* output char array */
	if (iValue == 0)
        outputCharacter('0');
    else
        outputString(strString);
}

void printf(char *string, ...) {
    // Gibt einen formatierten String aus.
    va_start(ap, string);       
    while(*string) {        
        if (*string == '\n') {
            gotoxy(1, wherey() + 1);
        } else if (*string == '\r') { }
        else if (*string == '%') {
            // Dezimalwert
            if (*(string+1) == 'd') {
                vars = va_arg(ap,unsigned int);
                outputInteger(vars);
                string++;
            }
            // Hexadezimalwert
            if (*(string+1) == 'x'){
                vars = va_arg(ap,unsigned int);
                outputHex(vars);
                string++;
            }           
            // Binärwert
            if (*(string+1) == 'b'){
                vars = va_arg(ap,unsigned int);
                outputBinary(vars);
                string++;
            }  
            // Char
            if (*(string+1) == 'c'){
                vars = va_arg(ap, unsigned int);
                if ((*(string + 2) >= '0') && (*(string + 2) <= '9')) {
                    int i = 0;
                    for (; i < (*(string + 2) - 48); i++) {
                       putchar(vars);
                    }
                    string++;
                } else {
                    putchar(vars);
                }
                string++;
            }  
            // Time-Struct
            if (*(string+1) == 't'){
                vars = va_arg(ap, unsigned int);
                outputTime((t_time*)vars);
                string++;
            }                   
            // Date-Struct     
            if (*(string+1) == 'a'){
                vars = va_arg(ap, unsigned int);
               outputDate((t_date*)vars);
                string++;
            }             
            // string   
            if (*(string+1) == 's'){
                vars = va_arg(ap, unsigned int);
                outputString((char*)vars);
                string++;
            }
	    // Prozentzeichen
	    if (*(string+1) == '%'){
		outputString("%");
		string++;
	    }            
        } else putchar(*string);
        string++;
    }
    va_end(ap);
}

int getintlen(int integer) /* Gibt die Länge einer Zahl(Zeichen) zurück */
{
    if (integer < -9999) return 6;
    if (integer < -999) return 5;
    if (integer < -99) return 4;
    if (integer < -9) return 3;
    if (integer < 0) return 2;
    if (integer > 9999) return 5;
    if (integer > 999) return 4;
    if (integer > 99) return 3;
    if (integer > 9) return 2;
    if (integer > 0) return 1;
}

int getuintlen(unsigned int integer)
{
    if (integer > 999999) return 7;
    if (integer > 99999) return 6;
    if (integer > 9999) return 5;
    if (integer > 999) return 4;
    if (integer > 99) return 3;
    if (integer > 9) return 2;
    if (integer > 0) return 1;
}
