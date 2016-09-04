/*

    Speaker Treiber für Nova-X
    
    version 0.01a: by MB
            
*/

#ifndef SPEAKER_C_
#define SPEAKER_C_

#include "speaker.h"

void sound(short freq)
{
  short freqdiv;
  unsigned char tmp;
  freqdiv = 1193180 / freq;
  nxoutport(0x43, 0xB6);
  nxoutport(0x42, (unsigned char)freqdiv);
  nxoutport(0x42, (unsigned char)(freqdiv >> 8));
  tmp = nxinport(0x61);
  if (tmp != (tmp | 3)) nxoutport(0x61, tmp | 3);
}

void nosound()
{
  unsigned char tmp;
  tmp = nxinport(0x61);
  tmp &= 0xFC;
  nxoutport(0x61, tmp);
}

void beep()
{
  sound(1000);
  delay(25); /* aufpassen 25 entspricht 250ms!!! */
  nosound();
}

#endif
