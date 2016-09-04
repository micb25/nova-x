/*
                                           
    xCalc for nova-x
                                           
                                           
    15.01.2003                             
   ------------
    Startet to work on xCalc                
    entered basic operations: + - * /
    
    16.01.2003
   ------------
    
     xcalc.c :
      void main() Requests digits from the user and call calculation routines
      
     xcalcman.c
      void man_xcalc()
 

*/

/* TODO (Filbert#1#): Weiter Rechenoperationen einbauen z.B. 
                      Wurzel / Log / Sin / Cos / Tan ... */
/* TODO (Filbert#2#): Bugs beseitigen, z.B. editier- Möglichkeiten */

/* Rechner */
#include "novaX.h"

int digit;
int first;
int second;
int op;

void main()
{
/* initializing Variables */

/* starting User Interface */
	textcolor(COLOR_LIGHTGRAY);
    printf("Enter your digits and operator\n\r");
	printf("Your first digit: ");
	textcolor(COLOR_YELLOW);
    digit = 0;
    first = 0;
    while(...)
    {
     digit = getch(1);
     if (digit == 257)
     {
       first = first*10;
       first = first+1;
       printf("1");
     } 
     else if (digit == 258)
     {
       first = first*10;
       first = first+2;
       printf("2");
     } 
     else if (digit == 259)
     {
       first = first*10;
       first = first+3;
       printf("3");
     } 
     else if (digit == 260)
     {
       first = first*10;
       first = first+4;
       printf("4");
     } 
     else if (digit == 261)
     {
       first = first*10;
       first = first+5;
       printf("5");
     } 
     else if (digit == 262)
     {
       first = first*10;
       first = first+6;
       printf("6");
     } 
     else if (digit == 263)
     {
       first = first*10;
       first = first+7;
       printf("7");
     } 
     else if (digit == 264)
     {
       first = first*10;
       first = first+8;
       printf("8");
     } 
     else if (digit == 265)
     {
       first = first*10;
       first = first+9;
       printf("9");
     } 
     else if (digit == 266)
     {
       first = first*10;
       first = first+0;
       printf("0");
     } 
     else if (digit == 283)
     {
      break;
     } 
     else 
     {
      /*do nothing when another key is pressed*/
     }
    } 
	textcolor(COLOR_LIGHTGRAY);
	printf("\n\r");
	printf("Your second digit: ");
	textcolor(COLOR_YELLOW);
    digit = 0;
    while(...)
    {
     digit = getch(1);
     if (digit == 257)
     {
       second = second*10;
       second = second+1;
       printf("1");
     } 
     else if (digit == 258)
     {
       second = second*10;
       second = second+2;
       printf("2");
     } 
     else if (digit == 259)
     {
       second = second*10;
       second = second+3;
       printf("3");
     } 
     else if (digit == 260)
     {
       second = second*10;
       second = second+4;
       printf("4");
     } 
     else if (digit == 261)
     {
       second = second*10;
       second = second+5;
       printf("5");
     } 
     else if (digit == 262)
     {
       second = second*10;
       second = second+6;
       printf("6");
     } 
     else if (digit == 263)
     {
       second = second*10;
       second = second+7;
       printf("7");
     } 
     else if (digit == 264)
     {
       second = second*10;
       second = second+8;
       printf("8");
     } 
     else if (digit == 265)
     {
       second = second*10;
       second = second+9;
       printf("9");
     } 
     else if (digit == 266)
     {
       second = second*10;
       second = second+0;
       printf("0");
     } 
     else if (digit == 283)
     {
      break;
     } 
     else 
     {
      /*do nothing when another key is pressed*/
     }
    } 
    textcolor(COLOR_LIGHTGRAY);
	printf("\n\r");
	printf("Your Operator: ");
	textcolor(COLOR_RED);
    op = scanf();
	textcolor(COLOR_LIGHTGRAY);
    printf("\n\r\n\r"); 

/*Werte Operator aus*/

/*------------------*/
/* Operator /       */
/*------------------*/
  if (strcmp(op,"/")==true)
  {
   textcolor(COLOR_YELLOW);
   printf(inttostr(first));
   textcolor(COLOR_RED);
   printf(op);
   textcolor(COLOR_YELLOW);
   printf(inttostr(second));
   textcolor(COLOR_RED);
   printf("=");
   textcolor(COLOR_YELLOW);
   if (second == 0)
   {
    textcolor(COLOR_RED);
    printf("Division by zero error ");
    printf(first);
    printf(" / ");
    printf(second);
    textcolor(COLOR_LIGHTGRAY);
    printf("\n\r");
   }
   printf(first/second); 
   textcolor(COLOR_LIGHTGRAY);
  } 

/*------------------*/
/* Operator *       */
/*------------------*/
  if (strcmp(op,"*")==true)
  {
   textcolor(COLOR_YELLOW);
   printf(inttostr(first));
   textcolor(COLOR_RED);
   printf(op);
   textcolor(COLOR_YELLOW);
   printf(inttostr(second));
   textcolor(COLOR_RED);
   printf("=");
   textcolor(COLOR_YELLOW);
   printf(inttostr(first* second));
   textcolor(COLOR_LIGHTGRAY);
   } 

/*------------------*/
/* Operator +       */
/*------------------*/
  if (strcmp(op,"+")==true)
  {
   textcolor(COLOR_YELLOW);
   printf(inttostr(first));
   textcolor(COLOR_RED);
   printf(op);
   textcolor(COLOR_YELLOW);
   printf(inttostr(second));
   textcolor(COLOR_RED);
   printf("=");
   textcolor(COLOR_YELLOW);
   printf(inttostr(first+ second));
   textcolor(COLOR_LIGHTGRAY);
   } 

/*------------------*/
/* Operator -       */
/*------------------*/
  if (strcmp(op,"-")==true)
  {
   textcolor(COLOR_YELLOW);
   printf(inttostr(first));
   textcolor(COLOR_RED);
   printf(op);
   textcolor(COLOR_YELLOW);
   printf(inttostr(second));
   textcolor(COLOR_RED);
   printf("=");
   textcolor(COLOR_YELLOW);
   printf(inttostr(first- second));
   textcolor(COLOR_LIGHTGRAY);
   } 
getch(1);
quit();  /* beende xCalc und geh zum kernel zurück */
}



