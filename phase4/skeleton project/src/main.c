#include <src/main.h>

void
DoBootSequence()
{
    // Adjust console buffering to flush when newline is output.
    D(setvbuf(stdout, NULL, _IOLBF, 0));

    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);

    //
    // Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks
    // This example function is found in the F2806x_SysCtrl.c file.
    //
    InitSysCtrl();

    //
    // Step 2. Initalize GPIO:
    // Enable XCLOCKOUT to allow monitoring of oscillator 1
    //
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;  // enable XCLOCKOUT through GPIO mux
    SysCtrlRegs.XCLK.bit.XCLKOUTDIV = 2;  // XCLOCKOUT = SYSCLK
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

    InitAdc();
    AdcOffsetSelfCal();

}

int16 SampleTemperature(void)
{
    int16 temp;

    //
    // Force start of conversion on SOC0
    //
    AdcRegs.ADCSOCFRC1.all = 0x01;

    //
    // Wait for end of conversion.
    //
    while(AdcRegs.ADCINTFLG.bit.ADCINT1 == 0)
    {
        //
        // Wait for ADCINT1
        //
    }
    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;        // Clear ADCINT1

    //
    // Get temp sensor sample result from SOC0
    //
    temp = AdcResult.ADCRESULT0;

    //
    // Convert the raw temperature sensor measurement into temperature
    //
    return(GetTemperatureC(temp));
}

// This calculates Fast Fourier Transform
int16 CalculateFastFourierTransform(void)
{
    fft_flag = FFT_FLAG;

    scale_flag = SCALE_FLAG;

    data = <1024-point Complex input>;

    // Bit-Reverse 1024-point data, Store into data_br, data_br aligned to
    // 12-least significant binary zeros

    hwafft_br(data, data_br, DATA_LEN_1024);

    // Bit-reverse input data, destination buffer aligned

    data = data_br;

    // Compute 1024-point FFT with scaling enabled

    out_sel = hwafft_1024pts(data, scratch, fft_flag, scale_flag);

    if (out_sel == OUT_SEL_DATA) {

        result = data;

    } else {

        result = scratch;
    }

    return result;
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


    EALLOW;
    AdcRegs.ADCCTL2.bit.ADCNONOVERLAP = 1;  // Enable non-overlap mode

    //
    // Connect channel A5 internally to the temperature sensor
    //
    AdcRegs.ADCCTL1.bit.TEMPCONV  = 1;

    AdcRegs.ADCSOC0CTL.bit.CHSEL  = 5;  // Set SOC0 channel select to ADCINA5

    //
    // Set SOC0 acquisition period to 26 ADCCLK
    //
    AdcRegs.ADCSOC0CTL.bit.ACQPS  = 25;

    AdcRegs.INTSEL1N2.bit.INT1SEL = 0;  // Connect ADCINT1 to EOC0
    AdcRegs.INTSEL1N2.bit.INT1E   = 1;  // Enable ADCINT1

    //
    // Set the flash OTP wait-states to minimum. This is important
    // for the performance of the temperature conversion function.
    //
    FlashRegs.FOTPWAIT.bit.OTPWAIT = 1;
    EDIS;

    InitCpuTimers();

    //
    // Configure CPU-Timer 0 to interrupt every 500 milliseconds:
    // 80MHz CPU Freq, 50 millisecond Period (in uSeconds)
    //
    ConfigCpuTimer(&CpuTimer0, 80, 3000000);

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
    EALLOW;
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1;
    GpioDataRegs.GPBDAT.bit.GPIO34 = 1;
    GpioDataRegs.GPBDAT.bit.GPIO39 = 1;
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

    // Blink LED just to verify that the interupts are occuring.
    EALLOW;
    GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
    GpioDataRegs.GPBTOGGLE.bit.GPIO39 = 1;
    EDIS;
    int16 temp = SampleTemperature();
    D(printf("Temp: %d\n", AdcResult.ADCRESULT0));
    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

    // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}

//
// End of File
//

