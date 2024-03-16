

void setup() {

}

void loop() {

}

// Follow the flow in Figure 41.11 for reading the sensor

// Write Request Interrupt.  
void CTSUWR_handler() {
  // This is called when Status Counter is 1
  
  // Set the measurement channel to move to Status 2
       
  // Write to CTSUSO1 to select the clock divider to move to status 3
  
}

// Data Ready Interrupt
void CTSURD_handler() {
  // This will be called at Status Counter 5 when measurement is ready to read
  
  // Read the results from the counter registers.
  
  // Restart the measurement.
  
}

void startCTSUmeasure() {
  // There has to be at least one measurement channel to software start

  // Software Start in CTSUCR0 register  
}

void initialCTSUsetup() {
  // Follow the flow chart Fig 41.9
  // Step 1: Discharge LPF (set TSCAP as OUTPUT LOW.)

  // Step 2: Setup I/O port PmnPFR registers
      // TSCAP Pin

      // Sensor Pin

  // Step 3: Enable CTSU in MSTPCRC bit MSTPC3 to 0

  // Step 4: Set CTSU Power Supply (CTSUCR1 register)

  // Step 5: Set CTSU Base Clock (CTSUCR1 and CTSUSO1 registers)

  // Step 6: Power On CTSU (set bits CTSUPON and CTSUCSW in CTSUCR1 at the same time)

  // Step 7: Wait for stabilization (Whatever that means...)

  // Continued from flow chart Fig 41.12

  // Step 8: Other Registers
      // CTSUCR1   // Select operating clock

      // CTSUSDPRS  // Noise Filter

      // CTSUSST   // Stabilization Time

      // CTSUCHAC0-4  Select the channel for measurement

      // CTSUCR0  // Control Start Signals  

      // CTSUDCLKC  ???  

  // Step 9:   
      // Enable the interrupts

  // Step 10: Start Measurement

}

