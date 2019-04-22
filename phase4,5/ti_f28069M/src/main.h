#ifndef MAIN_H
#define MAINH

#include "F2806x_Cla_typedefs.h"	// F2806x CLA Type definitions
#include "F2806x_Device.h"	// F2806x Headerfile Include File
#include "F2806x_Examples.h"	// F2806x Examples Include File
#include "F2806x_EPwm_defines.h"
#include <stdio.h>

//
// Included Files
//
#include <stdbool.h>
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "math.h"

//
// Defines that configure the period for each timer
//
#define BUILD_1				1		// Example project implementation
#define BUILD_2				2		// Project found on forum
#define BUILD_3				3		// Project implementation 1

#define EPWM1_INV_TBPRD		6250  // 60Hz SPWM with 90MHz sysclock, div = 1

#define EPWM2_FORUM_TBPRD	5000 // 5kHz PWM Period register


#define PI					3.14159265358979323846

#define true				1

// Macro to label code as debug code.
// Example: D(prinf("Line Reached!"));
// Remove the DEBUG variable to remove these statements from the code.
#define DEBUG
#ifdef DEBUG
#define D(x) x
#else
#define D(x)
#endif

void DoBootSequence (void);
void HRPWM1_Config (Uint16);
int16 CalculateFastFourierTransform (void);
__interrupt void cpu_timer0_isr (void);

#endif // MAIN_H
