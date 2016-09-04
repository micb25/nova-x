#ifndef _PROTMODE_H
#define _PROTMODE_H

extern void messageDescriptorSetup();  /* Die externen Funktionen (ProtMode.asm) */
extern void setUpInterruptHandler(); 
extern void CopyIntoMemory(unsigned int*, unsigned int,  unsigned int, unsigned int);
extern void megawait();

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
    char AVLBit);
void setGate16(
    int Destination, 
    unsigned long int Address, 
    short Selektor, 
    unsigned char PreasenzBit, 
    unsigned char DPL, 
    unsigned char Typ);
void StartProtectedMode();

#endif
