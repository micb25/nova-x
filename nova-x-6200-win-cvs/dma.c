/*
    DMA Funktionen von MB
    
    
    DMA Channels 0-3 =  8 Bit
    DMA Channels 4-7 = 16 Bit
    
    Achtung: DMA kann nur auf den 1. MB RAM zugreifen!

*/

#include "dma.h"

/* die verschiedenen Ports der einzelnen DMA Channels*/
/*   DMA Channel                 0     1     2     3     4     5     6     7 */
unsigned char dma_mask  [8] = { 0x0A, 0x0A, 0x0A, 0x0A, 0xD4, 0xD4, 0xD4, 0xD4 };
unsigned char dma_mode  [8] = { 0x0B, 0x0B, 0x0B, 0x0B, 0xD6, 0xD6, 0xD6, 0xD6 };
unsigned char dma_clear [8] = { 0x0C, 0x0C, 0x0C, 0x0C, 0xD8, 0xD8, 0xD8, 0xD8 };
unsigned char dma_page  [8] = { 0x87, 0x83, 0x81, 0x82, 0x8F, 0x8B, 0x89, 0x8A };
unsigned char dma_addr  [8] = { 0x00, 0x02, 0x04, 0x06, 0xC0, 0xC4, 0xC8, 0xCC };
unsigned char dma_count [8] = { 0x01, 0x03, 0x05, 0x07, 0xC2, 0xC6, 0xCA, 0xCE };

void dma_setup(unsigned char channel, DMA_BLOCK *buffer, unsigned char mode) {
    mode = mode | channel; /* überprüfen, ob der DMA Channel schon in der Mode gesetzt wurde*/
    asm("cli"); /* Int's ausschalten */
    buffer->page = 2;
    buffer->offset = 0;
    buffer->length = 512;
    channel = 2;
    mode = 70;
    
    nxoutport(dma_mask [channel], 0x04 | channel); /* DMA Channel maskieren*/
    nxoutport(dma_clear[channel], 0x00); /* aktuellen DMA Channel stoppen*/
    nxoutport(dma_mode [channel], mode); /* welche Operation soll ausgeführt werden? Lesen, Schreiben, Überprüfen? */
    nxoutport(dma_addr [channel], LO(buffer->offset)); /* wohin soll geschrieben werden - offset Adresse(low) */
    nxoutport(dma_addr [channel], HI(buffer->offset)); /* wohin soll geschrieben werden - offset Adresse(high) */
    nxoutport(dma_page [channel], buffer->page);       /* welche seite soll geschrieben werden? (0-15)*/
    nxoutport(dma_count[channel], LO(buffer->length)); /* wieviel Bytes sollen gelesen bzw geschrieben werden? */
    nxoutport(dma_count[channel], HI(buffer->length)); /* wieviel Bytes sollen gelesen bzw geschrieben werden? */
    nxoutport(dma_mask [channel], channel); /* DMA Channel maskieren*/
    asm("sti"); /* Int's einschalten */
}

void dma_pause(unsigned char channel) {
    nxoutport(dma_mask[channel], 0x04 | channel); /* einfach nur das Mask-Bit und den Channel setzen */
}

void dma_unpause(unsigned char channel) {
    nxoutport(dma_mask[channel], channel); /* Mask-Bit wieder aufheben */
}

void dma_stop(unsigned char channel) { // beendet einen DMA Transfer
    dma_pause(channel); /* DMA Channel maskieren */
    nxoutport(dma_clear[channel], 0x00); /* aktuellen Transfer abrechen */
    dma_unpause(channel); /* Maskierung aufheben */
}

/* 
schreibt aus einer Adresse und einer Länge in einen DMA_BLOCK
Bsp:
|||--- unbenutzt
000B0100h = addr
   |||||------Offset Adresse    
   |
   ------ Page(Seite)
*/

void makedmablock(DMA_BLOCK *block, unsigned int addr, unsigned short len) {
    block->page   = (addr & 0x000F0000) >> 16;  /* Seite übernehmen*/
    block->offset = (addr & 0x0000FFFF);        /* Offset Adresse übernehmen */
    block->length = len;                        /* Länge übernehmen */
}
