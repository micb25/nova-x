/*

    Task Deskriptoren

*/

#ifndef TSS_C_
#define TSS_C_

extern tss_update_gdt();
extern tss_testt();

unsigned short tss_getcurrentdesc()
{
    return ( (((unsigned char)readallmem(0x50001) & 0xFF) << 8)+((unsigned char)readallmem(0x50000) & 0xFF)) / 8;
}

void tss_setcurrentdesc(unsigned short descs)
{
    descs *= 8;
    writeallmem(0x50000, descs & 0xFF);    
    writeallmem(0x50001, (descs & 0xFF00) >> 8);    
}

void tss_test()
{
    int i, i2;
    outputInteger(tss_getcurrentdesc());
    printf("\n");
    tss_setcurrentdesc(tss_getcurrentdesc()+1);
    outputInteger(tss_getcurrentdesc());
    printf("\n");
    tss_update_gdt();
    for (i2=0; i2<10; i2++)
    {
        printf("\nSelektor: ");outputInteger(i2*8);
        printf("\n");
        for (i=0;i<8;i++)
            {
                    outputHexc(readallmem(0x50000+i+(i2*8)));
            }
    }
    printf("\nSize: ");
    outputInteger(tss_getcurrentdesc());

}   


#endif
