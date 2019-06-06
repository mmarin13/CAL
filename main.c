#include "general.h"
#include "hal_motor.h"
#include "sys_tasks.h"
#include "sys_schedule.h"

// FICD
#pragma config ICS = PGD2               // Comm Channel Select (Communicate on PGC2/EMUC2 and PGD2/EMUD2)
#pragma config JTAGEN = ON              // JTAG Port Enable (JTAG is Enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

int main()
{
	TASK_Inits();
	TASK_vSchedule();
    while(1)
    {
		
	}	
    return 0;
}