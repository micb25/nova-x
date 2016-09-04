#ifndef _DMA_H
#define _DMA_H

// Typdefinitionen

// ein DMA Block
typedef struct {
    unsigned short length;
    unsigned short offset;
    unsigned char page;
} DMA_BLOCK;

// ein DMA Block
typedef struct {
    unsigned char Track, Head, Sector;
} DISKPOSITION;

#define DMA_CHANNEL0  0
#define DMA_CHANNEL1  1
#define DMA_CHANNEL2  2    /* für Floppy reserviert */
#define DMA_CHANNEL3  3
#define LO(x)         (x & 0x00FF)
#define HI(x)         ((x & 0xFF00) >> 8)

// Funktionen

/* installiert einen DMA Channel */
void dma_setup(unsigned char channel, DMA_BLOCK *buffer, unsigned char mode);
/* unterbricht einen DMA Channel*/
void dma_pause(unsigned char channel);     
void dma_unpause(unsigned char channel);
/* schaltet den DMA Channel ganz aus*/
void dma_stop(unsigned char channel);      
void makedmablock(DMA_BLOCK *block, unsigned int addr, unsigned short len);

#endif
