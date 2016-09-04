#ifndef _CPU_H_
#define _CPU_H_

#include "typedefs.h"

void cpuidfill(ulong COMMAND);                                  /* Füllt die CPU Register je nach Command */
void cpuPrintRegs();                                /* Zeigt den aktuellen Inhalt der Register */
void cpuPrintInfos();                               /* Infos */
void cpubootInfos();                                /* CPU boot Infos*/
int cpuFamily();					/* Gibt die CPU Family zurück */
int cpuModel();                                     /* Gibt das CPU Model zurück */
int cpuStepping();                                  /* Gibt die CPU Stepping zurück */
char* cpuVendor();                                  /* Hersteller Vendor String */
char *cpuType();                                    /* CPU Type */
int isIntel();                                      /* Prüft ob es ein Intel CPU ist */
int isAMD();                                        /* Prüft ob es ein AMD CPU ist */
int isCyrix();                                      /* Prüft ob es ein Cyrix CPU ist */
char *cpuName();                                    /* Gibt den CPU Namen zurück */
void cpuPrintFeatureFlags();                        /* Gibt die CPU feature flags zurück */
int checkTSC();                                     /* Prüft ob der Time Stamp Counter vorhanden ist */

#endif
