#ifndef _VIDEO_H
#define _VIDEO_H

void outputString(char*);
void outputCharacter(char cCharacter);
void printf(char *string, ...); 
void outputBinaryc(unsigned char bzahl);
void outputBinary(unsigned int bzahl);
void outputBinaryl(unsigned long bzahl);
void outputHexc(unsigned char hzahl);
void outputHex(unsigned long hzahl);
void outputHexl(unsigned long lzahl);
void outputString(char* strText);
void outputCharacter(char cCharacter);
void outputInteger(long iValue);
extern void messageprotmode();
extern void setIDT();
extern void messageOK();
extern void enterProtectedMode();
extern void gotoxy(char, char);
extern void textcolor(char);
extern char putchar(char);
int getintlen(int integer);
int getuintlen(unsigned int integer);

#endif
