/*  MEMORY MANAGEMENT */
/*  Version 1.0 */
/*  by Hornet */

typedef struct {        /* MemAlloc-Header - 20 Byte */
    unsigned long adress;
    unsigned long size;
    unsigned long prev;
    unsigned long next;
    short x;
    short y;
} __attribute__ ((packed)) MemAllocHeader;

#include "mm.h"

long HeapStart,HeapSize;

MemAllocHeader neuerHeader, aktuellerHeader,prevHeader, nextHeader;

/* Die Malloc Funktion */
void* malloc(long bytes) {
    mmreadmementry(HeapStart, &aktuellerHeader); /* Lesen des ersten Eintrags */
    
    while (true) {
        if (aktuellerHeader.next == 0) {    /* Letzter MemHeader - danach muss Platz sein */
            neuerHeader.adress = aktuellerHeader.size + aktuellerHeader.adress + sizeof(MemAllocHeader);  /* Die aktuelle Position */
            if (neuerHeader.adress >= 0x10000) {
                return 0;
                break;
            }
            neuerHeader.size = bytes;    /* Größe */
            neuerHeader.prev = aktuellerHeader.adress; /* Der vorherige Header wird der der aktuelle sein */
            neuerHeader.next = aktuellerHeader.next;    /* next ist ja 0 */
            neuerHeader.x = 0;       /* Unbenutzt */
            neuerHeader.y = 0;       /* Unbenutzt */
            mmwritemementry(neuerHeader.adress, neuerHeader);  /* Schreiben des neuen Headers */
            aktuellerHeader.next = neuerHeader.adress;  /* Aktuellen letzten Header updaten */
            mmwritemementry(aktuellerHeader.adress, aktuellerHeader);
            return (void*) (neuerHeader.adress + sizeof(MemAllocHeader));  /* Zurückgeben der Adresse */
            break; /* Raus aus der Schleife */
        }
        if ((aktuellerHeader.next - (aktuellerHeader.adress + sizeof(MemAllocHeader) + aktuellerHeader.size)) >= (bytes + sizeof(MemAllocHeader))) {  /* Überprüfen, ob man eine Lücke nutzen kann */
            neuerHeader.adress = aktuellerHeader.size + aktuellerHeader.adress + sizeof(MemAllocHeader); /* Setzten des neuen Headers */
            neuerHeader.size = bytes;
            neuerHeader.prev = aktuellerHeader.adress;
            neuerHeader.next = aktuellerHeader.next;
            neuerHeader.x = 0;
            neuerHeader.y = 0;
            mmwritemementry(neuerHeader.adress, neuerHeader);  /* Neuer Header wird geschrieben */
            aktuellerHeader.next = neuerHeader.adress;
            mmwritemementry(aktuellerHeader.adress, aktuellerHeader); /* Aktueller Header wird upgedatet */
            mmreadmementry(aktuellerHeader.next, &nextHeader);
            nextHeader.prev = aktuellerHeader.adress;
            mmwritemementry(nextHeader.adress, nextHeader);
            return (void*) (neuerHeader.adress + sizeof(MemAllocHeader));  /* Adresse wird zurückgegeben */
            break;
        }
        /* if (aktuellerHeader.next == 0xfffc) ms(); */
        mmreadmementry(aktuellerHeader.next,&aktuellerHeader);  /* Es wird immer der nächste Header geladen */
    }
}

// Diese Funktion gibt die von malloc insgesamt reservierte Byte-Zahl zurück!

int mallocedsize() {
    long x = 0;
    mmreadmementry(HeapStart, &aktuellerHeader);  /* Lese den ersten Header */
    while (true) {                               /* Endlosschleife */
        x = x + aktuellerHeader.size;
        if (aktuellerHeader.next == 0)
            break;
        mmreadmementry(aktuellerHeader.next, &aktuellerHeader);  /* Nächsten Header laden */
    }
    return x;
}

/* Die Free Funktion */
void free(void *pointer) {
    long x;
    x = (long) pointer;  /* X = Adresse von Pointer */
    mmreadmementry(HeapStart, &aktuellerHeader);  /* Lese den ersten Header */
    while (true) {                               /* Endlosschleife */
        /* Überprüfen ob der übergebene Pointer im aktuellen Header - Bereich liegt */
        if ((aktuellerHeader.adress + sizeof(MemAllocHeader)) <= x && x <= (aktuellerHeader.adress + aktuellerHeader.size + sizeof(MemAllocHeader))) {
            mmreadmementry(aktuellerHeader.prev, &prevHeader);    /* Der vorherige Header wird geladen, und im vorherigen Header wird der Next */
            prevHeader.next = aktuellerHeader.next;              /* Eintrag verstellt */
            mmwritemementry(prevHeader.adress, prevHeader);      
            if (aktuellerHeader.next != 0) {                 /* Wenn der zu löschende Header nicht der letzte ist */
                mmreadmementry(aktuellerHeader.next, &nextHeader);       /* Im nächsten Header wird die Prev-Einstellung verändert */
                nextHeader.prev = aktuellerHeader.prev;
                mmwritemementry(nextHeader.adress, nextHeader);
            }
            char *pointer2;
            pointer2 = (char*) aktuellerHeader.adress;  /* Memory (zu löschender Bereich) wird mit 00 beschrieben - sieht schöner aus + leichter zu debuggen */
            for (x = 0; x != (aktuellerHeader.size + sizeof(MemAllocHeader)); x++) {
                *(pointer2 + x) = 0x00;
            }
            break;  /* OK, das wars - Ende der Schleife*/
        }
        if (aktuellerHeader.next == 0) {
            outputString("\n\rSpeicher konnte nicht freigegeben werden\n\r");
            systemstop();
        }
        mmreadmementry(aktuellerHeader.next, &aktuellerHeader);  /* Nächsten Header laden */
    }
}

pagetableentry tempPTE;

pagetableentry createPTEntry(
    unsigned long Pageframe,
    unsigned short AvailBit,
    unsigned char DirtyBit,
    unsigned char PCDBit,
    unsigned char PWTBit,
    unsigned char USBit,
    unsigned char RWBit,
    unsigned char PresentBit)
{
    if (Pageframe < 0x100000)
        tempPTE = Pageframe;
    else {
        printf ("ACHTUNG - CreatePTE: Pageframe zu groß! \n ");
        systemstop();
    }
    tempPTE = tempPTE << 3; 
    if (AvailBit < 0x8)
        tempPTE = tempPTE + AvailBit;
    else {
        printf ("ACHTUNG - CreatePTE: Avail-Bit zu groß! \n ");
        systemstop();
    }
    tempPTE = tempPTE << 3;
    if (DirtyBit < 0x2)
        tempPTE = tempPTE + DirtyBit;
    else {
        printf ("ACHTUNG - CreatePTE: Dirty-Bit zu groß! \n ");
        systemstop();
    }
    tempPTE = tempPTE << 2;
    if (PCDBit < 0x2)
        tempPTE = tempPTE + PCDBit;
    else {
        printf ("ACHTUNG - CreatePTE: PCD-Bit zu groß! \n ");
        systemstop();
    }
    tempPTE = tempPTE << 1;
    if (PWTBit < 0x2)
        tempPTE = tempPTE + PWTBit;
    else {
        printf ("ACHTUNG - CreatePTE: PWT-Bit zu groß! \n ");
        systemstop();
    }
    tempPTE = tempPTE << 1;
    if (USBit < 0x2)
        tempPTE = tempPTE + USBit;
    else {
        printf("ACHTUNG - CreatePTE: U/S-Bit zu groß! \n ");
        systemstop();
    }
    tempPTE = tempPTE << 1;
    if (PresentBit < 0x2)
        tempPTE = tempPTE + RWBit;
    else {
        printf("ACHTUNG - CreatePTE: R/W-Bit zu groß! \n ");
        systemstop();
    }
    tempPTE = tempPTE << 1;
    if (PresentBit < 0x2)
        tempPTE = tempPTE + PresentBit;
    else {
        printf("ACHTUNG - CreatePTE: Present-Bit zu groß! \n ");
        systemstop();
    }
    return tempPTE;
}

char *foo, *foo2;

// Initalisiert den Memory Manager
char startmemm() {
    HeapStart = (long) &_endofdata;  // Nach der Data Section fängt der HEAP an. Das Symbol wird durch LD gesetzt.
    HeapSize = 0x20000 - 0x500;    // Wir haben 128 KB zur Verfügung. 
    aktuellerHeader.adress = HeapStart;    // Der erste Eintrag wird gesetzt.
    aktuellerHeader.size = 0x10;
    aktuellerHeader.prev = 0x0;
    aktuellerHeader.next = 0x0;
    aktuellerHeader.x = 0x0;
    aktuellerHeader.y = 0xFF;
    mmwritemementry(HeapStart, aktuellerHeader); /* Schreiben des ersten Headers */
    
    // Ab hier wird kurz getestet, ob malloc() funktioniert.
    foo = malloc(512);
    free(foo);
    foo2 = malloc(512);
    free(foo2);
    if (foo != foo2) {
        return 1;
    } else {
        return 0;
    }
}

/* ERSTELLT DIE PAGING TABELLE ! */
/* BETA : Kommentare kommen später */

void initzPaging() {
    pagetableentry temp;
    int iw;
    unsigned long xs;

    temp = createPTEntry(0x11, 0x7, 0, 0, 1, 0, 0, 1);  
    setPTEintoMEM(0x10000, temp);   
    for (iw = 0; iw < 1021; iw++) {
        temp = createPTEntry(iw, 0x7, 0, 0, 1, 0, 0, 1);
        xs = 0x11000 + (4 * iw);
        setPTEintoMEM(xs, temp);
    }
    temp = createPTEntry(0x290, 0x7, 0, 0, 1, 0, 0, 1);
    setPTEintoMEM(0x10A00, temp);   
    for (iw = 0; iw < 1024; iw++) {
        temp = createPTEntry(iw + 0x80, 0x7, 0, 0, 1, 0, 0, 1);
        setPTEintoMEM(0x290000 + (4 * iw), temp);
    }
    temp = createPTEntry(0x2C0, 0x7, 0, 0, 1, 0, 0, 1);
    setPTEintoMEM(0x10B00, temp);   
    for (iw = 0; iw < 1024; iw++) {
        temp = createPTEntry(iw + 0x30, 0x7, 0, 0, 1, 0, 0, 1);
        setPTEintoMEM(0x2C0000 + (4 * iw), temp);
    }
}

// Diese Funktion findet die Größe des RAMS heraus, indem
// jedes MB ein String in den RAM geschrieben wird.
// Dazu wird ein 4 GB Deskriptor benutzt.

int memorysize() {
    long mb, z, check1, check2;
    z = 0;
    mb = 0x10000;
    
    while (z < 4000) { // Wer hat schon über 4000MB Ram ?!?!?!
        check1 = readallmem(mb);          // Lesen und Schreiben von den Kennzahlen
        writeallmem(mb, 0xCCCCCC);
        check2 = readallmem(mb);
        if (check2 != 0xCCCCCC)
            break;
        writeallmem(mb, 0xDDDDCD);
        check2 = readallmem(mb);
        if (check2 != 0xDDDDCD)
            break; 
        writeallmem(mb, check1);
        mb = mb + 0x100000;         // Die Schreib-Adresse erhöhen
        z = z + 1;
    }
    return z;
}
