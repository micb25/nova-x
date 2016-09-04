/*
    Exceptions by MB
    Das sind neue Expetion Handler. Sind schöner als die anderen und auch 
    nützlicher, da die Adresse angezeigt wird, wo der Fehler aufgetreten ist.
    Letzte Änderung: 11.07.03 17:01
    
*/

#include "typedefs.h"
#include "exceptions.h"

/* 
   Hier fangen die Exceptions an. Es wird durch die Zwischen-Prozedur bestimmt, um welche
   Fehlerart es sich handelt. Der ASM Befehl macht einen leichten Abruf der Adresse möglich.
*/

void exc0(dword errorCode, dword eip, dword cs, dword eflags) {
    asm ("add $8, %esp");
    showexception("Interrupt 0 - Divide by Zero", EXCEPTIONTYPE_FAULT, errorCode, eip, cs, eflags);
}

void exc1(dword errorCode, dword eip, dword cs, dword eflags) {
    asm ("add $8, %esp");    
    showexception("Interrupt 1 - Single Step", EXCEPTIONTYPE_FAULT, errorCode, eip, cs, eflags);
}

void exc2(dword errorCode, dword eip, dword cs, dword eflags) {
    asm ("add $8, %esp");
    showexception("Interrupt 2 - Non Maskable Interrupt", EXCEPTIONTYPE_INT, errorCode, eip, cs, eflags);
}

void exc3(dword errorCode, dword eip, dword cs, dword eflags) {
    asm ("add $8, %esp");
    showexception("Interrupt 3 - Breakpoint", EXCEPTIONTYPE_TRAP,errorCode, eip, cs, eflags);
}

void exc4(dword errorCode, dword eip, dword cs, dword eflags) {
    asm ("add $8, %esp");
    showexception("Interrupt 4 - Overflow", EXCEPTIONTYPE_TRAP, errorCode, eip, cs, eflags);
}

void exc5(dword errorCode, dword eip, dword cs, dword eflags) {
    asm ("add $8, %esp");
    showexception("Interrupt 5 - BOUND Range Exceeded", EXCEPTIONTYPE_FAULT, errorCode, eip, cs, eflags);
}

void exc6(dword errorCode, dword eip, dword cs, dword eflags) {
    asm ("add $8, %esp");
    showexception("Interrupt 6 - Invalid Opcode", EXCEPTIONTYPE_FAULT, errorCode, eip, cs, eflags);
}

void exc7(dword errorCode, dword eip, dword cs, dword eflags) {
    asm ("add $8, %esp");
    showexception("Interrupt 7 - No FPU found", EXCEPTIONTYPE_FAULT, errorCode, eip, cs, eflags);
}

void exc8(dword errorCode, dword eip, dword cs, dword eflags) {
    asm ("add $8, %esp");
    showexception("Interrupt 8 - Double Fault Exception", EXCEPTIONTYPE_ABORT, errorCode, eip, cs, eflags);
}

void exc9(dword errorCode, dword eip, dword cs, dword eflags) {
    asm ("add $8, %esp");
    showexception("Interrupt 9 - FPU Segment Overrun", EXCEPTIONTYPE_FAULT, errorCode, eip, cs, eflags);
}

void exc10(dword errorCode, dword eip, dword cs, dword eflags) {
    asm ("add $8, %esp");
    showexception("Interrupt 10 - Invalid Task State Segment", EXCEPTIONTYPE_FAULT,errorCode, eip, cs, eflags);
}

void exc11(dword errorCode, dword eip, dword cs, dword eflags) {
    asm ("add $8, %esp");
    showexception("Interrupt 11 - Segment Not Present", EXCEPTIONTYPE_FAULT,errorCode, eip, cs, eflags);
}

void exc12(dword errorCode, dword eip, dword cs, dword eflags) {
    asm ("add $8, %esp");
    showexception("Interrupt 12 - Stack Fault", EXCEPTIONTYPE_FAULT,errorCode, eip, cs, eflags);
}

void exc13(dword errorCode, dword eip, dword cs, dword eflags) {
    asm ("add $8, %esp");
    showexception("Interrupt 13 - General Protection Exception", EXCEPTIONTYPE_FAULT, errorCode, eip, cs, eflags);
}

void exc14(dword errorCode, dword eip, dword cs, dword eflags) {
    asm ("add $8, %esp");
    showexception("Interrupt 14 - Page Fault", EXCEPTIONTYPE_FAULT, errorCode, eip, cs, eflags);
}

void exc15(dword errorCode, dword eip, dword cs, dword eflags) {
    asm ("add $8, %esp");
    showexception("Interrupt 15 - Intel reserved", 0, errorCode, eip, cs, eflags);
}

void exc16(dword errorCode, dword eip, dword cs, dword eflags) {
    asm ("add $8, %esp");
    showexception("Interrupt 16 - FPU Fault", EXCEPTIONTYPE_FAULT, errorCode, eip, cs, eflags);
}

void excxx(dword errorCode, dword eip, dword cs, dword eflags) {
    asm ("add $8, %esp");
    showexception("Interrupt XX - Interrupts 17-31", EXCEPTIONTYPE_FAULT, errorCode, eip, cs, eflags);
}

// Das Muster, nachdem alle Exceptions angezeigt werden 
// ACHTUNG : EFLAGS müssen nicht immer richtig sein.
void showexception(
    const unsigned char *exceptmsg,
    int exctype,
    dword errorCode,
    dword eip,
    dword cs,
    dword eflags)
{
    textcolor(COLOR_LIGHTGRAY);
    textbackground(COLOR_BLACK);
    printf("\nAn exception occured! Press any key...");
    getchar();
    textcolor(COLOR_WHITE);
    textbackground(COLOR_LIGHTRED);
    cls();

    printf("%c9%c9%c9  NOVA-X KERNEL PANIC%c9%c9%c9%c7\n", ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    textbackground(COLOR_BLACK);
    gotoxy(29, 4);
    gettime(&currenttime);
    getdate(&currentdate);
    printf("@ %a %t\n\n", &currentdate, &currenttime);
    textcolor(COLOR_LIGHTRED);
    printcenter(exceptmsg);
    outputString("\n");
    textcolor(COLOR_WHITE);
    switch(exctype)            // Welche Exception-Art?
    {
        case EXCEPTIONTYPE_ABORT:
        printcenter("exception class: abort");
        break;
        case EXCEPTIONTYPE_FAULT:
        printcenter("exception class: fault");
        break;
        case EXCEPTIONTYPE_TRAP:
        printcenter("exception class: trap");
        break;
        case EXCEPTIONTYPE_INT:
        printcenter("exception class: interrupt");
        break;
    }
    gotoxy(20, wherey() + 3);
    outputString("EFLAGS: ");
    outputBinaryl(eflags);
    gotoxy(30, wherey() + 1);
    printf("CS:EIP = 0x");
    outputHex(cs);
    outputString(":0x");
    outputHex(eip);
    gotoxy(1, 13);
    printcenter("Control Register:");
    gotoxy(15, 14);
    printf("CR0 = ");
    outputBinaryl(getcr(0));
    printf(" 0x");
    outputHexl(getcr(0));
    gotoxy(15, 15);
    printf("CR2 = ");
    outputBinaryl(getcr(2));
    printf(" 0x");
    outputHexl(getcr(2));
    gotoxy(15, 16);
    printf("CR3 = ");
    outputBinaryl(getcr(3));
    printf(" 0x");
    outputHexl(getcr(3));
    gotoxy(15, 17);
    printf("CR4 = ");
    outputBinaryl(getcr(4));
    printf(" 0x");
    outputHexl(getcr(4));
    gotoxy(14, 19);
    printf("nova-x is running since %d hours, %d minutes and %d seconds", gettickscount() / 100 / 60 / 60, gettickscount() / 100 / 60, gettickscount() / 100);
    gotoxy(1, 22);
    printcenter("The system has been halted!");
    gotoxy(1, 23);
    printcenter("Press any key to reboot...");
    getchar();
    reboot();  
    asm("hlt"); // Falls reboot fehlschlägt
    while(1) {}    
}

// Diese Funktion schreibt einen Text zentral auf den Bildschirm.
void printcenter(const unsigned char *strc) {
    gotoxy((80 - (char)strlen(strc)) / 2, wherey());
    while (*strc) {
        outputCharacter(*strc++);
    }
}
