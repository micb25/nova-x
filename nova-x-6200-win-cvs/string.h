#ifndef _STRING_H_
#define _STRING_H_

#include <stdarg.h>

#ifndef NULL
#define NULL 0
#endif


char strcmp(const unsigned char *s1, const char *s2);           /* Stringvergleich */
int strlen(const unsigned char *str);                           /* Ermittlung der Stringl�nge */
// noch nicht vollst�ndig implementiert:
char *strcat (char *s1, const char *s2);                        /* String anh�ngen */
void itoa(unsigned int number, char *string);                   /* Wandelt integer in String um */
int atoi(const unsigned char *str);                             /* Wandelt String in Integer um */
int toupper(int c);
int tolower(int c);
void strtoupper(char *s);
void strtolower(char *s);
char isdigit(unsigned char ch);
// noch ungetestet:
int strrchr(char *string, char c);
/* 1 falls c hexadezimale Ziffer ist */
int is_hex_digit(char c);
/* 1 falls c Buchstabe ist */
int is_alpha(char c);
/* 1 falls c ein whitespace ist */
int is_space(char c);
/* 1 falls c ein math. operatorzeichen ist; bei bedarf zu erweitern */
int is_op(char c);
/* 1 falls str in int konvertiert werden kann */
int is_int(char* str);
/* 1 falls str hexadezimalzahl ist */
int is_hex(char* str);
/* schreibt n als hex in buffer, returnt pointer auf buffer */
char* to_hex(int n, char* buffer);
/* schreibt n als zahl-string (zahlensystem mit basis n) in buf, returnt
   buf; 1<basis<10, sonst NULL --> f�r hex bitte to_hex benutzen */
char* int_to_str(int n, int bais, char* buf);
/* wie atoi, aber aus Symmetriegr�nden eingef�hrt */
int str_to_int(char* s);
/* anzahl . ben�tigten stellen von n in zahlensystem mit basis basis */
int n_digits(int n, int basis);
/* vergleicht n zeichen von s1 mit n zeichen von s2, 0 falls �bereinstimmung
   1 falls s1 gr��er ist (alphabetisch), sonst -1 */
int strncmp(const char* s1, const char *s2, unsigned int n);
/* schreibt src umgedreht in dest */
char* reverse_string(const char* src, char* dest);
/* durchsucht buf nach srch, gibt pos. als char* zur�ck, NULL falls
   nicht gefunden */
char* strstr(const char* buf, const char* srch);
/* na sagen wir 70% standardkonform, funktionalit�t f�r float fehlt noch */
int sprintf(char *str, const char *format, ...); /* TODO */
/* hier das gleiche */
int snprintf(char *str, unsigned int size, const char *format, ...); /* TODO */
/* laut ANSI eigentlich void* .. */
char *memset(char *s, int c, unsigned int n);
/* verschl�sselt einen String - key mit reverse_string umdrehen -> entschl�sseln */
char *crypt(char *str1, char* key);
  
#endif
