#ifndef _CPU_H_
#define _CPU_H_

#include "typedefs.h"

void cpuidfill(ulong COMMAND);                                  /* F�llt die CPU Register je nach Command */
void cpuPrintRegs();                                /* Zeigt den aktuellen Inhalt der Register */
void cpuPrintInfos();                               /* Infos */
void cpubootInfos();                                /* CPU boot Infos*/
int cpuFamily();					/* Gibt die CPU Family zur�ck */
int cpuModel();                                     /* Gibt das CPU Model zur�ck */
int cpuStepping();                                  /* Gibt die CPU Stepping zur�ck */
char* cpuVendor();                                  /* Hersteller Vendor String */
char *cpuType();                                    /* CPU Type */
int isIntel();                                      /* Pr�ft ob es ein Intel CPU ist */
int isAMD();                                        /* Pr�ft ob es ein AMD CPU ist */
int isCyrix();                                      /* Pr�ft ob es ein Cyrix CPU ist */
char *cpuName();                                    /* Gibt den CPU Namen zur�ck */
void cpuPrintFeatureFlags();                        /* Gibt die CPU feature flags zur�ck */
int checkTSC();                                     /* Pr�ft ob der Time Stamp Counter vorhanden ist */

#endif
