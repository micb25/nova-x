/*
	CMOS Funktionen
				von MB

    Änderung: 22.06.03 23:22
    Änderung: 29.07.03 von thesame:
    ---> Neue Funktionen checkFPU() und checkcmos()
    Änderung: 15.08.03 von thesame:
    ---> Überarbeitung von checkFPU() und checkcmos() und Kommentierung
    ---> verschiedener Funktionen
    Änderung: 16.08.03 von thesame:
    ---> Perfektionierung von checkcmos() :)
*/

#include "cmos.h"

unsigned char tempint;

int getExtendedmemsize() {
	return (readfromcmos(0x18)*256)+readfromcmos(0x17);
}

int getBasememsize() {
	return (readfromcmos(0x16)*256)+readfromcmos(0x15);
}

char readfromcmos(char port) {
    nxoutport(0x70, port);
    return nxinport(0x71);
}

void writetocmos(char port, char byte) {
    nxoutport(0x70, port);
    nxoutport(0x71, byte);
}

void checkcmos() {
    // Dezimalzahlen der einzelnen Bits
    const int BIT_7 = 64;
    const int BIT_6 = 32;
    const int BIT_5 = 16;
    const int BIT_4 = 8;
    const int BIT_3 = 4;
    const int BIT_2 = 2;
    int value;
    NXbool error_occured = false;
    value = readfromcmos(0x0E);
    
    printf("Checking the CMOS chip status...   ");

    if ((value & BIT_7) == BIT_7) {
        error_occured = true;
        textcolor(COLOR_RED);
        printf("\n  Fatal Error: CMOS Clock Power Loss");
    }
    if ((value & BIT_6) == BIT_6) {
        error_occured = true;    
        textcolor(COLOR_RED);
        printf("\n  Fatal Error: CMOS bad checksum");
    }
    if ((value & BIT_5) == BIT_5) {
        error_occured = true;    
        textcolor(COLOR_RED);
        printf("\n  Fatal Error: Invalid configuration @ POST");
    }    
    if ((value & BIT_4) == BIT_4) {
        error_occured = true;    
        textcolor(COLOR_RED);
        printf("\n  Fatal Error: Memory size compare error");
    }    
    if ((value & BIT_3) == BIT_3) {
        error_occured = true;    
        textcolor(COLOR_RED);
        printf("\n  Fatal Error: Disk or controller error");
    }    
    if ((value & BIT_2) == BIT_2) {
        error_occured = true;    
        textcolor(COLOR_RED);
        printf("\n  Fatal Error: Invalid time or data (32nd)");
    }    
    textcolor(COLOR_LIGHTGRAY);
    if (error_occured) {
        getchar();
    } else {
        printf(": OK");
    }
    printf("\n");
}

void checkFPU() {
    printf("Math Co-Processor (FPU) available  : ");
    if ((readfromcmos(0x14) & 2) == 2) {
        printf("Yes\n");
    } else {
        printf("No\n");
        printf("Error! Nova-X requires a FPU! FPU emulation is not supported yet!\n");
        systemstop();
    }
}
