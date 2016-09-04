/*

xPad 0.4 Alpha 

xOS Development Team

Please note: xOS will be named to nova-x.

*/

int xReg_ft_1, xReg_ft_2;

int y=3; /*Position auf der y achse*/

int x=4, i;

unsigned int eingabe, zeile, zeichennr, spalte, pages[1024], size;



void xPad(void)

{

xReg_ft_1=strdef("*.xtxt");

    cls();

    textbackground(1);



        for (spalte=1;spalte<80;spalte++) { /* Die horizontale Balken */

        gotoxy (spalte,1);

        outputString (" ");

        gotoxy (spalte, 2);

        outputString (" ");

        gotoxy (spalte,25); 

        outputString (" ");

        }



        for (zeile=1;zeile<26;zeile++) { /* vertikale Balken */

        gotoxy (80,zeile);

        outputString (" ");

        gotoxy (1,zeile);

        outputString (" ");}





    textcolor(15); 

    gotoxy (30,1); /* Überschrift */

    outputString (" xPad 0.4 Alpha");

    gotoxy (2,25);

    outputString ("xPad 0.4 Alpha"); /* Beschriftung */

    gotoxy (70,25);

    outputString (xReg_ft_1);

    gotoxy (76,25);

    textbackground(0);      /* Standart Farbe */

    textcolor(15); 

    gotoxy(3,4); /*Setze Cursor */

    

    while(...){

    gotoxy(x,++y); /* Cursor wandert nach unten, ohne den Seitenrand zu durchkreuzen */

    outputString(" ");

    i=gets();

    outputString("\n\r");

    

    if(strcmp(i,"!html")==false){

    textcolor(COLOR_NAVY);

    xReg_ft_2=strdef("*.xhtml"); /* Registriere xhtml */

    }

     else if(strcmp(i,"!quit")==false){

     textcolor(COLOR_LIGHTGRAY);

     gotoxy(3,4); /*Setze Cursor */

     

    }

    eingabe = getchar();

    if (eingabe == 327) { /* Page Up */

    gotoxy(x,--y);

    outputString(" ");

    }

    if (eingabe == 335) { /* Page Down */

    gotoxy(x,++y);

    outputString(" ");

    }

    if (eingabe == 330) { /* <- */

    gotoxy(--x,y);

    outputString(" ");

    }

    if (eingabe == 332) { /* -> */

    gotoxy(++x,y);

    outputString(" ");

    }

    if(y==23) { /* Wir wollen ja nicht über den Rand schreiben... */

    y=3; /* Setze den Cursor wieder auf seine Ausgangsposition */

    xPad();

    }

}

}

