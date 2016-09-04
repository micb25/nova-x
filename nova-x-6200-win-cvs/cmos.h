#ifndef _CMOS_H_
#define _CMOS_H_

/* Liest die Gr��e des erweiterten Speichers in KB */
int getExtendedmemsize();
/* Liest die Gr��e des Basisspeichers in KB */
int getBasememsize();
// Liest den Wert eines Ports des CMOS-Chips ein und gibt ihn zur�ck
char readfromcmos(char port);
// Sendet den Wert "byte" and den Port "port" des CMOS-Chips
void writetocmos(char port, char byte);
// �berpr�ft den CMOS-Chip und gibt evtl. Fehler aus
void checkcmos();
// Gibt aus, ob eine FPU existiert
void checkFPU();

#endif
