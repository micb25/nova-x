/*  */
/*  */
/*  */
/*  */
/*  */
/*  */
/*  */

void manual(void) {
	cls();
	printf("   \n\r");
	textcolor (COLOR_YELLOW);
	printf("Please specify which manual you want to see (for example 'man install')!\n\r");
    textcolor(7);
}

void man_install(void) {
  cls();
  printf("=========================================\n\r");
  printf("                                         \n\r");
  printf("           Nova-X Manual - install       \n\r");
  printf("                xMan V. 1.0              \n\r");
  printf("=========================================\n\r");
  printf("   \n\r");
  printf("How to install?\n\r");
  printf("---------------\n\r\n\r");
  printf("1. Start Windows.\n\r");
  printf("2. Enter a floppy.\n\r");
  printf("3. Go into your Nova-X directory and run install.bat.\n\r");
  printf("4. When a window pops up with the message\n\r");
  printf("   'Image successfully written' the installation is\n\r");
  printf("   complete.\n\r");
  printf("5. Reboot your computer.\n\r\n\r");
    
  printf("Make sure that the computer boots from floppy. If it doesn't change the\n\r");
  printf("neccessary settings in your BIOS. If everything is correct installed, written\n\r");
  printf("on the disk and the BIOS tries to boot from floppy but Nova-X doesn't boot note\n\r");
  printf("this to the developers of Nova-X.\n\r\n\r");
   
  printf("Caution! All Data on the disk will be overwritten by the installation.\n\r\n\r");
}

