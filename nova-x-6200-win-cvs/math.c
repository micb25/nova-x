/*

    MATH Funktionen von MB

    werden noch erweitert! 
    
*/

#ifndef _MATH_C_
#define _MATH_C_

#ifndef PI
 #define PI 3.1415926535897932385
#endif

int round(float fzahl) /* so einfach kann man runden ;) */
{
    if (fzahl > 0) return (int) (fzahl + 0.5); 
    if (fzahl < 0) return (int) (fzahl - 0.5);
}

int power(int basis, unsigned int exponent) /* x hoch y */
{
    int tmpint = 1; 
    while (exponent--) tmpint *= basis;
    return tmpint;
}

int iabs(int zahl) /* Betrag einer Zahl */
{
    if (zahl < 0) return -zahl; else return zahl;
}

double dabs(double zahl) /* Betrag einer Zahl */
{
    if (zahl < 0) return -zahl; else return zahl;
}

float fabs(float zahl) /* Betrag einer Zahl */
{
    if (zahl < 0) return -zahl; else return zahl;
}

int sqr(int zahl) /* gibt das Quadrat einer Zahl zurück */
{
    return power(zahl, 2);
}

float arcsin(float alpha) /* Bogenmaß */
{
    /*       
        b =  Alpha * PI / 180
    */    
    return alpha * (PI / 180);
}

#endif
