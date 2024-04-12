/*

Firmware: emonTx5_DB_12CT_rf (Based on emonLibDB example EmonTx4DB_rf)

Due to the limits imposed by the data format required by the radio module, it is necessary to split the data into
two packets sent consecutively. Each packet is sent from a different NodeID, but both relate to the same datalog period
and share the same message number.

The first packet contains values pertaining to CTs 1-6, the second packet to CTs 7 - 12. 
*/

#define Serial Serial3
#include <Arduino.h>

#include "emonLibDB.h"
#define EMONTX4          // Must be above "#include <RFM69_LPL.h>

#include <RFM69_LPL.h>

#define DATALOG 9.8   
#define DATAWAIT 120    // millisecs between data packets
#define ACK1_RETRIES 8
#define ACK1_TIMEOUT 30
#define ACK2_RETRIES 8
#define ACK2_TIMEOUT 30

#define ENCRYPTION_KEY "89txbe4p8aik5kt3"

#define EXPANSION_BOARD  // Include for 12 CT's using expansion board

const char *firmware_version = {"1.0.0\n\r"};

RFM69 rf;

bool rfHealthy = false;

uint16_t networkGroup = 210;
uint16_t baseID = 5;

uint16_t NodeID1 = 28;
struct {
    uint32_t Msg;
    int16_t V1,V2,V3,P1,P2,P3,P4,P5,P6; 
    int32_t E1,E2,E3,E4,E5,E6; 
    uint32_t pulse;
    uint16_t ana;
} txPacket1;

/*  52 bytes
[[28]]
    nodename = emonTx5_28
    [[[rx]]]
        names = MSG, Vrms1, Vrms2, Vrms3, P1, P2, P3, P4, P5, P6, E1, E2, E3, E4, E5, E6, pulse, Analog
        datacodes = L, h, h, h, h, h, h, h, h, h, l, l, l, l, l, l, L, H
        scales = 1.0, 0.01, 0.01, 0.01, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0
        units = n, V, V, V, W, W, W, W, W, W, Wh, Wh, Wh, Wh, Wh, Wh, p, n
*/
        
uint16_t NodeID2 = 29;
struct {
    uint32_t Msg;
    int16_t V2,V3,P7,P8,P9,P10,P11,P12; 
    int32_t E7,E8,E9,E10,E11,E12; 
    uint32_t pulseD, pulseA;
} txPacket2;

/*  52 bytes
[[29]]
    nodename = emonTx5_29
    [[[rx]]]
        names = MSG, Vrms2, Vrms3, P7, P8, P9, P10, P11, P12, E7, E8, E9, E10, E11, E12, digPulse, anaPulse
        datacodes = L, h, h, h, h, h, h, h, h, l, l, l, l, l, l, L, L
        scales = 1.0, 0.01, 0.01, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0
        units = n, V, V, W, W, W, W, W, W, Wh, Wh, Wh, Wh, Wh, Wh, p, p
*/
        
const uint8_t LEDpin = PIN_PC2;                   // emonTx5 LED
uint32_t counter = 0;
void setup() 
{  

  pinMode(LEDpin, OUTPUT);

  Serial.begin(9600);
  Serial.println("Set baud=115200");
  Serial.end();
  Serial.begin(115200);
  
  Serial.print(F("emonTx5_DB_12CT_rf V")); Serial.write(firmware_version);
  Serial.println(F("OpenEnergyMonitor.org"));

  /****************************************************************************
  *                                                                           *
  * Initialise the radio                                                      *
  *                                                                           *
  ****************************************************************************/
  
  rf.setPins(PIN_PA7,PIN_PA4,PIN_PA5,PIN_PA6);
  if(rf.initialize(RF69_433MHZ, NodeID1, networkGroup))
  {
    rfHealthy = true;
    rf.encrypt(ENCRYPTION_KEY);
    Serial.println("RadioFormat: LowPowerLabs");
  }
  else
  {
    Serial.println("Unable to start Radio Module");
    panic(2,10);
    Serial.println("Continuing without radio.");
  }
  
  // Disable radio transmit
  // rfHealthy = false;
  
  /****************************************************************************
  *                                                                           *
  * Set the properties of the physical sensors                                *
  *                                                                           *
  ****************************************************************************/
  
  EmonLibDB_set_vInput(1, 101.3, 0.16);  
  
  /* Include the next two lines if you have a 3-phase emonVS */
 
  // EmonLibDB_set_vInput(2, 101.3, 0.16); 
  // EmonLibDB_set_vInput(3, 101.3, 0.16); 
  
  EmonLibDB_set_cInput(1, 100.0, 3.2);
  EmonLibDB_set_cInput(2, 100.0, 3.2);
  EmonLibDB_set_cInput(3, 100.0, 3.2);
  EmonLibDB_set_cInput(4, 50.0, 3.2);
  EmonLibDB_set_cInput(5, 50.0, 3.2);
  EmonLibDB_set_cInput(6, 50.0, 3.2);
  
#ifdef EXPANSION_BOARD

  EmonLibDB_set_cInput(7, 20.0, 3.2);
  EmonLibDB_set_cInput(8, 20.0, 3.2);
  EmonLibDB_set_cInput(9, 20.0, 3.2);
  EmonLibDB_set_cInput(10, 20.0, 3.2);
  EmonLibDB_set_cInput(11, 20.0, 3.2);
  EmonLibDB_set_cInput(12, 20.0, 3.2);
  
#endif  

  /****************************************************************************
  *                                                                           *
  * Link voltage and current sensors to define the power                      *
  *  & energy measurements                                                    *
  *                                                                           *
  * For best precision and performance, include only the following lines      *
  *    that apply to current/power inputs being used.                         *
  ****************************************************************************/

/*
  EmonLibDB_set_pInput(1, 1);                  // CT1, V1
*/
  EmonLibDB_set_pInput(1, 1);                  // CT2, V2 (etc)
  EmonLibDB_set_pInput(2, 1);
  EmonLibDB_set_pInput(3, 1);
  EmonLibDB_set_pInput(4, 1);  
  EmonLibDB_set_pInput(5, 1);
  EmonLibDB_set_pInput(6, 1);

#ifdef EXPANSION_BOARD

  EmonLibDB_set_pInput(7, 1);                  // CT7, V1  
  EmonLibDB_set_pInput(8, 1);                  // CT8, V2 (etc)
  EmonLibDB_set_pInput(9, 1);
  EmonLibDB_set_pInput(10, 1);  
  EmonLibDB_set_pInput(11, 1);
  EmonLibDB_set_pInput(12, 1);

#endif

  /* How to measure Line-Line loads: */
/*
  EmonLibDB_set_pInput(3, 1, 2);               // CT1 between V1 & V2    
  EmonLibDB_set_pInput(2, 2, 3);               // CT2 between V2 & V3
  EmonLibDB_set_pInput(3, 3, 1);               // CT2 between V3 & V1  (etc)  
  EmonLibDB_set_pInput(4, 1, 2);  
  EmonLibDB_set_pInput(5, 2, 3);
  EmonLibDB_set_pInput(6, 3, 1);

  EmonLibDB_set_pInput(7, 1, 2);  
  EmonLibDB_set_pInput(8, 2, 3);
  EmonLibDB_set_pInput(9, 3, 1);
  EmonLibDB_set_pInput(10, 1, 2);  
  EmonLibDB_set_pInput(11, 2, 3);
  EmonLibDB_set_pInput(12, 3, 1);
*/

/*
    Pulse counting on any channel is only available if the appropriate solder link is made on the hardware,
      and the related "TMP" link is broken.
*/      
  EmonLibDB_setPulseEnable(true);              // Enable counting on the "Pulse" input
  EmonLibDB_setPulseMinPeriod(20);             // Contact bounce must not last longer than 20 ms
  // EmonLibDB_setPulseEnable(Dig, true);         // Enable counting on the "Pulse" input
  // EmonLibDB_setPulseMinPeriod(Dig, 20);        // Contact bounce must not last longer than 20 ms

  EmonLibDB_datalogPeriod(DATALOG);            // Report every 9.8 s (approx)
  Serial.print("Starting ");
  EmonLibDB_Init();                            // Start continuous monitoring.
  Serial.print("reports every ");
  Serial.print(EmonLibDB_getDatalogPeriod());
  Serial.println(" seconds approx.\n");
  
}

void loop()             
{
  static uint32_t counter = 0;

  if (EmonLibDB_Ready())   
  {
    txPacket1.Msg = counter;
    txPacket1.V1 = EmonLibDB_getVrms(1) * 100.0;
    txPacket1.V2 = EmonLibDB_getVrms(2) * 100.0;
    txPacket1.V3 = EmonLibDB_getVrms(3) * 100.0;
    txPacket1.P1 = EmonLibDB_getRealPower(1);
    txPacket1.P2 = EmonLibDB_getRealPower(2);
    txPacket1.P3 = EmonLibDB_getRealPower(3);
    txPacket1.P4 = EmonLibDB_getRealPower(4);
    txPacket1.P5 = EmonLibDB_getRealPower(5);
    txPacket1.P6 = EmonLibDB_getRealPower(6);
    txPacket1.E1 = EmonLibDB_getWattHour(1);
    txPacket1.E2 = EmonLibDB_getWattHour(2);
    txPacket1.E3 = EmonLibDB_getWattHour(3);
    txPacket1.E4 = EmonLibDB_getWattHour(4);
    txPacket1.E5 = EmonLibDB_getWattHour(5);
    txPacket1.E6 = EmonLibDB_getWattHour(6);
    txPacket1.pulse = EmonLibDB_getPulseCount(1);
    txPacket1.ana = EmonLibDB_getAnalogueCount();
        
    if (rfHealthy)
    {
      rf.setAddress(NodeID1);
      if (!rf.sendWithRetry(baseID,(byte *)&txPacket1, sizeof(txPacket1), ACK1_RETRIES, ACK1_TIMEOUT)) {
        // Serial.println("RF No Ack (1)");
      }
    }
    
#ifdef EXPANSION_BOARD

    txPacket2.Msg = counter;
    txPacket2.V2 = EmonLibDB_getVrms(2) * 100.0;
    txPacket2.V3 = EmonLibDB_getVrms(3) * 100.0;
    txPacket2.P7 = EmonLibDB_getRealPower(7);
    txPacket2.P8 = EmonLibDB_getRealPower(8);
    txPacket2.P9 = EmonLibDB_getRealPower(9);
    txPacket2.P10 = EmonLibDB_getRealPower(10);
    txPacket2.P11 = EmonLibDB_getRealPower(11);
    txPacket2.P12 = EmonLibDB_getRealPower(12);
    txPacket2.E7 = EmonLibDB_getWattHour(7);
    txPacket2.E8 = EmonLibDB_getWattHour(8);
    txPacket2.E9 = EmonLibDB_getWattHour(9);
    txPacket2.E10 = EmonLibDB_getWattHour(10);
    txPacket2.E11 = EmonLibDB_getWattHour(11);
    txPacket2.E12 = EmonLibDB_getWattHour(12);
    txPacket2.pulseD = EmonLibDB_getPulseCount(2);
    txPacket2.pulseA = EmonLibDB_getPulseCount(3);
    
    delay(DATAWAIT);
  
    if (rfHealthy)
    {
      rf.setAddress(NodeID2);
      if (!rf.sendWithRetry(baseID,(byte *)&txPacket2, sizeof(txPacket2), ACK2_RETRIES, ACK2_TIMEOUT)) {
        // Serial.println("RF No Ack (2)");
      }
    }
    
#endif
    
    delay(100);
    
    Serial.print("MSG:"); Serial.print(counter++); Serial.print(",");
    // Serial.print(EmonLibDB_acPresent(1)?"  AC present ":"  AC missing ");
    
    Serial.print("V1:"); Serial.print(EmonLibDB_getVrms(1)); Serial.print(",");
    // Serial.print("V2:"); Serial.print(EmonLibDB_getVrms(2)); Serial.print(",");
    // Serial.print("V3:"); Serial.print(EmonLibDB_getVrms(3)); Serial.print(",");
    
    Serial.print("F:"); Serial.print(EmonLibDB_getLineFrequency()); Serial.print(",");
    
#ifndef EXPANSION_BOARD
    for (uint8_t ch=1; ch<=6; ch++)
#else
    for (uint8_t ch=1; ch<=12; ch++)
#endif
    {
      Serial.print(",P"); Serial.print(ch); Serial.print(":"); Serial.print(EmonLibDB_getRealPower(ch));
      // Serial.print(",I"); Serial.print(ch); Serial.print(":"); Serial.print(EmonLibDB_getIrms(ch),3);
      // Serial.print(",VA"); Serial.print(ch); Serial.print(":"); Serial.print(EmonLibDB_getApparentPower(ch));
      // Serial.print(",E"); Serial.print(ch); Serial.print(":"); Serial.print(EmonLibDB_getWattHour(ch));
      // Serial.print(",PF"); Serial.print(ch); Serial.print(":"); Serial.print(EmonLibDB_getPF(ch),4);
    } 
    Serial.println();
    delay(200);  
  }
}

void panic(uint8_t rate, uint8_t duration)
{
  while(duration)
  {
    PORTB.OUT |= PIN2_bm;
    delay(rate<<8);
    PORTB.OUT &= ~PIN2_bm;
    delay(rate<<8);
    duration--;
  }
  return;
}