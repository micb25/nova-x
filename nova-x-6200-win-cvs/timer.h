/*

    Nova-X Timer

    version 0.01a by MB
    
*/

#ifndef TIMER_H_
#define TIMER_H_

typedef struct {
    unsigned char enabled;
    unsigned int interval;
    unsigned int wastime;
} TTIMER;

unsigned int timerscount = 0;
void timerirq();
void TIMER_enable(TTIMER *ttimer);
void TIMER_disable(TTIMER *ttimer);
void TIMER_setinterval(TTIMER *ttimer, unsigned int interv);
unsigned int TIMER_getinterval(TTIMER *ttimer);
unsigned char TIMER_isenabled(TTIMER *ttimer);
int TIMER_wasexec(TTIMER *ttimer);

#endif
