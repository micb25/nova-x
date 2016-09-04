/*

    Nova-X Timer

    version 0.01a by MB
    
*/

#ifndef TIMER_C_
#define TIMER_C_

#include "timer.h"


void timerirq()
{

}

void TIMER_enable(TTIMER *ttimer)
{
    ttimer->enabled = true;
}

void TIMER_disable(TTIMER *ttimer)
{
    ttimer->enabled = false;
}

void TIMER_setinterval(TTIMER *ttimer, unsigned int interv)
{
    ttimer->interval = interv;
}

unsigned int TIMER_getinterval(TTIMER *ttimer)
{
    return ttimer->interval;
}

unsigned char TIMER_isenabled(TTIMER *ttimer)
{
    return ttimer->enabled;
}

int TIMER_wasexec(TTIMER *ttimer)
{
    
}

#endif
