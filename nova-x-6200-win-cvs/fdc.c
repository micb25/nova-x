/*
 Floppy Funktionen 
				von MB 
				    Version 0.01
    
    Letzte Änderung: 19.06.2003
    
    Neue Überarbeitung by Hornet 11.08.2003
*/

#define FDC_DEBUG

#include "dma.c"
#include "fdc.h"

int motor = false;

#define CMD_SPECIFY     0x03
#define CMD_SENSEI      0x08
#define CMD_READ        0xE6
#define CMD_WRITE       0xC5
#define CMD_SEEK        0x0F
#define CMD_RECAL       0x07
#define CMD_VERSION     0x10
#define FLOPPYTYPE_NEC     1
#define FLOPPYTYPE_INTEL   2
#define FLOPPYTYPE_UNKNOWN 3
#define RTCSEL      0x70 
#define RTCDATA     0x71 
#define NVRAM_FD    0x10 
#define FDCTimeOutTicks 100
#define FDTYPE_NONE 0x00 
#define FDTYPE_360  0x01 
#define FDTYPE_1200 0x02 
#define FDTYPE_720  0x03 
#define FDTYPE_1440 0x04 
#define FDTYPE_2880 0x05 
#define     FDTYPE_MASK 0x0F 

/* IO Ports */

#define FDC_DOR  0x3f2   /* Digital Output Register */
#define FDC_MSR  0x3f4   /* Main Status Register (input) */
#define FDC_DRS  0x3f4   /* Data Rate Select Register (output) */
#define FDC_DATA 0x3f5   /* Data Register */
#define FDC_DIR  0x3f7   /* Digital Input Register (input) */
#define FDC_CCR  0x3f7   /* Configuration Control Register (output) */
#define LOW_BYTE(x)      (x & 0x00FF)
#define HI_BYTE(x)       ((x & 0xFF00) >> 8)
#define     NFD         (2)   // Maximal 2 Laufwerke unterstützt 



/* interne Funktionen */

unsigned char motorstatus, statusz, status[7], fdctrack, fdcsector, fdchead, statreg0;
unsigned long timeout, t1, t2;
unsigned char floppytype;

/* warten bis FIFO bereit ist und das DIO Bit gesetzt ist*/

void fdcwaitwithdio() {
    while ((nxinport(0x3F4) & 192) !=  192)
        nxinport(0x80);
}

/* sendbyte() routine from intel manual */
/* Überarbeitet von Vitali Müller */
/* SendByte sendend ein Byte an den FDC FIFO damit dieser
   z.B. Befehle ausführt */

void sendbyte(int byte) {
    volatile int msr;
    int tmo;

    for (tmo = 0; tmo < 5000; tmo++) {
        msr = nxinport(FDC_MSR);     // Überprüfe, ob senden erlaubt ist
        if ((msr & 0xC0) == 0x80) {
            nxoutport(FDC_DATA,byte);     // Sende Byte
            return;
        }
        nxinport(0x80);   /* delay */
    }
}

/* getbyte() routine from intel manual */
/* Überarbeitet von Vitali Müller */
/* Liest einen Befehl vom FDC DATA FIFO, wenn es was zu lesen gibt */

int getbyte() {
    volatile int msr;
    int tmo, pto;             // Variabeln zur Berechnung des TimeOuts
 
    tmo = gettickscount();   // Speichern der Ausgangsticks

    while (true) {                // Endlosschleife
        msr = nxinport(FDC_MSR);          // Überprüfe ob FDC auf Lese Befehl wartet
        if ((msr & 0xd0) == 0xd0) {
            return nxinport(FDC_DATA);         // Lese und gib zurück
            break;
        }
        nxinport(0x80);       /* Verzögerung */
        pto = gettickscount();            // Aktuelle Ticks
        if (pto > tmo + FDCTimeOutTicks) {
            break;
            return 255;   /* Überprüfe ob TimeOut */
        }
    }
}


/* Die Funktion errechnet aus dem Image-Sektor die aktuelle Position auf dem Datenträger*/

void diskposition(int imgpos, DISKPOSITION *dp) {  
    int a, x1; 
	a = imgpos / 18;
	x1 = imgpos % 18;
    x1++;
    dp->Sector = x1;          /*   Die berechneten Werte werden geschrieben  */
    dp->Head = a % 2;
    dp->Track = a / 2;
}

/* warten bis FIFO bereit ist */
/*     FDC WAIT */

char fdcwait(char makeSIS) { 
    unsigned char temp;
    // Warte, bis Interrupt ankommt, oder Time Out
    timeout = 500;
    t1 = gettickscount();
    while (temp != 0) {
        temp = isirq6();
        t2 = gettickscount();
        if (t2 > t1 + timeout)
            break;
    }
    // TimeOut   
    if (t2 > t1 + timeout) {
        printf("\n ACHTUNG: Disk TimeOut \n");
        return false;
    }
    /* Falls es eine Result Phase gibt, wird es hier eingelesen.
       Das Status < 7 ist dafür da, dass nicht mehr als 8 Byte eingelesen
       werden, und dass "inportb(FDC_MSR) & (1<<4)" überprüft, ob das
       Gerät Busy ist und es somit eine Result Phase gibt */
    
    statusz = 0;    // Status -Zähler auf 0
    status[0] = 0xCC;
    while ((statusz < 7) && (nxinport(FDC_MSR) & (1 << 4)))
        status[statusz++] = getbyte(); // Einlesen
    // Sendet einen Sensie Interrupt Status
    // um das aktuelle Status Register und den FDC Track zu erhalten
    if (makeSIS == 1) {
        // send a "sense interrupt status" command
        sleep(1);
        sendbyte(CMD_SENSEI);
        statreg0 = getbyte();
        fdctrack = getbyte();
    }
    // Jetzt können wir den Interrupt löschen   
    clearirq6();
    // Nochmal die Returns

    return (!(t2 > t1 + timeout));
}

/* Wartet X Millisekunden */

void delay(unsigned long ms) {
    unsigned long ts1, ts2;
   
    ts1 = gettickscount();
    while (true) {
        ts2 = gettickscount();
        if (ts2 >= ts1 + ms)
            break;
    }
}

/* SEEK */
/* Bewegt den Lesekopf über eine bestimmte Spur */

unsigned char fdcseek(unsigned int track) {
    if (fdctrack == track)  // Sind wir schon da? 
        return true;
    /* Senden der Bytes */
    sendbyte(CMD_SEEK);
    sendbyte(0);
    sendbyte(track);
    /* Warte bis abgeschlossen */
    if (fdcwait(true) == 0) {
        return false;     /* timeout! */
    }
    //   delay(20);
    /* check that seek worked */
    if ((statreg0 != 0x20) || (fdctrack != track)) {
        printf("\n Seeken Fehlgeschlagen! StatReg0 : %d \n", statreg0);
        return false;
    } else {
        return true;
    }
}

unsigned char cmos_type; 
unsigned char type; 
DMA_BLOCK dmabuffer;

/* Ein kleiner HexViewer,
   der einen 512 Byte Großen Mem Bereich anzeigt */
   
unsigned int nNewLine;
unsigned char xs;

void hexedit(unsigned long Positions) {
    cls();
    nNewLine = Positions + 22;
    printf("\n");
    while (true) {
        xs = readallmem(Positions);
        printf("%x ", xs);
        xs = readallmem(Positions + 1);
        printf("%x ", xs);
        Positions = Positions + 2;
        if (Positions >= nNewLine) {
            printf("\n");
            nNewLine = nNewLine + 22;
        }
        if ((Positions >= 0x20000 + 512) || Positions < 0x20000)
            break;
    }
    printf("\n");
}

/* Diese Funktion liest einen Sector in den DMA Buffer! */

void ReadSectorBETA(DISKPOSITION *fdpos, unsigned char FirstRead, unsigned char fdmotoroff) {
    if (FirstRead == 1) {   /* Erster Read? Wenn ja, schalte Motor ein u.m. */
        enablefloppymotor();
        //nxoutport(FDC_CCR,0); 
    }
    fdcseek(fdpos->Track); /* Gehe zum Track X*/
    makedmablock(&dmabuffer, 0x200,0x200);
    dma_setup(2,&dmabuffer,0x46);
    clearirq6();
    sendbyte(CMD_READ);   /* Senden der READ BEFEHLE */
    sendbyte(fdpos->Head << 2);
    sendbyte(fdpos->Track);
    sendbyte(fdpos->Head);
    sendbyte(fdpos->Sector);
    sendbyte(2);               /* 512 Bytes pro Sektor */
    sendbyte(18);
    sendbyte(0x1b);            /* GAP 3 - 1.44 MB Disk */
    sendbyte(0xff);            /* DTL = unbenutzt */
    if (fdcwait(false)) {
        dma_stop(2);
        if ((status[0] & 0xc0) == 0) {
        }
    } else {
        printf("Etwas ist falsch:\n");
        printf("\nMSR: %d \n", nxinport(FDC_MSR));
        printf("DOR: %d \n", nxinport(FDC_DOR));
        printf("DIR: %d \n", nxinport(FDC_DIR));
        printf("FDCTACK: %d \n", fdctrack);
        systemstop();
    }
    if (fdmotoroff == 1)
        disablefloppymotor();
}

/* Liest den Diskettenlaufwerk-Typ aus */

char getFloppyType(unsigned char i) { 
    nxoutport(RTCSEL, NVRAM_FD); 
    cmos_type = nxinport(RTCDATA); 
    type = (cmos_type >> (4 * (NFD - 1 - i ))) & FDTYPE_MASK; 
    return type;
}

/* Liest die Anzahl der Diskettenlaufwerke aus */

int getnumberoffloppydrives(void) { 
    unsigned char cmosbyte; 
    int nrs_msb = 0; 
    int nrs_lsb = 0; 

    nxoutport(0x70, 0x14); 
    cmosbyte = nxinport(0x71); 

    nrs_msb = (cmosbyte << 6 ) & 1; 
    nrs_lsb = (cmosbyte << 7 ) & 1; 

    return ((nrs_msb + (2 * nrs_lsb)) + 1); 
} 

/* Initialisiert den FDC beim Bootvorgang */

char initalizingFDC() {
    int i, k, fdctype;
    printf("\nFloppy Disc Controller -  %d Floppy Drives found", getnumberoffloppydrives());
    for (i = 0; i <= getnumberoffloppydrives() - 1; i++) {
        printf("\nFD[%d]: ", i);
        k = getFloppyType(i);
        if (k == 0) {
            outputString("None");
        } else if (k == 1) {
            outputString("360KB");
        } else if (k == 2) {
            outputString("1.2MB");
        } else if (k == 3) {
            outputString("720KB");
        } else if (k == 4) {
            outputString("1.44MB");
        } else if (k == 4) {
            outputString("2.88MB");
        }
    }                
    printf("\nInitializing floppy controller     : ");
    nxoutport(FDC_DATA, CMD_VERSION);
    fdcwaitwithdio();
    fdctype = nxinport(FDC_DATA); /* Versionsnummer lesen*/
    if (fdctype == 0x80) { /* alter FDC */
        textcolor(COLOR_GREEN);
        outputString("OK\n");
        textcolor(COLOR_LIGHTGRAY);
        outputString("NEC765 compatible floppy controller found!\n");
        floppytype = FLOPPYTYPE_NEC;
    } else if (fdctype == 0x81) { /* alter FDC */
        textcolor(COLOR_GREEN);
        outputString("OK\n");
        textcolor(COLOR_LIGHTGRAY);
        outputString("Advanced NEC765 compatible floppy controller found!\n");
        floppytype = FLOPPYTYPE_NEC;
    } else if (fdctype == 0x90) { /* neuer FDC */
        textcolor(COLOR_GREEN);
        outputString("OK\n");
        textcolor(COLOR_LIGHTGRAY);
        outputString("Intel 82077 compatible floppy controller found!\n");
        floppytype = FLOPPYTYPE_INTEL;
    } else { /*FDC unbekannt*/
        textcolor(COLOR_RED);
        outputString("FAIL\n");
        textcolor(COLOR_LIGHTGRAY);
        printf("Unknown floppy controller found!(version #: %x ) \n",fdctype);
        floppytype = FLOPPYTYPE_UNKNOWN;
    }
    outputString("Reset floppy controller...\n");
    fdcreset(); 
}

void readtest(char Turbo) {
    DISKPOSITION testpos;
    unsigned long gbt = gettickscount(), gnt, xy = 0;

   	ReadSectorBETA(&testpos, 1, 0);
  	while (true) {
        diskposition(xy, &testpos);
   	   	ReadSectorBETA(&testpos, 0, 0);
   	   	hexedit(0x20000);
   	   	gotoxy(70, 5);
   	   	printf("Sektor: %d", xy);
   	   	if (Turbo == 0)
            getchar();
   	   	if (xy == 499)
            break;
   	   	xy++;
    }
   	gnt = gettickscount();
  	cls();
   	printf("\n Ticks: %d -> Sek.: %d ", gnt - gbt, (gnt - gbt) / 100);
}

/* Resettet den FDC */

void fdcreset(void) {
    /* Stoppen des Motors und DMA/IRQ */
    nxoutport(FDC_DOR, 0);  
    motorstatus = 0;
    /* Data Rate (500K/s) */
    nxoutport(FDC_DRS, 0);
    enablefloppymotor();
    clearirq6();
    /* Aktiviere Interrupts */
    nxoutport(FDC_DOR,0x0c);
    /* Den aktivierten Interrupt abwarten */
    fdcwait(true);
    /* Laufwerk-Zeiten (aus dem Bios) */
    sendbyte(CMD_SPECIFY);
    sendbyte(0xDF);  /* SRT = 3ms, HUT = 240ms */
    sendbyte(0x02);  /* HLT = 16ms, ND = 0 */
}

void fdcrecal(void) {
    enablefloppymotor();
    /* Bytes senden */
    sendbyte(CMD_RECAL);
    sendbyte(0); // Seeken
    /* Warten, bis Seek zu ende ist */
    fdcwait(true);
    /* Überprüfe ob alles OK: */
    if (statreg0 == 192) {
        printf ("\nFehler in der FDC-Recalibration!\n");
    } 
}
