/*  PROTECTED MODE INITALISIERUNG 
*   Diese Funktionen schalten die CPU und den Protected Mode 
*   Um die Funktionsweiße zu verstehen,  sollte man ein ausführliches Tutorial über
*   protected mode lesen! 
*
*/

#include "protmode.h"

typedef struct {        /* GDT-Zeiger Struktur */
    short Limit;
    unsigned long BaseAdr;
} __attribute__ ((packed)) NXtGDT;

typedef struct {        /* Desktriptor Struktur*/
    unsigned short  SegmentSize;  /* 2 Byte*/
    unsigned char SegmentAdr[3]; /* 3 Byte*/
    unsigned char Type[2]; /* 2 Byte*/
    unsigned char SegmentAdr2; /* 1 Byte == Gesamt 8 BYTE!*/
} NXtDeskriptor;

typedef struct {    /* Inerrupt Gate Struktur */
    unsigned char Adr[2]; /* 2 */
    unsigned short int Selektor; /* 2 */
    unsigned char Pudding; /* 1 */
    unsigned char Typ; /* 1 */
    unsigned char Adr2[2]; /* 2 */
} NXtGate;

#define NXnumberIDT  0x80  /* Anzahl der IDTs sowie GDTs  */
#define  NXnumberGDT  11
#define  NoOfInts 4

short tempnIDTs;
NXtDeskriptor arrayGDT[12]; /* GDTs ist die GD-Tabelle */
NXtGate arrayIDT[0x80];       /* IDTs ist die Interrupt-Deskriptor-Tabelle */
NXtGDT GDT,  IDT;        /* Die Zeiger auf die GDT bzw. IDT */
NXtGate tempGateDeskriptor; 

asm (".code16gcc"); /* Wir brauchen 16 BIT */

/* "Erstellt" einen Deskriptor -- Wer mehr über Deskriptoren wissen will,  sollte sich ein Tutorial durchlesen */
void setDescriptor16(
    int Destination, 
    unsigned long SegmentSize, 
    unsigned long SegmentBasisAdr, 
    char PresentBit, 
    char Privileg, 
    char SegmentBit, 
    char TypBit, 
    char CEBit, 
    char RWBit, 
    char AccessBit, 
    char GranularityBit, 
    char DBBit, 
    char CoderBit,
    char AVLBit)
{ 
    NXtDeskriptor temparrayGDT;
    unsigned long tts;
    unsigned short tempIntCD,  tempnIDTs; /* CD = Create Deskriptor - Prozeduren */
    unsigned char tempCharBuffer[4],  *tempCharPointer;
    NXtDeskriptor tempDeskriptor;
 
    tempIntCD = (SegmentSize % 0x10000);
    arrayGDT[Destination].SegmentSize = tempIntCD; /* Segment Größe : Bit  0..15 ! = Integer */
    arrayGDT[Destination].SegmentAdr[0] = SegmentBasisAdr % 0x100; /* 54*/
    arrayGDT[Destination].SegmentAdr[1] = (SegmentBasisAdr >> 8) % 0x100; /* 76 */
    arrayGDT[Destination].SegmentAdr[2] = (SegmentBasisAdr >> 16) % 0x100; /*8*/
    arrayGDT[Destination].SegmentAdr2 =  (SegmentBasisAdr >> 24) % 0x100;  /* 0 */

    arrayGDT[Destination].Type[0] = PresentBit;

    arrayGDT[Destination].Type[0] = arrayGDT[Destination].Type[0] << 2;
    arrayGDT[Destination].Type[0] = arrayGDT[Destination].Type[0] + Privileg;

    arrayGDT[Destination].Type[0] = arrayGDT[Destination].Type[0] << 1;
    arrayGDT[Destination].Type[0] = arrayGDT[Destination].Type[0] + SegmentBit;

    arrayGDT[Destination].Type[0] = arrayGDT[Destination].Type[0] << 1;
    arrayGDT[Destination].Type[0] = arrayGDT[Destination].Type[0] + TypBit;

    arrayGDT[Destination].Type[0] = arrayGDT[Destination].Type[0] << 1;
    arrayGDT[Destination].Type[0] = arrayGDT[Destination].Type[0] + CEBit;

    arrayGDT[Destination].Type[0] = arrayGDT[Destination].Type[0] << 1;
    arrayGDT[Destination].Type[0] = arrayGDT[Destination].Type[0] + RWBit;

    arrayGDT[Destination].Type[0] = arrayGDT[Destination].Type[0] << 1;
 
    arrayGDT[Destination].Type[0] = arrayGDT[Destination].Type[0] + AccessBit; /* Schreiben des 1. Typen-Blocks */
 
    arrayGDT[Destination].Type[1] = GranularityBit;
    arrayGDT[Destination].Type[1] = arrayGDT[Destination].Type[1] << 1;

    arrayGDT[Destination].Type[1] = arrayGDT[Destination].Type[1] + DBBit;
    arrayGDT[Destination].Type[1] = arrayGDT[Destination].Type[1] << 1;

    arrayGDT[Destination].Type[1] = arrayGDT[Destination].Type[1] + CoderBit;
    arrayGDT[Destination].Type[1] = arrayGDT[Destination].Type[1] << 1;

    arrayGDT[Destination].Type[1] = arrayGDT[Destination].Type[1] + AVLBit;
    arrayGDT[Destination].Type[1] = arrayGDT[Destination].Type[1] << 4;

    arrayGDT[Destination].Type[1] = arrayGDT[Destination].Type[1];
    /*tts = SegmentSize >> 16;
    arrayGDT[Destination].Type[1] =arrayGDT[Destination].Type[1] + tts;*/
}

/*  Erstellt ein Interrupt Gate*/
void setGate16(
    int Destination, 
    unsigned long int Address, 
    short Selektor, 
    unsigned char PreasenzBit, 
    unsigned char DPL, 
    unsigned char Typ)
{
    unsigned short tempIntCD,  tempnIDTs; /* CD = Create Deskriptor - Prozeduren */
    unsigned char tempCharBuffer[4],  *tempCharPointer;
    arrayIDT[Destination].Adr[0] = Address % 0x100; /* 54*/
    arrayIDT[Destination].Adr[1] = (Address >> 8) % 0x100; /* 76 */
    arrayIDT[Destination].Adr2[0] = (Address >> 16) % 0x100; /*8*/
    arrayIDT[Destination].Adr2[1] =  (Address >> 24) % 0x100;  /* 0 */
    arrayIDT[Destination].Selektor = Selektor;
    arrayIDT[Destination].Typ = 0; 
    arrayIDT[Destination].Typ = arrayIDT[Destination].Typ + PreasenzBit; /* Verarbeiten der einzelnen Bits */
    arrayIDT[Destination].Typ = arrayIDT[Destination].Typ << 2;
    arrayIDT[Destination].Typ = arrayIDT[Destination].Typ + DPL;
    arrayIDT[Destination].Typ = arrayIDT[Destination].Typ << 5;
    arrayIDT[Destination].Typ = arrayIDT[Destination].Typ + Typ;
    arrayIDT[Destination].Pudding = 0; /* Pudding hat keine Funktion - hält Speicher frei */
}

asm (".code16gcc");
void StartProtectedMode() {          /* Diese Funktion schaltet in den Protected Mode */
    unsigned long GDTPpos,  IDTPpos,  IDTpos,  GDTpos ,  EndOfPMSS,  EndOfIHS;
    /*  
      Zuerst werden die Diskriptoren erstellt und dann in den PMSS geschrieben.
      Danach wird der GDT-Zeiger,  der später mit LGDT geladen wird,  auch ins PMSS (Pos. 00) geschrieben.
      Es werden mit setGate die InterruptGates für die ersten belegten 32 Interrupts geladen, 
      dann kommen noch 3 weiter Interrupts hinzu: "print",  Timer und Keyboard Interrupt dazu.
      Die Handler für die Inerrupts befinden sich in der ProtMode.asm. Mit setIDT werden die
      Interrupt Handler in den dazu vorgesehnen Discriptor kopiert. SPäter wird auch der IDT Zeiger ins
      PMSS geschrieben und mti LIDT geladen (Pos. 0x50).
      Es wird danach der PIC umprogrammiert,  damit die IRQ0-15 Hardwareinterrupts auf 0x20-0x2F zeigen, 
      sodass nicht die Exceptions ausgelöst werden.
      Der PIC Zähler wird auf 100Hz gestellt - eine gute Einstelllung für späteres Multitasking!
      Zum Schluss wird der Protected Mode aktiviert.
    */
    messageDescriptorSetup();
    /* Status ausgeben */

    GDTPpos = 0x00;
    IDTpos = sizeof(GDT);
    GDTpos = IDTpos + sizeof(GDT);
    GDTpos = 0x10;
    IDTPpos = GDTpos + NXnumberGDT * 8;
    EndOfPMSS = (IDTpos + 8 * NXnumberIDT) / 0x1000;
    EndOfPMSS = (EndOfPMSS + 1) * 0x1000;
    EndOfIHS = (0x2000 + (NoOfInts * 0x100) ) / 0x1000;
    EndOfIHS = (EndOfIHS + 1) * 0x1000;
    setDescriptor16(0, 0xDDDD, 0x00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); /* Dummy */
    //sys16();
    setDescriptor16(1, 0xFFFF, 0x80000, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0); /* Codesegment Selektor: 8*/
    setDescriptor16(2, 0xEEEE, 0xB8000, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0); /* Video-RAM Selektor : 16 */
    setDescriptor16(3, 0xFFFF, 0x60000, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0); /* Interrupt Handler Segment: Selektor 24*/
    setDescriptor16(4, 0xFFFF, 0x70000, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0); /* Stack-Segment : Selektor 32 */
    setDescriptor16(5, 0xFFFF, 0x50000, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0); /* Protected Mode System Segment kurz PMSS- Selektor: 40*/
    setDescriptor16(6, 0xFFFF, 0x00000, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0); /* BIG Segment 48*/
    setDescriptor16(7, 0xFFFF, 0x40000, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0); /* Nova-X System Segment Sele: 56*/
    setDescriptor16(8, 0x20,   0x30000, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0); /* Nova-X DATA Segment Sele: 64*/
    setDescriptor16(9, 0xFFFF, 0xA0000000, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0); /* Codesegment Selektor: 72*/
    setDescriptor16(10, 0x20,  0xB0000000, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0); /* Nova-X DATA Segment Sele: 64*/
    CopyIntoMemory((unsigned int*)arrayGDT, GDTpos, NXnumberGDT * 8, 4);  /* Kopieren der GDT ins PMSS nach 0x200 */
    
    //* Setzten des GDT Zeigers */
    GDT.BaseAdr = 0x50000 + GDTpos; 
    GDT.Limit = NXnumberGDT * 8; /* 0x00 00 00 50 */
    CopyIntoMemory((unsigned int*)&GDT, GDTPpos, sizeof(GDT), 4);
    //CopyIntoMemory((unsigned int*)&GDT.Limit, GDTPpos, 13, 4); /* Kopieren des GDT-Zeigers nach PMSS 0x00 */
    //CopyIntoMemory((unsigned int*)&GDT.BaseAdr, GDTPpos+2, 5, 4); /* Kopieren des GDT-Zeigers nach PMSS 0x00 */
    /* Erstellen der Interrupt-Gates */
    /* Erstellen der Interrupt-Gates */
    for (tempnIDTs = 0; tempnIDTs < 30; tempnIDTs++)
        setGate16(tempnIDTs, 256 * tempnIDTs, 24, 1, 1, 14); /* Interrupts von 0-30 sind vorgegeben. Befinden sich im Selektor 24 (Interrupt Handler Segment)*/
    setGate16(0x20, 0x2000, 24, 1, 1, 14); /* Timer Interrupt  - No 0*/
    setGate16(0x21, 0x2100, 24, 1, 1, 14); /* Keyboard Interrupt - No 1*/
    setGate16(0x26, 0x2200, 24, 1, 1, 14);  /* Floppy Interrupt - IRQ6 - No 2 */
    setGate16(0x50, 0x2300, 24, 1, 1, 14); /* Printf Interrupt 0x50 (RealMode: 0x10) wird erstellt - No 3*/
    CopyIntoMemory((unsigned int*)&arrayIDT, IDTPpos , 8 * NXnumberIDT, 4); /* Kopieren der IDT ins PMSS (EFFF) */

    IDT.BaseAdr = 0x50000 + IDTPpos;
    IDT.Limit = 8 * NXnumberIDT;
    CopyIntoMemory((unsigned int*)&IDT, IDTpos , sizeof(IDT), 4);
    //CopyIntoMemory((unsigned int*)&IDT.Limit, IDTpos , 15, 4); /* Kopieren des Zeigers ins PMSS 0x50 */
    //CopyIntoMemory((unsigned int*)&IDT.BaseAdr, IDTpos +2, 15, 4); /* Kopieren des Zeigers ins PMSS 0x50 */

    /* In der PM.asm befinden sich die Interrupt Handler für die Interrupts 0-30 + 32! 
    Diese werden nun nach 0x60000 kopiert (Interrupt Handler Segment).*/
    setUpInterruptHandler(); 
    messageOK();         /* Zeigt OK für "Setting Up Descriptors"  */

    enterProtectedMode(IDTpos); /* Protected Mode wird gestartet --- 00 10 50 00*/
}
