/*

    nxFS
    Nova-X Filesystem 

    version 0.01a by MB
  
    Funktionen mit dem Prefix 'nxFS' sollten nicht direkt benutzt werden!
        
*/

#include "fdc.h"

typedef struct {
    char * filename;
    unsigned int startsektor;
    unsigned int filepos;
    unsigned int absfilepos;
    unsigned int filesize;
    unsigned int id;
} TFILE;


void nxFS_init();
void dir();
char* nxFS_getstringfrompos(int pos);
char* nxFS_getnstringfrompos(int pos, int count);
char *nxFS_getstringlinefrompos(int pos);
int nxFS_getint16(int pos);
int nxFS_getint16rev(int pos);
unsigned int nxFS_getuint16(int pos);
unsigned int nxFS_getuint16rev(int pos);
unsigned char myreadallmem(unsigned int pos);
void nxFS_printattr(unsigned char attr);
void fopen(TFILE* tmpfile, char *filename);
int feof(TFILE *tmpfile);
char* nxFS_readstringfromfilepos(char *filename, unsigned int pos);
