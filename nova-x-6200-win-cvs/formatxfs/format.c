/*
xnet- crew. 2002/2003
xnet-crew@gmx.net
http://www.xOS.net.tc

====================================
This system is written by the Nova-X Team! Its a preview version, so dont
be angry about bugs ;)
*/

#include <string.h>
#include <stdio.h>
#include "math.c"

/* DEBUG - Debuglevel:
 * 0 = No output at all.
 * 1 = Only heavy errors.
 * 2 = Heavy errors + Warnings.
 * 3 = Heavy errors + Warnings + Informational Stuff.
 * 4 = Heavy errors + Warnings + Informational Stuff + Common Values.
 *
 * Note: Errors will always force a resultcode of 2,
 *       Warnings set RC to 1, Sucess set RC to 0, (If catched: Unexpected Terminations set RC to 3).
 */
#define DEBUG 4

/* program related stuff... */
#define XFS_FIL "xfs.fil"
#define DISK_IMG "disk.img"
#define BOOT_BIN "boot.bin"
#define FILENAME_SIZE 255
#define DATA_ENTRY_DATA 4087
#define DATA_PADDING 235
#define BUFFERSIZE 512

/* Bit's of file-entries in fs-structure. */
#define fs_READ 1
#define fs_WRITE 2
#define fs_EXE 4
#define fs_HIDDEN 8
#define fs_DIR 16

/* Node- (Sector-) Types of fs-structure. */
#define fs_node_EMPTY 0
#define fs_node_FILE 1
#define fs_node_DATA 2

char FileList[1024][128];
unsigned int FileListIndex = 0;

unsigned long getfilesize(char *filename);
unsigned long get_file_size(char *filename);
unsigned long load_file_list();


typedef struct                                     /*Dateieintrag eines Blocks */
{
	unsigned char type;	/* 0 = unbenutzt. 1 = Dateieintrag 2= DATEN eintrag (fs_node_EMPTY/FILE/DATA)*/   
	unsigned long prev_entry;	/* LBA pointer to previous file entry */
	unsigned long next_entry;	/* LBA pointer to next file entry */
	unsigned char filename[FILENAME_SIZE];	/* File name, padded with NULLs */
	/* Attributes
	 * Bit 0: Read		fs_READ
	 * Bit 1: Write		fs_WRITE
	 * Bit 2: Execute	fs_EXE
	 * Bit 3: Hidden	fs_HIDDEN
	 * Bit 4: Directory	fs_DIR
	 * Rest are unused, and available for future expansion
	 */
	unsigned char attributes;
	unsigned long size;		/* size in bytes */
	unsigned long parent_dir;	/* LBA pointer to parent dir entry */
	unsigned long first_dataentry;	/* LBA pointer to next data struct */
	unsigned char padding[DATA_PADDING];
} __attribute__ ((packed)) t_file_entry;

typedef struct
{
	unsigned char type;	/* 0 = unused. 1 = file entry. 2 = data_entry */
	unsigned long prev_entry;	/* LBA pointer to previous data/file entry */
	unsigned long next_entry;	/* LBA pointer to next data/file entry */
	unsigned char data[DATA_ENTRY_DATA];
} __attribute__ ((packed)) t_data_entry;

unsigned long getfilesize(char *filename)
{
	/* Determine Sizes of Files mentionend to be in the fs-structure */
	FILE *in;
	unsigned long size=0;
	if( (in = fopen(filename, "rb")) == NULL )
	{
	    return 0;
	}
	fseek(in, 0, SEEK_END);
	size = ftell(in);
	fclose(in);
	return size;
}	


unsigned long get_file_size(char *filename)
{
	/* ummm. This function is only used, to determine the size of the fileNAMES file */
	FILE *f;
	unsigned i=0;
	
	if (! (f = fopen(filename, "r")) )
	{
	    if (DEBUG > 1)
	    {
		printf("[warning] get_file_size: Cannot read %s\r\n",filename);
	    }
	    i = 0;
	}
	else
	{
    	    i = 0;

	    while (!feof(f))
	    {
		char buffer[1];
		fread(buffer, 1, 1, f);
		i++;
	    }
	fclose(f);
	}

	return i;
}

unsigned long load_file_list(char *filename)
{
	unsigned long xFS_fil_file_len;
	xFS_fil_file_len = get_file_size( filename );
	if (xFS_fil_file_len == 0)
	{
	    if (DEBUG > 0)
	    {
		printf("[error] load_file_list: Unable to allocate %d bytes. Exiting.\r\n",xFS_fil_file_len);
	    }
	    exit(2);
	}
	else
	{
	    if (DEBUG > 2)
	    {
		printf("[info] load_file_list: Attempting to load filelist from %s ...",filename);
	    }
	}
	char *buffer = (char *) malloc(sizeof(char) * xFS_fil_file_len);
	int FileNameFill = 0;
	unsigned int i = 0;
	FILE *f;

	if ( (f = fopen(filename , "r")) != NULL )
	{
	    fread(buffer, xFS_fil_file_len, 1, f);

    	    for (i = 0; i < xFS_fil_file_len; i++)
		{		
	    	    if ( (buffer[i] == '\n') || (buffer[i] == '\r') ) /* Zusaetzlich CR abfangen */
			{
				FileList[FileListIndex][FileNameFill++] = '\0';
				if (buffer[i] == '\n')	/* Dieses natuerlich nur bei LF ... */
				    {
				    FileNameFill = 0;
				    FileListIndex++;
				    }
			}
			else
			{
				FileList[FileListIndex][FileNameFill++] = buffer[i];
			}
		}
	    if (DEBUG > 2)
	    {
		printf("done.\r\n");
	    }
	    return 1;
	}
	else
	{
	    if (DEBUG > 1)
	    {
		if (DEBUG > 2)
		{
		    printf("failure.\r\n");
		}
		else
		{
		    printf("[warning] load_file_list: Cannot load filelist from %s.\r\n",filename);
		}
	    }
	}
	fclose(f);
	free(buffer);
}

int main(int argc, char **argv) /* SO LAUTET DER STANDARD*/
{
	t_file_entry file_entry;
	t_data_entry data_entry;
	unsigned char *data;
	unsigned long _fs_start = 1;
	unsigned long filesize=0;
	unsigned long cur_lbasector = 0;
	char filename[FILENAME_SIZE];
	unsigned int no_of_files = 1;
	unsigned long file_loop=0;
	unsigned int no_of_proceeded_files = 0;	
	unsigned int loop=0, loop2;
	FILE *dev, *file , *bootfile;
	unsigned char buffer[BUFFERSIZE]; /* hornet: changed */
	size_t fwr_size=BUFFERSIZE; /* 512 */
	size_t fwr_nmemb=1; /* 1 */
	size_t hal;
	if (DEBUG > 3)
	{
        printf("[note] main: Size of t_bootsect = %d\n", sizeof(buffer));
	    printf("[note] main: Size of t_file_entry = %d\n", sizeof(t_file_entry));
	    printf("[note] main: Size of t_data_entry = %d\n", sizeof(t_data_entry));
	    printf("[note] main: Size of data_entry.data = %d\n", sizeof(data_entry.data));
	}
	/*
	 * Open -> Imagefile
	 */
	
	if(!(dev = fopen( DISK_IMG, "wb")))
	{
	    if (DEBUG > 0)
	    {
		printf("[error] main: Cannot write Imagefile %s. Exiting\r\n",DISK_IMG);
	    }
	    exit(2);
	}
	else
	{
	    if (DEBUG > 2)
	    {
		printf("[info] main: Imagefile %s sucessfully opened.\r\n",DISK_IMG);
	    }
	}
	
	/*
	 * Open <- Bootloader/sector
	 */
	 
	if(!(bootfile = fopen (BOOT_BIN, "rb")))
	{
	    if (DEBUG > 0)
	    {
		printf("[error] main: Cannot read Bootloader %s. Exiting.\r\n",BOOT_BIN);
	    }
	    exit(2);
	}
	else
	{
	    if (DEBUG > 2)
	    {
		printf("[info] main: Bootloader %s sucessfully opened.\r\n",BOOT_BIN);
	    }
	}
	
	/*
	 * Read the Bootloader/sector to memory ...
	 */
	 
	if (!(hal = fread( buffer, fwr_size, fwr_nmemb, bootfile))) /* HMHMHMHM.... Warum eigentlich hier ein if NOT ? ... evtl macht das dann unter Windows probleme hier .... muss man spaeter nochmal kucken... */
    	{
	    if (DEBUG > 2)
	    {
		printf("[info] main: Reading bootsector to buffer.\r\n");
	    }
	}
	else
	{
	    if (DEBUG > 0)
	    {
		printf("[error] main: Cannot read bootsector to buffer. Exiting.\r\n");
	    }
	    exit(2);
	}
	
	/*
	 * ...and write it to Imagefile.
	 */
	 
	if (loop = fwrite( buffer, fwr_size, fwr_nmemb, dev))    /* Bootsektor wird geschrieben */
	{
	    if (DEBUG > 2)
	    {
		printf("[info] main: Writing bootsector.\r\n");
	    }
	}
	else
	{
	    if (DEBUG > 0)
	    {
		printf("[error] main: Bootsector could not be written from buffer. Exiting.\r\n");
	    }
		exit(2);
	}
	fclose (bootfile);
	cur_lbasector++;           /* Aktueller Sektor+1 */

	if ( load_file_list( XFS_FIL ) == 0 )
	{
	    if (DEBUG > 0)
	    {
		printf("[error] main: Error in function load_file_list.\r\n");
		printf("[?!$&'] main: What the heck !?! This error cannot occure... exiting.\r\n");
	    }
	    exit(3);
	}
	if (DEBUG > 2)
	{
	    printf("[info] main: Processing %d Files...\r\n",FileListIndex);
	}
	for (no_of_files = 0; no_of_files < FileListIndex; no_of_files++)
	{
	    strcpy(filename, FileList[no_of_files]);
	    if (DEBUG > 2)
	    {
		printf("[info] main: Processing %s ... ", filename);
	    }
	    filesize = getfilesize(filename);
	    if (filesize == 0)
	    {
		if (DEBUG > 0)
		{
		    if (DEBUG > 2)
		    {
		    printf("*EMPTY FILE* " );
		    }
		    else
		    {
		    printf("[warning] main: Filesize of %s is %d.\r\n",filename,filesize);
		    }		    
		}
	    }
	    else
	    {
		if (DEBUG > 2)
		{
		    printf("%d bytes ", filesize);
		}
	    }
	    if ( (file = fopen(filename, "rb")) == NULL)
	    {
		if (DEBUG > 0)
		{
		    if (DEBUG > 2)
		    {
		    printf("READ ERROR. Skipping.\r\n");
		    }
		    else
		    {
		    printf("[warning] main: Skipping %s.\r\n",filename);
		    }		    
		}
	    }
	    else
	    {
		if (DEBUG > 2)
		{
		    printf("reading... \r\n");
		}
	        /* Auf dem Heap wird soviel Speicherplatz reserviert!! */
	        if (! (data = (unsigned char *) malloc(filesize+sizeof(data_entry.data))) )
		{
		    if (DEBUG > 0)
		    {
			printf("Insufficient memory. Exiting.\r\n");
		    }
		    exit(2);
		}
	        for (loop2 = 0; loop2 < (DATA_ENTRY_DATA - 1); loop2++) 
	        {
		    data[loop2] = 0;
		}
		/* -------------- */
 		for(loop=0; loop<filesize; loop++)
		{
		    data[loop] = fgetc(file);  /* Nach DATA kommen die gesamten Daten */
		}
		file_entry.type = fs_node_FILE; /* Used - File entry */
		file_entry.prev_entry = 0; /* Erste Datei */
		file_entry.next_entry = ceil( (double)filesize / (double) (DATA_ENTRY_DATA - 1) ) * 8 + 1 + cur_lbasector;
   		if (no_of_files == FileListIndex-1)
		{
		    file_entry.next_entry = 0;            
		}
		for (loop = 0; loop < (FILENAME_SIZE + 1); loop++)
		{
		    file_entry.filename[loop] = 0;
		}
		strcpy(file_entry.filename, filename);
		file_entry.attributes = (0xFF & ( fs_READ + fs_WRITE ));
		file_entry.size = ftell(file);
		file_entry.parent_dir = 0;
		file_entry.first_dataentry = cur_lbasector+1;
		/* Padding "leeren" */
		for (loop = 0; loop < (DATA_PADDING + 1); loop++)
		{
		    file_entry.padding[loop] = 0;
		}
    		fwrite(&file_entry, BUFFERSIZE, 1, dev); 
		cur_lbasector ++;
		/* runde auf ( filesize / 4087 ) */
		for(loop=0; loop < ceil((double) filesize / (double)sizeof(data_entry.data)); loop++)
		{
		    data_entry.type = fs_node_DATA;
		    if (loop == 0) 
        	    {
			data_entry.prev_entry = 0;
		    }
		    else
		    {
		         data_entry.prev_entry = cur_lbasector-8; 
		    }     
		    if(loop+1 == ceil((double) filesize / (double)sizeof(data_entry.data)))
		    {
			data_entry.next_entry = 0;
		    }
		    else
		    {
			data_entry.next_entry = cur_lbasector+8;	/* _fs_start+((loop+2)*8); */
		    }
    		    for (loop2 = 0; loop2 < (DATA_ENTRY_DATA -1); loop2++)
		    {
			data_entry.data[loop2] = 0;
		    }			
		    memcpy(data_entry.data, data+(loop*sizeof(data_entry.data)), sizeof(data_entry.data));
		    // Von Data nach date_entry.data kopieren und zwar haeppchenweise
	    	    if (! (fwrite(&data_entry, 4096, 1, dev)) )  // Und das schreiben
		    {
			if (DEBUG > 0)
			{
			    printf("WRITE ERROR. Exiting.\r\n");
			}
			exit(2);
		    }
	    	    cur_lbasector+=8;  // Sektor um 8 erhoehen.
		}
    		fclose(file);
		free(data);
		no_of_proceeded_files++;
	    }
	} /* no_Of_Files schleife */

	if((2880 - cur_lbasector) != 0)
	{
	    data = (unsigned char *) malloc(BUFFERSIZE);
	    if(!data) return;
	    memset(data, 0, BUFFERSIZE);
	    for (loop=0; loop == BUFFERSIZE; loop++)
	    {
		data[loop] = fs_node_EMPTY;
	    }
	    for(loop=0; loop<(500-cur_lbasector); loop++)  // 2880 = Disk Groesse (Standard 3,5'' 1.44MB)
	    {
	        fwrite(data, BUFFERSIZE, 1, dev);
	    }
	    free(data);
	}
	fclose(dev);
	if ( no_of_proceeded_files == no_of_files )
	{
	    if (DEBUG > 2)
	    {
		printf("[info] main: Diskimage sucessfully written.\r\n");
	    }
	    exit(0);
	}
	else
	{
	    if (DEBUG > 1)
	    {
		printf("[warning] main: %d File",(no_of_files - no_of_proceeded_files));
	    }
	    if ( (no_of_files - no_of_proceeded_files) > 1 )
	    {
		if (DEBUG > 1)
		{
		    printf("s were");
		}
	    }
	    else
	    {
		if (DEBUG > 1)
		{
		    printf(" was");
		}
	    }
	    if (DEBUG > 1)
	    {
		printf(" *NOT* migrated to the Imagefile.\r\n");
		printf("[warning] main: Your Imagefile might not work as expected.\r\n");
	    }
	    exit(1);
	}
}
