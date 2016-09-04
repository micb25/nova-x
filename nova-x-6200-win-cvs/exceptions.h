#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#define EXCEPTIONTYPE_ABORT 1
#define EXCEPTIONTYPE_FAULT 2
#define EXCEPTIONTYPE_TRAP  3
#define EXCEPTIONTYPE_INT   4

extern dword getcr(unsigned char controlregister);

void exc0(dword errorCode, dword eip, dword cs, dword eflags);
void exc1(dword errorCode, dword eip, dword cs, dword eflags);
void exc2(dword errorCode, dword eip, dword cs, dword eflags);
void exc3(dword errorCode, dword eip, dword cs, dword eflags);
void exc4(dword errorCode, dword eip, dword cs, dword eflags);
void exc5(dword errorCode, dword eip, dword cs, dword eflags);
void exc6(dword errorCode, dword eip, dword cs, dword eflags);
void exc7(dword errorCode, dword eip, dword cs, dword eflags);
void exc8(dword errorCode, dword eip, dword cs, dword eflags);
void exc9(dword errorCode, dword eip, dword cs, dword eflags);
void exc10(dword errorCode, dword eip, dword cs, dword eflags);
void exc10(dword errorCode, dword eip, dword cs, dword eflags);
void exc12(dword errorCode, dword eip, dword cs, dword eflags);
void exc13(dword errorCode, dword eip, dword cs, dword eflags);
void exc14(dword errorCode, dword eip, dword cs, dword eflags);
void exc15(dword errorCode, dword eip, dword cs, dword eflags);
void exc16(dword errorCode, dword eip, dword cs, dword eflags);
void excxx(dword errorCode, dword eip, dword cs, dword eflags);

void showexception(
    const unsigned char *exceptmsg,
    int exctype,
    dword errorCode,
    dword eip,
    dword cs,
    dword eflags);
void printcenter(const unsigned char *strc);

#endif
