#include "commands.h"

char *dummy_space = ".........................................................................";

char *tl1 = "=========================================";
char *tl2 = "-----------------------------------------";
//char *tl3 = "-----------------------------------------\n";
//char *tl4 = "=========================================\n\n";

void command_todo() {
    cls();
    printf("%s\n",tl1);
    printf("%s\n\n",tl2);
    printf("           nova-x - Todo-List\n\n");
    printf("%s\n",tl2);
    printf("%s\n\n",tl1);
    printf("To Do...\n");
    printf("--------\n");
    printf("\n\t Graphical User Interface (GUI)");
    printf("\n\t Multi Threading");
    printf("\n\t Paging");
    printf("\n\t Memory Management");
    printf("\n\t User Management System\n\n");
}

void command_copyright() {
    cls();
    printf("%s\n",tl1);
    printf("%s\n\n",tl2);
    printf("            nova-x Copyright\n\n");
    printf("%s\n",tl2);
    printf("%s\n\n",tl1);
    printf("nova-x Copyright:\n");
    printf("---------------\n");
    printf("nova-x and nova-x file system copyright 2002, 2003 by Roman Lossin-Bessler,\n");
    printf("Niels Goeran Blume, Karl Oberdieck, Vitali Mueller, Gernot Fritz, \n");
    printf("David Rieger, Tobias Philipp, Anton Zolotov, Stephan Seitz, Mathias Korte,\n");
    printf("Michael Boehme.\n\n");
}

void command_bug() {
    cls();
    textcolor(COLOR_PURPLE);
    printf("If you find bugs in nova-x you can do the following:\n\n");
    textcolor(COLOR_LIGHTGRAY);
    printf("\t Post the bug/problem in the nova-x Forum (");
    textcolor(COLOR_WHITE);
    printf("board.nova-x.net");
    textcolor(COLOR_LIGHTGRAY);
    printf(")\n");
    printf("\t Visit www.nova-x.net to get contact information.\n");
    printf("\t Visit the nova-x team by IRC (#nova-X on ircnet)\n");
    printf("\t Reboot your PC and try the action where the error happened again.\n\n");
}

void command_help() {
    cls();
    printf("%s\n",tl1);
    printf("%s\n\n",tl2);
    printf("             nova-x Helpfile\n\n");
    printf("%s\n",tl2);
    printf("%s\n\n",tl1);
    printf("Commands:\n");
    printf("---------\n");
    printf("\n\t help           Displays this screen");
    printf("\n\t clear/clr/cls  Clears the screen");
//    printf("\n\t credits         Displays the nova-x Credits");
    printf("\n\t restart/reboot Restarts the computer");
//    printf("\n\t man             Shows a manualpage. For example: 'man install'");
    printf("\n\t todo           Displays the nova-x Todo-List");
    printf("\n\t copyright      Shows nova-x copyright");
    printf("\n\t bug            If you find bugs in nova-x, enter that command.");
    printf("\n\t license        Shows nova-x license.");
    printf("\n\t logo           Show the nova-x logo.");
    printf("\n\t time/date      Shows the current time/date");
    printf("\n\t set time/date  Sets the current time/date\n");    
    textcolor(COLOR_LIME);
    printf("\nAll commands are case-sensitive.\n");
    textcolor(COLOR_LIGHTGRAY); 
}

void command_logo() {
    cls();
    textcolor(COLOR_WHITE);
    printf("                       ..........\n");
    printf("                      //////////\n"); textcolor(COLOR_NAVY);
    printf("                     //######//\n"); textcolor(COLOR_WHITE);
    printf("  ##########");                              textcolor(COLOR_NAVY);
    printf("        //######//\n");
    printf("   #@@@@@@@@#      //######//\n");
    printf("    #@@@@@@@@#    //######//\n");
    printf("     #@@@@@@@@#  //######//\n");
    printf("      #@@@@@@@##//######//\n");
    printf("       #@@@@@@@@@@@@@@#//\n");
    printf("        #@@@@@@@@@@@@#//");                textcolor(COLOR_WHITE); printf("   nova-x  \n"); textcolor(COLOR_NAVY);
    printf("         ##@@@@@@@@@##");                   textcolor(COLOR_WHITE); printf("  xnet-crew@gmx.net\n"); textcolor(COLOR_NAVY);
    printf("         ##@@@@@@@@@##");               textcolor(COLOR_WHITE); printf("nova-x 0.0.6   \n"); textcolor(COLOR_NAVY);
    printf("        ###@@@@@@@@@###");                 textcolor(COLOR_WHITE); printf("   nova-x dev team  \n"); textcolor(COLOR_NAVY);
    printf("       ###@@@@@@@@@@@###\n");
    printf("      ###@@@@@##@@@@@@###\n");
    printf("     ###@@@@@@###@@@@@@###\n");
    printf("    ###@@@@#### ###@@@@@###\n");
    printf("   ##@@@@@###    ###@@@@@@##\n"); textcolor(COLOR_WHITE);
    printf("  ##########      ###########\n\n");
    printf("%s\n\n",tl2);
    textcolor(COLOR_LIGHTGRAY);
}

void command_license() {
	printf("nova-x is released under GNU (General Public License).\n");
}
