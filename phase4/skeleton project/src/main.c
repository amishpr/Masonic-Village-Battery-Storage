#include <src/main.h>

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
  DoBootSequence ();
  //
  // Interrupts that are used in this example are re-mapped to
  // ISR functions found within this file.
  //
  EALLOW;			// This is needed to write to EALLOW protected registers
  PieVectTable.TINT0 = &cpu_timer0_isr;
  EDIS;				// This is needed to disable write to EALLOW protected registers

  InitCpuTimers ();

  //
  // Configure CPU-Timer 0 to interrupt every 500 milliseconds:
  // 80MHz CPU Freq, 50 millisecond Period (in uSeconds)
  //
  ConfigCpuTimer (&CpuTimer0, 80, 3 * 100000);

  //
  // To ensure precise timing, use write-only instructions to write to the
  // entire register. Therefore, if any of the configuration bits are changed
  // in ConfigCpuTimer and InitCpuTimers (in F2806x_CpuTimers.h), the
  // below settings must also be updated.
  //

  //
  // Use write-only instruction to set TSS bit = 0
  //
  CpuTimer0Regs.TCR.all = 0x4001;

  //
  // Step 5. User specific code, enable interrupts:
  //

  // Configure GPIO34 and GPIO39 to blink LEDs
  // Configure GPIO for EPwm
  EALLOW;
  GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
  GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1;
  GpioDataRegs.GPBDAT.bit.GPIO34 = 1;
  GpioDataRegs.GPBDAT.bit.GPIO39 = 1;
  //
  // Disable internal pull-up for the selected output pins for reduced power
  // consumption. Pull-ups can be enabled or disabled by the user.
  // Comment out other unwanted lines.
  //
  GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;	// Disable pull-up on GPIO0 (EPWM1A)
  GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;	// Disable pull-up on GPIO1 (EPWM1B)

  //
  // Configure EPWM-1 pins using GPIO regs
  // This specifies which of the possible GPIO pins will be EPWM1 functional
  // pins.
  // Comment out other unwanted lines.
  //
  GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;	// Configure GPIO0 as EPWM1A
  GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;	// Configure GPIO1 as EPWM1B
  EDIS;

  //
  // Enable CPU INT1 which is connected to CPU-Timer 0
  //
  IER |= M_INT1;

  //
  // Enable TINT0 in the PIE: Group 1 interrupt 7
  //
  PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

  //
  // Enable global Interrupts and higher priority real-time debug events
  //
  EINT;				// Enable Global interrupt INTM
  ERTM;				// Enable Global realtime interrupt DBGM

  //
  // Step 6. IDLE loop. Just sit and loop forever (optional)
  //
  for (;;);
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
// End of File
//
