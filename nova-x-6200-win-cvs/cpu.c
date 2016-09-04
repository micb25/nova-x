/* nova-x cpu detection unit */

/* typedefs */
typedef unsigned long ulong;

// Headerfile
#include "cpu.h"

/* commands */
#define CPU_CMD_VENDOR 			0		// Vendor Command
#define CPU_CMD_PROPERTIES 		1		// Family, Model, Stepping, Feature Flags Command
#define CPU_CMD_CONFIG			2		// Processor Configuration
#define CPU_CMD_SERIAL			3		// CPU Serial Number

/* Intel Produkt Namen nach Family */
char* IntelProduktFamily4[10] =
{ "486 DX-25/33", "486 DX-50", "486 SX", "486 DX/2", "486 SL", "486 SX/2", "unknown", "486 DX/2-WB", "486 DX/4", "486 DX/4-WB" };
char *IntelProduktFamily5[10] =
{ "Pentium 60/66 A-step", "Pentium 60/66", "Pentium 75 - 200", "OverDrive PODP5V83", "Pentium MMX", "unknown" ,"unknown" ,"Mobile Pentium 75 - 200" ,"Mobile Pentium MMX", "unknown" };
char *IntelProduktFamily6[10] =
{ "Pentium Pro A-step","Pentium Pro","unknown","Pentium II (Klamath)","unknown","Pentium II","Mobile Pentium II","Pentium III","Pentium III","unknown" };
char *IntelProduktFamily15[10] =
{ "Pentium 4","Pentium 4","Pentium 4","Pentium 4","Pentium 4","Pentium 4","Pentium 4","Pentium 4","Pentium 4","Pentium 4" };

/* AMD Produkt Namen nach Family */
char* AMDProduktFamily4[16] =
{ "unknown", "unknown", "unknown", "486 DX/2", "unknown", "unknown", "unknown", "486 DX/2-WB", "486 DX/4", "486 DX/4-WB", "unknown", "unknown", "unknown", "unknown", "Am5x86-WT", "Am5x86-WB" };
char* AMDProduktFamily5[15] =
{ "K5/SSA5", "K5", "K5", "K5", "unknown", "unknown", "K6", "K6", "K6-2", "K6-3", "unknown", "unknown", "unknown", "K6-2+ / K6-III+"};
char* AMDProduktFamily6[8] =
{ "Athlon", "Athlon", "Athlon", "Duron", "Athlon (Thunderbird)", "unknown", "Athlon (Palamino)", "Duron (Morgan)"};

/* Intel Feature Flags */
char *Intel_feature_flags[] = {
  "FPU    Floating Point Unit",
  "VME    Virtual 8086 Mode Enhancements",
  "DE     Debugging Extensions",
  "PSE    Page Size Extensions",
  "TSC    Time Stamp Counter",
  "MSR    Model Specific Registers",
  "PAE    Physical Address Extension",
  "MCE    Machine Check Exception",
  "CX8    COMPXCHG8B Instruction",
  "APIC   On-chip Advanced Programmable Interrupt Controller present and enabled",
  "10     Reserved",
  "SEP    Fast System Call",
  "MTRR   Memory Type Range Registers",
  "PGE    PTE Global Flag",
  "MCA    Machine Check Architecture",
  "CMOV   Conditional Move and Compare Instructions",
  "FGPAT  Page Attribute Table",
  "PSE-36 36-bit Page Size Extension",
  "PN     Processor Serial Number present and enabled",
  "CLFSH  CFLUSH instruction",
  "20     reserved",
  "DS     Debug store",
  "ACPI   Thermal Monitor and Clock Ctrl",
  "MMX    MMX instruction set",
  "FXSR   Fast FP/MMX Streaming SIMD Extensions save/restore",
  "SSE    Streaming SIMD Extensions instruction set",
  "SSE2   SSE2 extensions",
  "SS     Self Snoop",
  "HT     Hyper Threading",
  "TM     Thermal monitor",
  "30     reserved",
  "31     reserved",
};

/* AMD Feature Flags */
char *AMD_feature_flags[] = {
  "Floating Point Unit",
  "Virtual Mode Extensions",
  "Debugging Extensions",
  "Page Size Extensions",
  "Time Stamp Counter (with RDTSC and CR4 disable bit)",
  "Model Specific Registers with RDMSR & WRMSR",
  "PAE - Page Address Extensions",
  "Machine Check Exception",
  "COMPXCHG8B Instruction",
  "APIC",
  "10 - Reserved",
  "SYSCALL/SYSRET or SYSENTER/SYSEXIT instructions",
  "MTRR - Memory Type Range Registers",
  "Global paging extension",
  "Machine Check Architecture",
  "Conditional Move Instruction",
  "PAT - Page Attribute Table",
  "PSE-36 - Page Size Extensions",
  "18 - reserved",
  "19 - reserved",
  "20 - reserved",
  "21 - reserved",
  "AMD MMX Instruction Extensions",
  "MMX instructions",
  "FXSAVE/FXRSTOR",
  "25 - reserved",
  "26 - reserved",
  "27 - reserved",
  "28 - reserved",
  "29 - reserved",
  "3DNow! Instruction Extensions",
  "3DNow instructions",
};

/* Struktur mit den aktuellen registern */
struct {
	ulong eax;
	ulong ebx;
	ulong ecx;
	ulong edx;
} regs;

/* füllt die cpu regs je nach command */
void cpuidfill(ulong COMMAND) {
    ulong REGEAX, REGEBX, REGECX, REGEDX;

    __asm__ __volatile("cpuid"
      : "=a" (REGEAX),
        "=b" (REGEBX),
        "=c" (REGECX),
        "=d" (REGEDX)
      : "a"  (COMMAND));

    regs.eax = REGEAX;
    regs.ebx = REGEBX;
    regs.ecx = REGECX;
    regs.edx = REGEDX;
}

void cpuPrintRegs() {
	printf("\n");
	printf("eax : %x / %c \n", regs.eax, (char)regs.eax);
	printf("eax : %x / %c \n", regs.ebx, (char)regs.ebx);
	printf("eax : %x / %c \n", regs.ecx, (char)regs.ecx);
	printf("eax : %x / %c \n", regs.edx, (char)regs.edx);
}

void cpuPrintInfos() {
    int tsc;
    int speed;
        
	printf("vendor:   %s    \n", cpuVendor());
	printf("family:   %d-%x \n", cpuFamily(), cpuFamily());
	printf("model:    %d-%x \n", cpuModel(), cpuModel());
	printf("stepping: %d-%x \n", cpuStepping(), cpuStepping());
	printf("name:     %s    \n", cpuName());
	printf("type:     %s    \n", cpuType());
    speed = cpuSpeed();
	printf("speed:    %d MHZ (measured) ", speed);
    if (speed == 0) {
        tsc = checkTSC();
        if (tsc == 0)
            printf("No TSC, calculation cannot be performed");
    }
    printf("\n");
}

void cpubootInfos() {
	printf("%s CPU found (family %d / model %d / speed %d MHZ) \n\n",
	cpuName(), cpuFamily(), cpuModel(), cpuSpeed() );
}

int cpuSpeed() {
	unsigned long int a, b, total;
	int t, t1;

	/* Read Time Stamp Counter aufrufen um sich die Anzahl der startticks zu holen */
	asm volatile("rdtsc":"=A" (a));

	/* Eine Sekunde warten, um den Messwert erneut auslesen zu können. */
	/* Dann kann man messen, wievel Cycles der CPU in einer Sekunde berechnet hat */
	t1 = gettickscount();
	while (true) {
		t = gettickscount();
		if ((t - 100) > t1)
            /* Sekunde vorbei, Schleife verlassen */
			break;
	}

	/* Read Time Stamp Counter aufrufen um sich die Anzahl der endticks zu holen */
	asm volatile("rdtsc":"=A" (b));

	/* Totalticks ermitteln */
	total = b - a;

	return (total / 1000000);
}

/* CPU Family */
int cpuFamily() {
	int family;

	cpuidfill(CPU_CMD_PROPERTIES);
	family = (regs.eax >> 8) & 0xF;
	return (family);
}

/* CPU Model */
int cpuModel() {
	int model;

	cpuidfill(CPU_CMD_PROPERTIES);
	model = (regs.eax >> 4) & 0xF;
	return (model);
}

/* CPU Stepping */
int cpuStepping() {
	int stepping;

	cpuidfill(CPU_CMD_PROPERTIES);
	stepping = regs.eax & 0xF;
	return (stepping);
}

/* CPU Vendor */
char* cpuVendor() {
	cpuidfill( CPU_CMD_VENDOR );

	if (((char)regs.ebx == 'G') &&
	    ((char)regs.edx == 'i') &&
	    ((char)regs.ecx == 'n'))
	{
		return "GenuineIntel";
    }
    if (((char)regs.ebx == 'A') &&
        ((char)regs.edx == 'e') &&
        ((char)regs.ecx == 'c'))
    {
        return "AuthenticAMD";
    }
    if (((char)regs.ebx == 'C') &&
        ((char)regs.edx == 'x') &&
        ((char)regs.ecx == 't'))
    {
        return "CyrixInstead";
    }

    return "Unknown     ";
}

/* CPU type */
char *cpuType() {
	cpuidfill(CPU_CMD_PROPERTIES);

	switch (((regs.eax >> 12 ) &0x7 )) {
		case 0: {
			return "Original";
		}
	    break;
	    case 1: {
	    	return "OverDrive";
	 	}
	    break;
	    case 2: {
	    	return "Dual";
		}
		break;
		case 3: {
			return "reserved";
		}
		default: {
			return "unknown";
		}
	}
}

int isIntel() {
	cpuidfill(CPU_CMD_VENDOR);

	if (((char)regs.ebx == 'G') &&
	    ((char)regs.edx == 'i') &&
	    ((char)regs.ecx == 'n'))
		return 1;
    else
		return 0;
}

int isAMD() {
	cpuidfill(CPU_CMD_VENDOR);

	if (((char)regs.ebx == 'A') &&
	    ((char)regs.edx == 'e') &&
	    ((char)regs.ecx == 'c'))
		return 1;
    else
		return 0;
}

int isCyrix() {
	cpuidfill(CPU_CMD_VENDOR);

	if (((char)regs.ebx == 'C') &&
	    ((char)regs.edx == 'x') &&
	    ((char)regs.ecx == 't'))
		return 1;
    else
		return 0;
}

char *cpuName() {
	int family;
	int model;
	int stepping;

	family = cpuFamily();
	model = cpuModel();
	stepping = cpuStepping();

	if (isIntel() == 1)	{
		switch (family) {
			case 4:	{
				return (IntelProduktFamily4[model]);
			}
			break;
			case 5:	{
				return (IntelProduktFamily5[model]);
			}
			break;
			case 6: {
				return (IntelProduktFamily6[model]);
			}
			break;
			case 15: {
				return (IntelProduktFamily15[model]);
			}
			break;
		}
	}

	if (isAMD() == 1) {
		switch (family) {
			case 4:	{
				return (AMDProduktFamily4[model]);
			}
			break;
			case 5:	{
				return (AMDProduktFamily5[model]);
			}
			break;
			case 6:	{
				return (AMDProduktFamily6[model]);
			}
			break;
		}
	}

	/* hier sollte er nicht hinkommen */
	return "unknown";
}

void cpuPrintFeatureFlags() {
	int feature;
	int i;

	cpuidfill (CPU_CMD_PROPERTIES);
	feature = regs.edx;

	/* Intel Feature Flags */
	if (isIntel() == 1) {
		for (i = 0; i < 32; i++) {
    		if (feature & (1 << i))	{
				printf("%s\n", Intel_feature_flags[i]);
    	  	}
  		}
	}
	/* AMD Feature Flags */
	if (isAMD() == 1) {
		for (i = 0; i < 32; i++) {
			if (cpuFamily() == 5 && cpuModel() == 0 && i == 9) {
				printf("Global Paging Extensions\n");
		    } else {
				if (feature & (1 << i)) {
					printf("%s\n", AMD_feature_flags[i]);
				}
			}
		}
	}
}

int checkTSC() {
    int ok;
	int feature;

	cpuidfill (CPU_CMD_PROPERTIES);
	feature = regs.edx;
 
    if (feature & (1 << 4)) {
        /* Time Stamp Counter vorhanden */
        ok = 1;
  	} else {
        /* Time Stamp Counter nicht vorhanden */
        ok = 0;
    }
    return ok;
}
