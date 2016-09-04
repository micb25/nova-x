/*

    nxFS
    Nova-X Filesyst#em 

    version 0.01a by MB
    
        
    für jeden Cluster(4096Bytes = 8 Sektoren) sind am Anfang 9 Bytes
    
    1 Byte:     Typ 0x01 File Entry
                    0x02 Data Entry
    2-5 Byte:   letzter Entry(Sektor)
    6-9 Byte:   nächster Entry(Sektor)
 
    Dateiattribute:
    1. Bit: Datei ist lesbar
    2. Bit: Datei ist schreibbar
    3. Bit: Datei kann ausgeführt werden
    4. Bit: versteckte Datei
    5. Bit: Verzeichnis          
*/

#include "nxFS.h"

#define nxFS_DEBUG

void nxFS_init()
{
    DISKPOSITION *fdpos;
    diskposition(0, fdpos);
    ReadSectorBETA(fdpos, 1, 0);
    #ifdef nxFS_DEBUG
     printf("disc label: %s\n", nxFS_getnstringfrompos(0x20008, 8));
    #endif
    printf("initializing was successfully!\n");
}

void dir()
{
    printf("\nFiles in FD[0]:\n\n");
    printf("Filename                            Size in Bytes      Attr\n");
    DISKPOSITION *fdpos;
    diskposition(1, fdpos);
    ReadSectorBETA(fdpos, 1, 0);
    /* Kernel */
    outputString(nxFS_getstringfrompos(0x20009));
    gotoxy(50-getuintlen(nxFS_getint16rev(0x20109)), wherey());
    outputInteger(nxFS_getint16rev(0x20109));
    gotoxy(56, wherey());
    nxFS_printattr(myreadallmem(0x20108));        
    outputCharacter('\n');
    
    /* andere Files */
    
    unsigned int i=10; unsigned int filescount = 1; unsigned int bytescount = nxFS_getint16rev(0x20109);
    
    while(true)
    {
        diskposition(i, fdpos);
        ReadSectorBETA(fdpos, 0, 0);
        if (myreadallmem(0x20000) == 0x02) i += 8; /* nächsten Cluster lesen */
        if (myreadallmem(0x20000) == 0x01) /* File Entry gefunden! */
                {
                outputString(nxFS_getstringfrompos(0x20009));
                gotoxy(50-getuintlen(nxFS_getint16rev(0x20109)), wherey());
                outputInteger(nxFS_getint16rev(0x20109));
                gotoxy(56, wherey());
                nxFS_printattr(myreadallmem(0x20108));
                outputCharacter('\n');
                i++; filescount++; bytescount += nxFS_getint16rev(0x20109);
                }
        if (myreadallmem(0x20000) == 0x00) break;
        if (myreadallmem(0x20000) != 0x01 && myreadallmem(0x20000) != 0x02 && myreadallmem(0x20000) != 0x00)
                i++;
    }
    printf("\n");
    outputInteger(filescount);
    printf(" files in "); outputInteger(bytescount); printf(" bytes found!\n\n");
    return;
}

char* nxFS_getstringfrompos(int pos)
{
    char *tmpstr;
    int i = 0;
    while(true)
    {
    if (readallmem(pos + i) != 0x00)
    {
        *(tmpstr + i) = readallmem(pos + i);
        i++;
    } else 
        break;
    }
    *(tmpstr + i) = 0x00;
    return tmpstr;
}

char* nxFS_getnstringfrompos(int pos, int count)
{
    char *tmpstr;
    int i = 0;    
    
    if (myreadallmem(pos) == 0x00) return 0x0000;
    while(true)
        {
                if (readallmem(pos + i) != 0x00 && count > 0)
                {
                    *(tmpstr + i) = readallmem(pos + i);
                    i++; count--;
                } else 
                    break;
        }    
    *(tmpstr+i) = 0x00;
    return tmpstr;
}

char *nxFS_getstringlinefrompos(int pos)
{
    char *tmpstr;
    int i = 0;    
    
    if (myreadallmem(pos) == 0x00 || myreadallmem(pos) == 0x0A || myreadallmem(pos) == 0x0D) return 0x0000;
    while(true)
        {
                if (myreadallmem(pos + i) == 0x0A || myreadallmem(pos + i) == 0x0D) break;
                if (myreadallmem(pos + i) != 0x00)
                {
                    *(tmpstr + i) = myreadallmem(pos + i);
                    i++; 
                } else 
                    break;
        }    
    *(tmpstr+i) = 0x00;
    return tmpstr;
}

int nxFS_getint16(int pos)
{
    int i = 0;
    i = myreadallmem(pos++) << 8;
    i += myreadallmem(pos);
    i &= 0xFFFF;
    return i;
}

int nxFS_getint16rev(int pos)
{
    int i = 0;
    i = myreadallmem(pos+1) << 8;
    i += myreadallmem(pos);
    i &= 0xFFFF;
    return i;
}

unsigned int nxFS_getuint16(int pos)
{
    unsigned int i = 0;
    i = myreadallmem(pos) << 8;
    i += myreadallmem(pos+1);
    i &= 0xFFFF;
    return i;
}

unsigned int nxFS_getuint16rev(int pos)
{
    unsigned int i = 0;
    i = myreadallmem(pos+1) << 8;
    i += myreadallmem(pos);
    i &= 0xFFFF;
    return i;
}

unsigned char nxFS_convert(int abc)
{
    abc &= 0xFF;
    return (unsigned char) abc;
}

unsigned char myreadallmem(unsigned int pos)
{
    return (nxFS_convert(readallmem(pos)));
}

void nxFS_printattr(unsigned char attr)
{
    if ((attr & 0x01) != 0) outputCharacter('r'); else outputCharacter('-');
    if ((attr & 0x02) != 0) outputCharacter('w'); else outputCharacter('-');
    if ((attr & 0x04) != 0) outputCharacter('e'); else outputCharacter('-');
    if ((attr & 0x08) != 0) outputCharacter('h'); else outputCharacter('-');
    if ((attr & 0x10) != 0) outputCharacter('d'); else outputCharacter('-');
}

void fopen(TFILE* tmpfile, char *filename) /* read-only */
{
    DISKPOSITION *fdpos2;
    diskposition(1, fdpos2);
    ReadSectorBETA(fdpos2, 1, 0);
    if (strcmp(nxFS_getstringfrompos(0x20009), filename) == 0)
        {
                tmpfile->filename = nxFS_getstringfrompos(0x20009);
                tmpfile->startsektor = 1;
                tmpfile->filesize = nxFS_getint16rev(0x20109);
                tmpfile->filepos = 0;
                tmpfile->absfilepos = 9;
                tmpfile->id = 1;
                return;
        }
    
    int i = 10;    
    
    while(true)
    {
        diskposition(i, fdpos2);
        ReadSectorBETA(fdpos2, 0, 0);
        if (myreadallmem(0x20000) == 0x02) i += 8; /* nächsten Cluster lesen */
        if (myreadallmem(0x20000) == 0x01) /* File Entry gefunden! */
                {
                 if (strcmp(nxFS_getstringfrompos(0x20009), filename) == 0) 
                  {
                  tmpfile->filename = nxFS_getstringfrompos(0x20009);
                  tmpfile->startsektor = i;
                  tmpfile->filesize = nxFS_getint16rev(0x20109);
                  tmpfile->filepos = 0;
                  tmpfile->absfilepos = 9;
                  tmpfile->id = i;
                  return;
                  }
                 }
        if (myreadallmem(0x20000) == 0x00) break;
    }
    disablefloppymotor();
    
    tmpfile->filename = "";
    tmpfile->startsektor = 0;
    tmpfile->filesize = 0;
    tmpfile->filepos = 0;
    tmpfile->absfilepos = 0;
    return;
}

char* freadln(TFILE *tmpfile)
{
    if (tmpfile->startsektor == 0 || tmpfile->filesize == 0) return 0x00;
    unsigned int pos = tmpfile->filepos;
    int sektor = tmpfile->startsektor;
    char *tmpstr;
    DISKPOSITION *fdpos;               
    sektor++; /* zum ersten Data Entry springen */
    if (nxFS_getint16rev(0x20109) < pos) return 0; /* Dateiende wurde erreicht! */
    int abspos = pos + (((pos / 4096) + 1) * 9) + (sektor*512); /* absolute Filepos auf Disk */
    diskposition((int)(abspos / 512), fdpos);
    ReadSectorBETA(fdpos, 1, 0);
    tmpfile->filepos += strlen(nxFS_getstringlinefrompos(0x20000 + (abspos - ((abspos / 512)*512))));
    return nxFS_getstringlinefrompos(0x20000 + (abspos - ((abspos / 512)*512)));
}

int feof(TFILE *tmpfile)
{
    if (tmpfile->filepos == tmpfile->filesize || tmpfile->filepos > tmpfile->filesize)
        {return 1;} else {return 0;}
}

char* nxFS_readstringfromfilepos(char *filename2, unsigned int pos)
{
    char *tmpstr;
    DISKPOSITION *fdpos;
    int sektor = 0;
    diskposition(1, fdpos);
    ReadSectorBETA(fdpos, 1, 0);
    if (strcmp(nxFS_getstringfrompos(0x20009), filename2) == 0)
        sektor = 1;
    
    int i = 10;    
    
    if (sektor != 1)
    while(true)
    {
        diskposition(i, fdpos);
        ReadSectorBETA(fdpos, 0, 0);
        if (myreadallmem(0x20000) == 0x02) i += 8; /* nächsten Cluster lesen */
        if (myreadallmem(0x20000) == 0x01) /* File Entry gefunden! */
                {
                 if (strcmp(nxFS_getstringfrompos(0x20009), filename2) == 0) { sektor = i; break;}
                 i++;
                }
        if (myreadallmem(0x20000) == 0x00) break;
        if (i > 2880) break;
    }
        
    disablefloppymotor();
        
    if (sektor == 0) return 0; /* Datei wurde nicht gefunden! */
        
    sektor++; /* zum ersten Data Entry springen */
    if (nxFS_getint16rev(0x20109) < pos) return 0; /* Dateiende wurde erreicht! */
    int abspos = pos + (((pos / 4096) + 1) * 9) + (sektor*512); /* absolute Filepos auf Disk */
    diskposition((int)(abspos / 512), fdpos);
    ReadSectorBETA(fdpos, 1, 0);
    return nxFS_getstringlinefrompos(0x20000 + (abspos - ((abspos / 512)*512)));
}

void fclose(TFILE *datei)
{
datei->startsektor=0;
datei->filepos=0;
datei->absfilepos=0;
datei->filesize=0;
}
