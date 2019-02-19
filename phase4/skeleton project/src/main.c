#include <src/main.h>

void
DoBootSequence()
{
    //
    // Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks
    // This example function is found in the F2806x_SysCtrl.c file.
    //
    InitSysCtrl();
    //
    // Initalize GPIO:
    // This example function is found in the F2806x_Gpio.c file and
    // illustrates how to set the GPIO to it's default state.
    //
    // InitGpio();  // Skipped for this example

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
    InitPieCtrl();
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
    InitPieVectTable();

}

void
ConfigureADC(void)
{
    InitAdc();
    AdcOffsetSelfCal();

    // Configure ADC to sample the temperature sensor on ADCIN5:
    // The output of Piccolo temperature sensor can be internally connected to
    // the ADC through ADCINA5 via the TEMPCONV bit in the ADCCTL1 register.
    // When this bit is set, any voltage applied to the external ADCIN5 pin
    // is ignored.
    EALLOW;

    // Connect internal temp sensor to channel ADCINA5.
    AdcRegs.ADCCTL1.bit.TEMPCONV    = 1;

    // ADCIN5: Since the temperature sensor is connected to ADCIN5, configure
    // the ADC to sample channel ADCIN5 as well as the ADC SOC trigger and
    // ADCINTs preferred. This example uses EPWM1A to trigger the ADC
    // to start a conversion and trips ADCINT1 at the end of the conversion.

    AdcRegs.ADCCTL2.bit.ADCNONOVERLAP = 1;  // Enable non-overlap mode

    // ADCINT1 trips after AdcResults latch
    AdcRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    AdcRegs.INTSEL1N2.bit.INT1E     = 1;    // Enabled ADCINT1
    AdcRegs.INTSEL1N2.bit.INT1CONT  = 0;    // Disable ADCINT1 Continuous mode

    //
    // setup EOC0 to trigger ADCINT1 to fire
    //
    AdcRegs.INTSEL1N2.bit.INT1SEL   = 0;

    //
    // set SOC0 channel select to ADCINA5
    // (which is internally connected to the temperature sensor)
    //
    AdcRegs.ADCSOC0CTL.bit.CHSEL    = 5;

    AdcRegs.ADCSOC0CTL.bit.TRIGSEL  = 5;    // set SOC0 start trigger on EPWM1A

    //
    // set SOC0 S/H Window to 26 ADC Clock Cycles, (25 ACQPS plus 1)
    //
    AdcRegs.ADCSOC0CTL.bit.ACQPS    = 25;
    EDIS;

    //
    // Step 7. User specific code, enable interrupts:
    //

    //
    // Enable ADCINT1 in PIE
    //
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1; // Enable INT 1.1 in the PIE
    IER |= M_INT1;                     // Enable CPU Interrupt 1
    EINT;                              // Enable Global interrupt INTM
    ERTM;                              // Enable Global realtime interrupt DBGM

    LoopCount = 0;
    ConversionCount = 0;

    //
    // Assumes ePWM1 clock is already enabled in InitSysCtrl();
    //
    EPwm1Regs.ETSEL.bit.SOCAEN  = 1;        // Enable SOC on A group

    //
    // Select SOC from from CPMA on upcount
    //
    EPwm1Regs.ETSEL.bit.SOCASEL = 4;

    EPwm1Regs.ETPS.bit.SOCAPRD  = 1;        // Generate pulse on 1st event
    EPwm1Regs.CMPA.half.CMPA    = 0x0080;   // Set compare A value
    EPwm1Regs.TBPRD             = 0xFFFF;   // Set period for ePWM1
    EPwm1Regs.TBCTL.bit.CTRMODE = 0;        // count up and start
}

void
main(void)
{
    DoBootSequence();
    //
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    //
    EALLOW;    // This is needed to write to EALLOW protected registers
    PieVectTable.TINT0 = &cpu_timer0_isr;
    EDIS;      // This is needed to disable write to EALLOW protected registers

    //
    // Step 4. Initialize the Device Peripheral. This function can be
    //         found in F2806x_CpuTimers.c
    //
    InitCpuTimers();   // For this example, only initialize the Cpu Timers

    //
    // Configure CPU-Timer 0 to interrupt every 500 milliseconds:
    // 80MHz CPU Freq, 50 millisecond Period (in uSeconds)
    //
    ConfigCpuTimer(&CpuTimer0, 80, 500000);

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

    //
    // Configure GPIO34 as a GPIO output pin
    //
    EALLOW;
    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
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
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM

    //
    // Step 6. IDLE loop. Just sit and loop forever (optional)
    //
    for(;;);
}


__interrupt void
cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;

    //
    // Toggle GPIO34 once per 500 milliseconds
    //
    GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;

    // TODO: Get temperature

    //
    // Acknowledge this interrupt to receive more interrupts from group 1
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

//
// End of File
//

