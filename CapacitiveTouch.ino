
#include "EventLinkInterrupt.h"

int ctsurdEventLinkIndex = 0;
int ctsuwrEventLinkIndex = 0;

volatile boolean newData = false;
volatile uint16_t sCounter = 0;
volatile uint16_t rCounter = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("\n\n\n*** Starting R4_CTSU_Test.ino***\n\n\n");
  initialCTSUsetup();
}

void loop() {
  if (newData) {
    newData = false;
    noInterrupts();
    uint32_t s = sCounter;
    uint32_t r = rCounter;
    interrupts();
    Serial.print("Result : ");
    Serial.print(s);
    Serial.print(" : ");
    Serial.print(r);
    Serial.println();
  }
}

// Follow the flow in Figure 41.11 for reading the sensor

// Write Request Interrupt.  
void CTSUWR_handler() {
  // This is called when Status Counter is 1
  resetEventLink(ctsuwrEventLinkIndex);
  
  // Set the measurement channel to move to Status 2
  R_CTSU->CTSUMCH0 = 0;
       
  // Write to CTSUSO1 to select the clock divider to move to status 3
  R_CTSU->CTSUSO1 = 0x0F00;
  
}

// Data Ready Interrupt
void CTSURD_handler() {
  // This will be called at Status Counter 5 when measurement is ready to read
  resetEventLink(ctsurdEventLinkIndex);
  
  // Read the results from the counter registers.
  sCounter = R_CTSU->CTSUSC;
  rCounter = R_CTSU->CTSURC;
  newData = true;
  
  // Restart the measurement.
  startCTSUmeasure();
  
}

void startCTSUmeasure() {
  // There has to be at least one measurement channel to software start
  R_CTSU->CTSUMCH0 = 0;

  // Software Start in CTSUCR0 register  
  R_CTSU->CTSUCR0 = 1;
}

void initialCTSUsetup() {
  // Follow the flow chart Fig 41.9
  // Step 1: Discharge LPF (set TSCAP as OUTPUT LOW.)
  R_PFS->PORT[1].PIN[12].PmnPFS = (1 << R_PFS_PORT_PIN_PmnPFS_PDR_Pos);
  delay(1000);

  // Step 2: Setup I/O port PmnPFR registers
      // TSCAP Pin  
  R_PFS->PORT[1].PIN[12].PmnPFS = (1 << R_PFS_PORT_PIN_PmnPFS_PMR_Pos) | (0x0C << R_PFS_PORT_PIN_PmnPFS_PSEL_Pos);

      // Sensor Pin
  R_PFS->PORT[2].PIN[4].PmnPFS = (1 << R_PFS_PORT_PIN_PmnPFS_PMR_Pos) | (0x0C << R_PFS_PORT_PIN_PmnPFS_PSEL_Pos);

  // Step 3: Enable CTSU in MSTPCRC bit MSTPC3 to 0
  R_MSTP->MSTPCRC &= ~(1 << R_MSTP_MSTPCRC_MSTPC3_Pos);

  // Step 4: Set CTSU Power Supply (CTSUCR1 register)  
  R_CTSU->CTSUCR1 = 0;

  // Step 5: Set CTSU Base Clock (CTSUCR1 and CTSUSO1 registers)
  R_CTSU->CTSUSO1 = 0x0F00;

  // Step 6: Power On CTSU (set bits CTSUPON and CTSUCSW in CTSUCR1 at the same time)
  R_CTSU->CTSUCR1 |= 0x03;

  // Step 7: Wait for stabilization (Whatever that means...)
  delay(1000);

  // Continued from flow chart Fig 41.12

  // Step 8: Other Registers
      // CTSUCR1   // Select operating clock

      // CTSUSDPRS  // Noise Filter
  R_CTSU->CTSUSDPRS = 0x63;

      // CTSUSST   // Stabilization Time
  R_CTSU->CTSUSST = 0x10;

      // CTSUCHAC0-4  Select the channel for measurement
  R_CTSU->CTSUCHAC[0] = 1;
  R_CTSU->CTSUMCH0 = 0;

      // CTSUCR0  // Control Start Signals  

      // CTSUDCLKC  ???  
  R_CTSU->CTSUDCLKC = 0x30;

  // Step 9:   
      // Enable the interrupts
  // CTSUWR is event 0x42
  // CTSURD is event 0x43
  // CTSUFN is event 0x44
  ctsurdEventLinkIndex = attachEventLinkInterrupt(0x43, CTSURD_handler);
  ctsuwrEventLinkIndex = attachEventLinkInterrupt(0x42, CTSUWR_handler);

  // Step 10: Start Measurement
  startCTSUmeasure();

}

