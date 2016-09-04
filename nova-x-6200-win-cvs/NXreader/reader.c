#include "novaX.h"
#include "nxFS.h"
unsigned int eingabe, zeile, zeichennr, spalte, pages[1024], size;
unsigned char buffer[1500], zeichen ;
int ignore, pageno, readsize, maxpage;


void createwindow(void){ /* Erstellt die "GUI" */
    cls();
    textbackground(1);

        for (spalte=1;spalte<80;spalte++) { /* Die horizontale Balken */
        gotoxy (spalte,1);
        printf (" ");
        gotoxy (spalte, 2);
        printf (" ");
        gotoxy (spalte,25); 
        printf (" ");
        }

        for (zeile=1;zeile<26;zeile++) { /* vertikale Balken */
        gotoxy (80,zeile);
        printf (" ");
        gotoxy (1,zeile);
        printf (" ");}


    textcolor(15); 
    gotoxy (28,1); /* Überschrift */
    printf (" Nova-X Reader ");
    gotoxy (2,25);
    printf ("Use Page Up/Page Down for scrolling or ESC to exit this program"); /* Beschriftung */
    gotoxy (70,25);
    printf ("Page:");
    gotoxy (76,25);
    printf (inttostr(pageno+1)); /* Page Nr */
    textbackground(0);      /* Standart Farbe */
    textcolor(15);
}

void clearboard(void){ /* Löscht die beschriebene Fläche */
    gotoxy(2,3);
    
     for (zeile = 0; zeile<22;zeile++){   /* Überschreiben der beschriebenen Fläche */
     gotoxy(2,zeile+3);
      for (spalte = 0; spalte<78;spalte++) printf (" ");
     }
     
     textbackground(1); /* Seitenzahl aktualisieren */
     textcolor(15);
     gotoxy (70,25);
     printf ("Page:");
     gotoxy (76,25);
     printf (inttostr(pageno+1));
     printf ("  ");
     textbackground(0);
     textcolor(15);

     gotoxy(2,3);
}

void main (void){
    pageno = 0;
    maxpage = 0;
    createwindow(); /* erstellt die "GUI" */
     size = loadfileintomem("gnulicense.txt", 1); /* Lädt die Datei in den Speicher */
    ReadFromMemory(1,0,&buffer,1500);
    gotoxy(2,3);
    pages[0] = 0;
      while (...) { /* Die eigentliche Programm Schleife */
       readsize = (size - pages[pageno]); /* die zu lesende Buffer-größe wird bestimmt */
        if (readsize > 1499) readsize = 1500; /* Ist die Größe über 1500, dann beschränke */

       ReadFromMemory(1,pages[pageno],&buffer[0],readsize); /* Lesen der Datei aus dem Speicher in den Buffer */
       zeile = 0;  /* Neue Seite */
       spalte = 0;
       zeichennr = 0;
       
       for (zeichennr = 0; zeile < 22; zeichennr++){ /* Jedes zeichennr wird dargestellt */
              ignore = false;  /* Wenn ignore = true, dann wird Zeichen nicht dargestellt */
              zeichen = buffer[zeichennr]; 

          if (zeichen < 31) { /* Wenn Zeichen ein Systemzeichen ist */
          ignore = true; }

          if (zeichen == 13) { /* ENTER - Neue Zeile*/
          zeile++;
          spalte = 0; 
          }
          
          if (zeichen == 9) {  /* Tab */
          spalte = spalte + 8;
          }
    
        if (spalte > 76) { spalte = 0; zeile++; } /* Zeile Voll */
        
        if (zeichennr >= readsize) break; /* Ende des Buffers erreicht */
        
        gotoxy (spalte+3, zeile+3); /* Cursor setzten */
        if (ignore == false){ printf (chrtostr(zeichen)); spalte++; } /* Zeichen darstellen */
} /* Ende der Zeichen-Darstell-Schleife  */
if (zeichennr < readsize) if (pageno >= maxpage) maxpage = pageno+1; /* maxpage - Variable schreiben, falls dies das Ende der Datei ist */

zeichennr--;
 if (maxpage-1 == pageno ) pages[pageno+1] = pages[pageno] + zeichennr; /* Pages wird aktualisiert */

gotoxy(100,100);  /* Coursor unsichtbar machen */
    while(...){ /* Eingabe-Schleife */    
     eingabe = getch(1); /* Eingabe = SCAN CODE */
     if (eingabe == 336) { 
        pageno++;
        if (pageno > maxpage) pageno = maxpage; /* Wenn die höchste Seite überschritten wurde */
        else break; 
        } /* page down */

     if (eingabe == 328) { /* Page Up */
     if (pageno != 0){
     pageno = pageno - 1;
     if (pageno <= 0) pageno = 0; /* es gibt keien Page -1 ! */
     break;}
     }

     if (eingabe == 256) {  /* ESC Taste */
     break;}

     }
if (eingabe == 256) break; /* Hauptschleife auch beenden */
clearboard(); /* Text löschen */
} /* Neue Page */
quit();                /* Ruft Interrupt auf */
} /* Ende von Main */ 
