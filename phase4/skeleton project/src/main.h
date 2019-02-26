#ifndef MAIN_H
#define MAINH

#include "F2806x_Cla_typedefs.h"// F2806x CLA Type definitions
#include "F2806x_Device.h"      // F2806x Headerfile Include File
#include "F2806x_Examples.h"   	// F2806x Examples Include File
#include <stdio.h>


// Macro to label code as debug code.
// Example: D(prinf("Line Reached!"));
// Remove the DEBUG variable to remove these statements from the code.
#define DEBUG
#ifdef DEBUG
#define D(x) x
#else
#define D(x)
#endif

void DoBootSequence(void);
int16 SampleTemperature(void);
__interrupt void cpu_timer0_isr(void);

#endif  // MAIN_H
