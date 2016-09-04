#ifndef _FDC_H_
#define _FDC_H_

// interne FUnktionen

extern void enablefloppymotor(void);    /* in fdc.asm ausgelagert */
extern void disablefloppymotor(void);   /* in fdc.asm ausgelagert */ 
extern char isirq6(void);               /* in fdc.asm ausgelagert */
extern void clearirq6(void);            /* in fdc.asm ausgelagert */

void fdcreset(void);
void fdcrecal(void);
void initfdc(void);                     /* funktioniert perfekt! */
unsigned char getfloppytype(unsigned char drive);
void fdcwaitwithdio();
void sendbyte(int byte);
int getbyte();
void diskposition(int imgpos, DISKPOSITION *dp);
char fdcwait(char makeSIS);
void delay(unsigned long ms);
unsigned char fdcseek(unsigned int track);
void hexedit(unsigned long Positions);
void ReadSectorBETA(DISKPOSITION *fdpos, unsigned char FirstRead, unsigned char fdmotoroff);
char getFloppyType(unsigned char i);
int getnumberoffloppydrives();
char initalizingFDC();
void readtest(char Turbo);

#endif
