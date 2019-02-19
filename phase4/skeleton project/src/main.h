#ifndef MAIN_H
#define MAINH

#include "F2806x_Cla_typedefs.h"// F2806x CLA Type definitions
#include "F2806x_Device.h"      // F2806x Headerfile Include File
#include "F2806x_Examples.h"   	// F2806x Examples Include File
#include <stdio.h>

void DoBootSequence(void);
void ConfigureADC(void);
__interrupt void cpu_timer0_isr(void);

#endif  // MAIN_H
