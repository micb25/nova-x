/* 32 BIT Kernel */

#include "includes.h" /* hier sind alle Includes aufgelistet! */

// nsh.h -------------
char prompt[256];
struct nsh_var
{
    char identifier[16];
    char content[1024];
};
void nsh_get_var(struct nsh_var *variable);
void nsh_put_var(struct nsh_var *variable);
// --------------------

void bootprocess(){
    initvideo();    /* Video Einstellungen */
    textcolor(COLOR_GREEN);
    printf("OK\n"); /* So, Meldung, das wir im PM sind */
    textcolor(COLOR_LIGHTGRAY);   
    printf("Initializing Keyboard              : ");
    initkeyb();     /* Tastatur-Buffer leeren */
    initkbmap();    /* Keyboard Map initialisieren */
    setLEDs(0,1,0); // Tastatur LEDs werden gesetzt
    printf("OK\n"); /* So, Meldung, das wir im PM sind */
//    printf("Checking Memory                    : %d MB\n",memorysize());
    printf("Initializing Paging                : ");
    initzPaging();
    printf("OK\n");
//    cpubootInfos();    // Gibt unter VMware ne Exception !?!?!
    printf("Setting CPU into Paging Mode       : ");
    setPaging();
    printf("OK\n"); 
    checkFPU();
    printf("Setting FPU into Protected Mode    : ");
    setFPUPM();
    printf("OK\n");
    
    DEBUG_init(); /* Debug Console erstellen */
    
    initprinter(); /* überprüft welche LPT Ports vorhanden sind */
    
    initalizingFDC();
    printf("\n");
    
    printf("Please wait while initializing the nxFS!\n");
    nxFS_init();
    
    checkcmos();
}

// nsh.c -------------------------
//#include nsh.h

    

int nsh( int nsh_pid ) {
    int exit = 0;
    int rc = 0;
    int i = 0;
    int ii = 0;
    char shellBuffer[1024];
    char shellCommand[1024];
    char shellParameter[16];
    char shellContent[1024];
    struct nsh_var nshvar_number[10];
    
    char *prompt = "nsh"; // Vordefiniert
    
    while(!exit){
	if ( nsh_pid == 1 )
	    {
	    printf("%s$", prompt);
	    }
	else
	    {
    	    printf("%s[%d]%%", prompt, nsh_pid);
	    }
	i=0;
	while (i<1024) {
	    shellCommand[i]  = '\0';
	    shellBuffer[i] = '\0';
	    shellContent[i] = '\0';
	    i++;
	    }
	while (i<16) {
	    shellParameter[i] = '\0';
	    }
        gets(shellBuffer);
        printf("\n\r");
	i = 0;
	while ((shellBuffer[i] != '\0') && (i < 1024))
	    {
	    if (shellBuffer[i] == ' ')
		{
		shellCommand[i] = '\0';
		i++;
		break;
		}
	    else
		{
		shellCommand[i] = shellBuffer[i];
		}
	    i++;
	    }
	ii=0;
	while (shellBuffer[i] != '\0')
	    {
	    if (shellBuffer[i] == ' ')
		{
		shellParameter[ii] = '\0';
		i++;
		break;
		}
	    else
		{
    		shellParameter[ii] = shellBuffer[i];
		i++;
		if ( ii < 16) { ii++; } else { shellParameter[ii] = '\0'; }
		}
	    }
	
	//printf("Befehl = %s.\nParameter = %s.\n", shellCommand,shellParameter);

	DEBUG_print("\nshell: ");
	DEBUG_print(shellCommand);

   	if (strcmp(shellCommand, "help") == false) {
   		   command_help();
   	    }
        else if (strcmp(shellCommand, "bug") == false) {
   	        command_bug();
        }
        else if (strcmp(shellCommand, "dir") == false) {
   	        dir();
        }
   	    else if(strcmp(shellCommand, "exit") == false) {
            exit = 1;
        }
        else if(strcmp(shellCommand, "readfile") == false) {
        //    TFILE *datei;
        //    outputString("\nfopen(): ");
        //    fopen(datei, "smalltext.txt");
        //    outputString("\nfreadln(): ");
        //    outputString(freadln(datei));
        //    outputString("\nfclose():");
        //    fclose(datei);
        outputString(nxFS_readstringfromfilepos("smalltext.txt", 0));
            //outputInteger(isfile("smalltext.txt"));
        }
        else if ((strcmp(shellCommand, "clear") == false) || (strcmp(shellCommand, "clr") == false) || (strcmp(shellCommand, "cls") == false)) {
            cls();
        }
        else if (strcmp(shellCommand, "todo") == false) {
            command_todo();
        }
        else if (strcmp(shellCommand, "nsh") == false) {
            rc = nsh( 10 * nsh_pid + 1 );
        }
        else if ((strcmp(shellCommand, "reboot") == false) ||
                 (strcmp(shellCommand, "restart") == false)) {
            reboot();
        }        
        else if (strcmp(shellCommand, "copyright") == false) {
            command_copyright();
        }
        else if (strcmp(shellCommand, "readtest") == false){
            readtest(0);
        }
        else if (strcmp(shellCommand, "fdcspeed") == false){
            readtest(1);
        }
      //  else if (strcmp(shellCommand, "credits") == false) {
//            command_credits();
//        }
        else if (strcmp(shellCommand, "date") == false) {
            getdate(&currentdate);
            printf("\ncurrent date: %a\n", &currentdate);
        }
        else if (strcmp(shellCommand, "time") == false) {
            gettime(&currenttime);
            printf("\ncurrent time: %t\n", &currenttime);
        }
        else if ( (strcmp(shellCommand, "set") == false) && (strcmp(shellParameter, "date") == false) ) {
            printf("Enter the new date. Begin with day:\n==>");
            char day[3];
            char month[3];
            char year[5];
            gets(day);
            printf("\nNow the month:\n==>");
            gets(month);
            printf("\nNow the year (4 digits!)\n==>");
            gets(year);
            t_date *datestruct;
            datestruct->day = str_to_int(day);
            datestruct->month = str_to_int(month);
            datestruct->year = str_to_int(year);
            setdate(datestruct);
            getdate(&currentdate);
            printf("\n\nDate set: %a\n", &currentdate);
        }
        else if ( (strcmp(shellCommand, "set") == false) && (strcmp(shellParameter, "time") == false) ) {
            printf("Enter the new time. Begin with hour:\n==>");
            char hour[3];
            char minute[3];
            char second[3];
            gets(hour);
            printf("\nNow the minute:\n==>");
            gets(minute);
            printf("\nNow the second:\n==>");
            gets(second);
            t_time *timestruct;
            timestruct->hours = str_to_int(hour);
            timestruct->minutes = str_to_int(minute);
            timestruct->seconds = str_to_int(second);
            settime(timestruct);
            gettime(&currenttime);
            printf("\n\nTime set: %t\n", &currenttime);
        }        
        else if (strcmp(shellCommand, "logo") == false) {
            command_logo();
        }
	  // 	else if (strcmp(shellCommand, "man") == false) {
      //      manual();
      //  }
      //  else if (strcmp(shellCommand, "man install") == false) {
      //      man_install();
      // }
        else if (strcmp(shellCommand, "license") == false){
            command_license();
        }
        else if (strcmp(shellCommand, "") == false) {
            // Wenn einfach Enter gedrückt wurde
        }
   	    else {
		textcolor(COLOR_RED);
		printf("%s : Unknown command.\n\r",shellCommand);
		textcolor(COLOR_LIGHTGRAY); }
   	}
   	// for(;;); /*Foreverschleife ^^*/
	return( exit );
}


// Der Kernel

int main() {

    int nsh_returned = 0;
    int nsh_instance = 1;
    bootprocess();
    // cls();
    // command_logo();
    printf("Welcome to Nova-x!\n");
    printf("\n");
    beep();
    textcolor(COLOR_LIGHTGRAY);
    while ( 1 ) /* Forever */
	{
	nsh_returned = nsh( nsh_instance );
	printf("You left your primary bootshell. (Exit code: %d)\n", nsh_returned);
	printf("A new primary bootshell will startup now.\n");
	}
}
