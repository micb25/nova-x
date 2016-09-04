#ifndef _MM_H_
#define _MM_H_

extern char mmwriteallmem(long, long); 
extern long mmreadallmem(long);
extern char _endofdata; // Symbol, welches von LD gesetzt wird und das Ende des .data Blocks markiert. Dort beginnt der Heap.
extern char mmwritemementry(long, MemAllocHeader); /* Schreibt einen MemAllocHeader ins DS-Segment */
extern char mmreadmementry(long, MemAllocHeader*); /*  Liest ein MemAllocHeader aus dem DS-Segment   */

void* malloc(long bytes);
int mallocedsize();
void free(void *pointer);
pagetableentry createPTEntry(
    unsigned long Pageframe,
    unsigned short AvailBit,
    unsigned char DirtyBit,
    unsigned char PCDBit,
    unsigned char PWTBit,
    unsigned char USBit,
    unsigned char RWBit,
    unsigned char PresentBit);
char startmemm();
void initzPaging();
int memorysize();

#endif
