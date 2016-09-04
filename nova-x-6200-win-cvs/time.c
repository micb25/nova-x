/*
    Zeitfunktionen von MB
   
    updated by MB 18.08.03 11:10
        - BCD und Binärformat 
          -> neue Funktionen
        - Uhrzeit und Datum können gesetzt werden
    updated by thesame 19.08.03 10:37
    
    updated by MB 09.09.03 13:51 und thesames Veränderungen z.T. entfernt
    ( falsche Ausgabe von Zeit und Datum von thesame berichtigt )

*/

#include "time.h"

void getdate(t_date *temp) {    
    if (isBCDformatset() == true) {
        nxoutport(0x70, 0x07);                       /* Adresse 0x07 des CMOS adressieren(Tag) */  
        temp->day = BCD2binair(nxinport(0x71));         /* Adresse lesen und umwandeln */
        nxoutport(0x70, 0x08);                       /* Adresse 0x08 des CMOS adressieren(Monat) */  
        temp->month = BCD2binair(nxinport(0x71));   /* Adresse lesen und umwandeln */
        nxoutport(0x70, 0x09);                       /* Adresse 0x09 des CMOS adressieren(Jahreszahl) */  
        temp->year = BCD2binair(nxinport(0x71));    /* hintere Jahreszahl lesen z.B. 03 */
        temp->year = temp->year + 2000;            /* Wir sind schon im 21. Jahrhundert ;) */
    }
    if (isBCDformatset() == false) {
        nxoutport(0x70, 0x07);                       /* Adresse 0x07 des CMOS adressieren(Tag) */  
        temp->day = nxinport(0x71);
        nxoutport(0x70, 0x08);                       /* Adresse 0x08 des CMOS adressieren(Monat) */  
        temp->month = nxinport(0x71);
        nxoutport(0x70, 0x09);                       /* Adresse 0x09 des CMOS adressieren(Jahreszahl) */  
        temp->year = nxinport(0x71);                /* hintere Jahreszahl lesen z.B. 03 */
        temp->year = temp->year + 2000;            /* Wir sind schon im 21. Jahrhundert ;) */    
    }
}

void gettime(t_time *temp) {
    if (isBCDformatset() == true) {
        nxoutport(0x70, 0x00);                        /* Adresse 0x00 des CMOS adressieren(Sekunden) */  
        temp->seconds = BCD2binair(nxinport(0x71)); /* Adresse lesen und umwandeln */
        nxoutport(0x70, 0x02);                        /* Adresse 0x02 des CMOS adressieren(Minuten) */  
        temp->minutes = BCD2binair(nxinport(0x71));    /* Adresse lesen und umwandeln */
        nxoutport(0x70, 0x04);                        /* Adresse 0x04 des CMOS adressieren(Stunden) */  
        temp->hours = BCD2binair(nxinport(0x71));     /* Adresse lesen und umwandeln */
    }
    if (isBCDformatset() == false) {
        nxoutport(0x70, 0x00);                        /* Adresse 0x00 des CMOS adressieren(Sekunden) */  
        temp->seconds = nxinport(0x71);
        nxoutport(0x70, 0x02);                        /* Adresse 0x02 des CMOS adressieren(Minuten) */  
        temp->minutes = nxinport(0x71);
        nxoutport(0x70, 0x04);                        /* Adresse 0x04 des CMOS adressieren(Stunden) */  
        temp->hours = nxinport(0x71);    
    }
}

/*

    BCD Format:

    Hi Nibble   | Low Nibble
       Zehner   |      Einer

Bsp: 
 Binär 13 = 0000|1101b
 BCD   13 = 0001|0011b  = (1 * 10) + (3 * 1)
*/

unsigned char BCD2binair(unsigned char b) {
    return (b & 0x0f) + (((b & 0xF0) >> 4) * 10);
}

unsigned char binair2BCD(unsigned char b) {
    return ((b / 10) << 4) + (b - ((b / 10) * 10));
}

void outputTime(t_time *zeit) {
    // rewritten by thesame 15.08.03
    if (zeit->hours < 10)
        putchar('0');                // wenn die Zahl < 10 ist dann eine 0 davor schreiben 
    printf("%d:", zeit->hours);      // und mit einem abschließendem Doppelpunkt ausgeben
    if (zeit->minutes < 10)          // Das ganze wird für die Minuten und Sekunden wiederholt
        putchar('0');                
    printf("%d:", zeit->minutes);                 
    if (zeit->seconds < 10)
        putchar('0');
    outputInteger(zeit->seconds);                 
}


void outputDate(t_date *datum) {
    // rewritten by thesame 15.08.03
    if (datum->day < 10) { putchar('0'); }      // Wenn die Zahl < 10 ist, dann eine 0 davor schreiben
    printf("%d.", datum->day);                  // Und mit Trennungszeichen ausgeben
    if (datum->month < 10) { putchar('0'); }    // Das ganze für den Monat wiederholen
    printf("%d.%d", datum->month, datum->year); // und das Jahr ausgeben
}

NXbool isBCDformatset() {
    nxoutport(0x70, 0x0B);
    if ((nxinport(0x71) & 0x04) == 0)
        return 1;
    nxoutport(0x70, 0x0B);
    if ((nxinport(0x71) & 0x04) == 4)
        return 0;
}

void setdate(t_date *datum) {
    if (isBCDformatset() == true) {
        nxoutport(0x70, 0x07);                        /* Adresse 0x07 des CMOS adressieren(Tag) */  
        nxoutport(0x71, binair2BCD(datum->day));      /* Tag schreiben */
        nxoutport(0x70, 0x08);                        /* Adresse 0x08 des CMOS adressieren(Monat) */  
        nxoutport(0x71, binair2BCD(datum->month));    /* Monat schreiben */
        nxoutport(0x70, 0x09);                        /* Adresse 0x09 des CMOS adressieren(Jahreszahl) */  
        nxoutport(0x71, binair2BCD(datum->year - 2000));/* hintere Jahreszahl schreiben */
        return;
    }
    if (isBCDformatset() == false) {
        nxoutport(0x70, 0x07);
        nxoutport(0x71, datum->day);
        nxoutport(0x70, 0x08);
        nxoutport(0x71, datum->month);
        nxoutport(0x70, 0x09);
        nxoutport(0x71, datum->year - 2000);
        return;
    }
}

void settime(t_time *zeit) {
    if (isBCDformatset() == true) {
        nxoutport(0x70, 0x00);                        /* Adresse 0x00 des CMOS adressieren(Sekunden) */  
        nxoutport(0x71, binair2BCD(zeit->seconds));   /* Sekunden schreiben */
        nxoutport(0x70, 0x02);                        /* Adresse 0x02 des CMOS adressieren(Minuten) */  
        nxoutport(0x71, binair2BCD(zeit->minutes));   /* Minuten schreiben */
        nxoutport(0x70, 0x04);                        /* Adresse 0x04 des CMOS adressieren(Stunden) */  
        nxoutport(0x71, binair2BCD(zeit->hours));     /* Stunden schreiben */
        return;
    }
    if (isBCDformatset() == false)                    /* und alles nochmal, wenn es sich nicht um das BCD Format handelt*/
    {
        nxoutport(0x70, 0x00);                     
        nxoutport(0x71, zeit->seconds);
        nxoutport(0x70, 0x02);                     
        nxoutport(0x71, zeit->minutes);
        nxoutport(0x70, 0x04);
        nxoutport(0x71, zeit->hours);
        return;
    }
}
