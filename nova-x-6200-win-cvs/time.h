#ifndef _TIME_H_
#define _TIME_H_

void getdate(t_date *temp);                  // Liest das aktuelle Datum aus
void gettime(t_time *temp);                  // Liest die aktuelle Uhrzeit aus
unsigned char BCD2binair(unsigned char b);   // Konvertiert eine Zahl ins Binärformat */
unsigned char binair2BCD (unsigned char b);  // Konvertiert eine Zahl ins BCD-Format
void outputTime(t_time *zeit);               // Gibt einen t_time struct aus
void outputDate(t_date *datum);              // Gibt einen t_date struct aus
NXbool isBCDformatset();
void setdate(t_date *datum);                 // Setzt das Datum des CMOS
void settime(t_time *zeit);                  // Setzt die Zeit des CMOS

#endif
