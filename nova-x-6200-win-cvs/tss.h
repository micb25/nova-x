/*

    Task Deskriptoren

*/

#ifndef TSS_H_
#define TSS_H_

#define TSS_TYPE_80286          1
#define TSS_TYPE_80286_ACTIVE   3
#define TSS_TYPE_80386          9
#define TSS_TYPE_80286_ACTIVE   11

void tss_test();
unsigned short tss_getcurrentdesc();

#endif
