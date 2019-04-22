#include <src/main.h>

//
// Typedef
//
typedef struct
{
    volatile struct EPWM_REGS *EPwmRegHandle;
    Uint16 EPwm_CMPA_Direction;
    Uint16 EPwm_CMPB_Direction;
    Uint16 EPwmTimerIntCount;
    Uint16 EPwmMaxCMPA;
    Uint16 EPwmMinCMPA;
    Uint16 EPwmMaxCMPB;
    Uint16 EPwmMinCMPB;
} EPWM_INFO;

/
// Function Prototypes
//
void InitEPwm2Example(void);
__interrupt void epwm2_isr(void);
void update_inv_compare(EPWM_INFO *epwm_info);

//
// Globals
//
#define STANDARD_WAVE 0;
#define SIN_WAVE      1;

EPWM_INFO epwm2_info;


//
// Defines that configure the period for each timer
//
#define EPWM2_TIMER_TBPRD  6250  // Period register
#define EPWM2_MAX_CMPA     1950
#define EPWM2_MIN_CMPA       50
#define EPWM2_MAX_CMPB     1950
#define EPWM2_MIN_CMPB       50

//
// Defines to keep track of which way the compare value is moving
//
#define EPWM_CMP_UP   1
#define EPWM_CMP_DOWN 0

// *************** End example related globals *********************

Uint16 i = 0, m = 0;
float32 a = 0;

// local variables for forum project
//
Uint32 b[101]={0};

// local variables for Proj 1
//
int currentBuild = BUILD_1, waveForm = 1;
Uint16 sinTable[120] = {0};


void
DoBootSequence ()
{
  // Adjust console buffering to flush when newline is output.
  D (setvbuf (stdout, NULL, _IOLBF, 0));

  //
  // Initialize System Control:
  // PLL, WatchDog, enable Peripheral Clocks
  // This example function is found in the F2806x_SysCtrl.c file.
  //
  InitSysCtrl ();

  //
  // Step 2. Initalize GPIO:
  // Enable XCLOCKOUT to allow monitoring of oscillator 1
  //
  EALLOW;
  GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;	// enable XCLOCKOUT through GPIO mux
  SysCtrlRegs.XCLK.bit.XCLKOUTDIV = 2;	// XCLOCKOUT = SYSCLK
  EDIS;

  // Clear all interrupts and initialize PIE vector table:
  // Disable CPU interrupts
  //

  DINT;
  //
  // Initialize the PIE control registers to their default state.
  // The default state is all PIE interrupts disabled and flags
  // are cleared.
  // This function is found in the F2806x_PieCtrl.c file.
  //
  InitPieCtrl ();
  //
  // Disable CPU interrupts and clear all CPU interrupt flags
  //
  IER = 0x0000;
  IFR = 0x0000;
  //
  // Initialize the PIE vector table with pointers to the shell Interrupt
  // Service Routines (ISR).
  // This will populate the entire table, even if the interrupt
  // is not used in this example.  This is useful for debug purposes.
  // The shell ISR routines are found in F2806x_DefaultIsr.c.
  // This function is found in F2806x_PieVect.c.
  //
  InitPieVectTable ();

}

//
// HRPWM1_Config -
//
void
HRPWM1_Config (Uint16 period)
{
  D (printf ("Configuring HRPwm1\n"));
  //
  // ePWM1 register configuration with HRPWM
  // ePWM1A toggle low/high with MEP control on Rising edge
  //
  EPwm1Regs.TBCTL.bit.PRDLD = TB_IMMEDIATE;	// set Immediate load
  EPwm1Regs.TBPRD = period - 1;	// PWM frequency = 1 / period
  EPwm1Regs.CMPA.half.CMPA = period / 2;	// set duty 50% initially
  EPwm1Regs.CMPA.half.CMPAHR = (1 << 8);	// initialize HRPWM extension
  EPwm1Regs.CMPB = period / 2;	// set duty 50% initially
  EPwm1Regs.TBPHS.all = 0;
  EPwm1Regs.TBCTR = 0;

  EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;
  EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;	// EPwm1 is the Master
  EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
  EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
  EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;

  EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
  EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
  EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
  EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

  EPwm1Regs.AQCTLA.bit.ZRO = AQ_CLEAR;	// PWM toggle low/high
  EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;
  EPwm1Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
  EPwm1Regs.AQCTLB.bit.CBU = AQ_SET;

  EALLOW;
  EPwm1Regs.HRCNFG.all = 0x0;
  EPwm1Regs.HRCNFG.bit.EDGMODE = HR_REP;	// MEP control on Rising edge
  EPwm1Regs.HRCNFG.bit.CTLMODE = HR_CMP;
  EPwm1Regs.HRCNFG.bit.HRLOAD = HR_CTR_ZERO;
  EDIS;
  D (printf ("Configured EPwm1\n"));
}

// This calculates Fast Fourier Transform
int16
CalculateFastFourierTransform (void)
{
  fft_flag = FFT_FLAG;

  scale_flag = SCALE_FLAG;

  data = <1024 - point Complex input >;

  // Bit-Reverse 1024-point data, Store into data_br, data_br aligned to
  // 12-least significant binary zeros

  hwafft_br (data, data_br, DATA_LEN_1024);

  // Bit-reverse input data, destination buffer aligned

  data = data_br;

  // Compute 1024-point FFT with scaling enabled

  out_sel = hwafft_1024pts (data, scratch, fft_flag, scale_flag);

  if (out_sel == OUT_SEL_DATA)
    {

      result = data;

    }
  else
    {

      result = scratch;
    }

  return result;
}

void
main (void)
{

  // If running from flash copy RAM only functions to RAM (I don't think this is setup correctly with the .cmd file)
 //
 #ifdef _FLASH
     memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
 #endif

  switch(currentBuild) {
  case BUILD_1:
    {
        // PLL, WatchDog, enable Peripheral Clocks
        //
        InitSysCtrl();

        // For this case just init GPIO pins for ePWM1, ePWM2, ePWM3
        // These functions are in the F2806x_EPwm.c file
        //
        InitEPwm2Gpio();

        // *** Clear all interrupts and initialize PIE vector table ***

        // Disable CPU interrupts
        //
        DINT;

        // Initialize the PIE control registers to their default state.
        //
        InitPieCtrl();

        // Disable CPU interrupts and clear all CPU interrupt flags
        //
        IER = 0x0000;
        IFR = 0x0000;

        // Initialize the PIE vector table with pointers to the shell Interrupt
        // Service Routines (ISR).
        //
        InitPieVectTable();

        // Map EPWM1 interrupt to local ISR function.
        //
        // NOTE:
        // EALLOW call needed to write to EALLOW protected registers
        // EDIS call needed to disable write to EALLOW protected registers
        EALLOW;
        PieVectTable.EPWM2_INT = &epwm2_isr;
        EDIS;

        // *** Initialize all the Device Peripherals **

        // InitPeripherals();  // Not required for this example

        // For now, only initialize the ePWM
        //
        EALLOW;
        SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
        EDIS;

        InitEPwm2Example();

        EALLOW;
        SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
        EDIS;

        // *** User specific code ***

        // Generate lookup table. Create 120 samples for 200Hz period sin wave.
        //
      for(m = 0; m < 120; ++m) {
        a = sin(PI * (1/200.0) * m);
        sinTable[m] = 3125 * a + 3125;
      }

        // Enable CPU INT3 which is connected to EPWM1 INT
        //
        IER |= M_INT3;

        // Enable EPWM INTn in the PIE: Group 3 interrupt 1
        //
        PieCtrlRegs.PIEIER3.bit.INTx2 = 1;

        // Enable global Interrupts and higher priority real-time debug events
        //
        EINT;   // Enable Global interrupt INTM
        ERTM;   // Enable Global realtime interrupt DBGM
        
        // IDLE loop. Just sit and loop forever
        //
        for(;;)
        {
            __asm(" NOP");
        }
    }
  case BUILD_2:
    {
      // Used for testing and implemenation
      
        // IDLE loop. Just sit and loop forever
        //
        for(;;)
        {
            __asm(" NOP");
        }
    }
  case BUILD_3:
    {
      // Used for testing and implemenation
      
        // IDLE loop. Just sit and loop forever
        //
        for(;;)
        {
            __asm(" NOP");
        }
    }
  }

  // DoBootSequence ();
  // //
  // // Interrupts that are used in this example are re-mapped to
  // // ISR functions found within this file.
  // //
  // EALLOW;			// This is needed to write to EALLOW protected registers
  // PieVectTable.TINT0 = &cpu_timer0_isr;
  // EDIS;				// This is needed to disable write to EALLOW protected registers

  // InitCpuTimers ();

  // //
  // // Configure CPU-Timer 0 to interrupt every 500 milliseconds:
  // // 80MHz CPU Freq, 50 millisecond Period (in uSeconds)
  // //
  // ConfigCpuTimer (&CpuTimer0, 80, 3 * 100000);

  // //
  // // To ensure precise timing, use write-only instructions to write to the
  // // entire register. Therefore, if any of the configuration bits are changed
  // // in ConfigCpuTimer and InitCpuTimers (in F2806x_CpuTimers.h), the
  // // below settings must also be updated.
  // //

  // //
  // // Use write-only instruction to set TSS bit = 0
  // //
  // CpuTimer0Regs.TCR.all = 0x4001;

  // //
  // // Step 5. User specific code, enable interrupts:
  // //

  // // Configure GPIO34 and GPIO39 to blink LEDs
  // // Configure GPIO for EPwm
  // EALLOW;
  // GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
  // GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1;
  // GpioDataRegs.GPBDAT.bit.GPIO34 = 1;
  // GpioDataRegs.GPBDAT.bit.GPIO39 = 1;
  // //
  // // Disable internal pull-up for the selected output pins for reduced power
  // // consumption. Pull-ups can be enabled or disabled by the user.
  // // Comment out other unwanted lines.
  // //
  // GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;	// Disable pull-up on GPIO0 (EPWM1A)
  // GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;	// Disable pull-up on GPIO1 (EPWM1B)

  // //
  // // Configure EPWM-1 pins using GPIO regs
  // // This specifies which of the possible GPIO pins will be EPWM1 functional
  // // pins.
  // // Comment out other unwanted lines.
  // //
  // GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;	// Configure GPIO0 as EPWM1A
  // GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;	// Configure GPIO1 as EPWM1B
  // EDIS;

  // //
  // // Enable CPU INT1 which is connected to CPU-Timer 0
  // //
  // IER |= M_INT1;

  // //
  // // Enable TINT0 in the PIE: Group 1 interrupt 7
  // //
  // PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

  // //
  // // Enable global Interrupts and higher priority real-time debug events
  // //
  // EINT;				// Enable Global interrupt INTM
  // ERTM;				// Enable Global realtime interrupt DBGM

  // //
  // // Step 6. IDLE loop. Just sit and loop forever (optional)
  // //
  // for (;;);
}

__interrupt void
cpu_timer0_isr (void)
{
  CpuTimer0.InterruptCount++;

  // Blink LED just to verify that the interupts are occuring.
  EALLOW;
  GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
  GpioDataRegs.GPBTOGGLE.bit.GPIO39 = 1;
  EDIS;

  D (printf ("Timer elapsed!\n"));
  // Acknowledge this interrupt to receive more interrupts from group 1
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}

//
// epwm2_isr - 
//
__interrupt void
epwm2_isr(void)
{
    //
    // Update the CMPA and CMPB values
    //
  update_inv_compare(&epwm2_info);

    //
    // Clear INT flag for this timer
    //
    EPwm2Regs.ETCLR.bit.INT = 1;

    //
    // Acknowledge this interrupt to receive more interrupts from group 3
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

//
// InitEPwm2Example - 
//
void
InitEPwm2Example()
{
    //
    // Setup TBCLK
    //
    EPwm2Regs.TBPRD = EPWM2_TIMER_TBPRD;         // Set timer period 801 TBCLKs
    EPwm2Regs.TBPHS.half.TBPHS = 0x0000;         // Phase is 0
    EPwm2Regs.TBCTR = 0x0000;                    // Clear counter

    //
    // Set Compare values
    //
    EPwm2Regs.CMPA.half.CMPA = 3000;//EPWM2_MIN_CMPA;      // Set compare A value
    EPwm2Regs.CMPB = 3000;//EPWM2_MIN_CMPB;             // Set Compare B value

    //
    // Setup counter mode
    //
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
    EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    //
    // Setup shadowing
    //
    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_IMMEDIATE;
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_IMMEDIATE;

    //
    // Set actions
    //
    EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;     // Set PWM2A on event A, up count
    EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;   // Clear PWM2A on event B, down count

    EPwm2Regs.AQCTLB.bit.CAU = AQ_CLEAR;    // Clear PWM2B on event B, up count
    EPwm2Regs.AQCTLB.bit.CAD = AQ_SET;    // Set PWM2B on event A, down count

    //
    // Interrupt where we will change the Compare Values
    //
    EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm2Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm2Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 3rd event

    // Active Low PWMs - Setup Deadband
    //
    EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm2Regs.DBCTL.bit.POLSEL = 2;
    EPwm2Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm2Regs.DBRED = 185;
    EPwm2Regs.DBFED = 185;

    //
    // Information this example uses to keep track of the direction the 
    // CMPA/CMPB values are moving, the min and max allowed values and
    // a pointer to the correct ePWM registers
    //
    
    //
    // Start by increasing CMPA & increasing CMPB 
    //
    epwm2_info.EPwm_CMPA_Direction = EPWM_CMP_UP;
    epwm2_info.EPwm_CMPB_Direction = EPWM_CMP_UP;
    
    epwm2_info.EPwmTimerIntCount = 0;         // Zero the interrupt counter
    
    //
    // Set the pointer to the ePWM module
    //
    epwm2_info.EPwmRegHandle = &EPwm2Regs;
    
    //
    // Setup min/max CMPA/CMPB values
    //
    epwm2_info.EPwmMaxCMPA = 6245;  // Just shy of 6250
    epwm2_info.EPwmMinCMPA = 5;
    epwm2_info.EPwmMaxCMPB = 6245;
    epwm2_info.EPwmMinCMPB = 5;
}

// update_inv_compare -
//
void update_inv_compare(EPWM_INFO *epwm_info)
{

  switch(waveForm) {
    case 0:
      //
      // If we were increasing CMPA, check to see if we reached the max value
      // If not, increase CMPA else, change directions and decrease CMPA
      //
      if(epwm_info->EPwm_CMPA_Direction == EPWM_CMP_UP)
      {
        if(epwm_info->EPwmRegHandle->CMPA.half.CMPA <
           epwm_info->EPwmMaxCMPA)
        {
          epwm_info->EPwmRegHandle->CMPA.half.CMPA++;
        }
        else
        {
          epwm_info->EPwm_CMPA_Direction = EPWM_CMP_DOWN;
          epwm_info->EPwmRegHandle->CMPA.half.CMPA--;
        }
      }

      //
      // If we were decreasing CMPA, check to see if we reached the min value
      // If not, decrease CMPA else, change directions and increase CMPA
      //
      else
      {
        if(epwm_info->EPwmRegHandle->CMPA.half.CMPA ==
           epwm_info->EPwmMinCMPA)
        {
          epwm_info->EPwm_CMPA_Direction = EPWM_CMP_UP;
          epwm_info->EPwmRegHandle->CMPA.half.CMPA++;
        }
        else
        {
          epwm_info->EPwmRegHandle->CMPA.half.CMPA--;
        }
      }



      //
      // If we were increasing CMPB, check to see if we reached the max value
      // If not, increase CMPB else, change directions and decrease CMPB
      //
      if(epwm_info->EPwm_CMPB_Direction == EPWM_CMP_UP)
      {
        if(epwm_info->EPwmRegHandle->CMPB < epwm_info->EPwmMaxCMPB)
        {
          epwm_info->EPwmRegHandle->CMPB++;
        }
        else
        {
          epwm_info->EPwm_CMPB_Direction = EPWM_CMP_DOWN;
          epwm_info->EPwmRegHandle->CMPB--;
        }
      }

      //
      // If we were decreasing CMPB, check to see if we reached the min value
      // If not, decrease CMPB else, change directions and increase CMPB
      //
      else
      {
        if(epwm_info->EPwmRegHandle->CMPB == epwm_info->EPwmMinCMPB)
        {
          epwm_info->EPwm_CMPB_Direction = EPWM_CMP_UP;
          epwm_info->EPwmRegHandle->CMPB++;
        }
        else
        {
          epwm_info->EPwmRegHandle->CMPB--;
        }
      }

      break;
    case 1:
      EALLOW;
      epwm_info->EPwmRegHandle->CMPA.half.CMPA = sinTable[i];     // Update COMPA
      EDIS;

      ++i;
      if (i == 120) {
        i = 0;
      }

      break;

    case 2:
      // Do nothing exciting, just reset COMP A
      // You MUST reset during interrupt or COMP A return to a default.
      epwm_info->EPwmRegHandle->CMPA.half.CMPA = 3000;
      break;
  }

    return;
}

//
// End of File
//