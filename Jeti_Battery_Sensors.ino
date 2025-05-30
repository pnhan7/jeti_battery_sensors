/*
Jeti INA219 Sensors v 1.0

Read INA219 sensors, up to 4 devices, connect to A4, A5 of Ardunio Pro Mini 

Connect diagram
RX     Arduino
GND    GND
VDD    RAW
Ext/E1 D3
-      D3 <R2.4k> D4

INA219 Arduino
VCC    VCC
GNC    GND
SDA    A4
SCL    A5

INA219 Battery 
VIN-   Positive
GND    Negative

INA219    A1	    A0      I2C Address
1	        GND	    GND	    0x40
2	        GND	    VCC	    0x41
3	        VCC	    GND	    0x42
4	        VCC	    VCC	    0x43

*/

#include <EEPROM.h>
#include <stdlib.h>
#include <Adafruit_INA219.h>
#include "SoftwareSerialJeti.h"
#include "JETI_EX_SENSOR.h"

#define prog_char char PROGMEM
#define GETCHAR_TIMEOUT_ms 20

#ifndef JETI_RX
#define JETI_RX 3
#endif

#ifndef JETI_TX
#define JETI_TX 4
#endif

#define JB_INIT F("Battery")

#define ITEMNAME_1 F("1")
#define ITEMTYPE_1 F("V")
#define ITEMVAL_1 &sens_voltage1

#define ITEMNAME_2 F("2")
#define ITEMTYPE_2 F("V")
#define ITEMVAL_2 &sens_voltage2

#define ITEMNAME_3 F("3")
#define ITEMTYPE_3 F("V")
#define ITEMVAL_3 &sens_voltage3

#define ITEMNAME_4 F("4")
#define ITEMTYPE_4 F("V")
#define ITEMVAL_4 &sens_voltage3


#define ABOUT_1 F(" Batterty")
#define ABOUT_2 F("Batterty")

SoftwareSerial JetiSerial(JETI_RX, JETI_TX);

void JetiUartInit() {
  JetiSerial.begin(9700);
}

void JetiTransmitByte(unsigned char data, boolean setBit9) {
  JetiSerial.set9bit = setBit9;
  JetiSerial.write(data);
  JetiSerial.set9bit = 0;
}

unsigned char JetiGetChar(void) {
  unsigned long time = millis();
  while (JetiSerial.available() == 0) {
    if (millis() - time > GETCHAR_TIMEOUT_ms)
      return 0;
  }
  int read = -1;
  if (JetiSerial.available() > 0) {
    read = JetiSerial.read();
  }
  long wait = (millis() - time) - GETCHAR_TIMEOUT_ms;
  if (wait > 0)
    delay(wait);
  return read;
}

JETI_Box_class JB;

unsigned char SendFrame() {
  boolean bit9 = false;
  for (int i = 0; i < JB.frameSize; i++) {
    if (i == 0)
      bit9 = false;
    else if (i == JB.frameSize - 1)
      bit9 = false;
    else if (i == JB.middle_bit9)
      bit9 = false;
    else
      bit9 = true;
    JetiTransmitByte(JB.frame[i], bit9);
  }
}

unsigned char DisplayFrame() {
  for (int i = 0; i < JB.frameSize; i++) {
  }
}

//uint8_t frame[10];
//short value = 27;

float sens_voltage1 = 0.0;
float sens_voltage2 = 0.0;
float sens_voltage3 = 0.0;
float sens_voltage4 = 0.0;

bool is_connected_ina219_1 = false;
bool is_connected_ina219_2 = false;
bool is_connected_ina219_3 = false;
bool is_connected_ina219_4 = false;

Adafruit_INA219 ina219_1(0x40);
Adafruit_INA219 ina219_2(0x41);
Adafruit_INA219 ina219_3(0x42);
Adafruit_INA219 ina219_4(0x43);

#define MAX_SCREEN 2
#define MAX_CONFIG 1
#define COND_LES_EQUAL 1
#define COND_MORE_EQUAL 2

#define INA_LED_STATUS true
//setup ina219 and blink the led
void setupINA219() {

  uint8_t deviceStatus = 0;
  // Check INA219 connections and build the binary status
  if (ina219_1.begin()) {
    is_connected_ina219_1 = true;
    deviceStatus |= 0b0001;  // Set Bit 0
  }
  if (ina219_2.begin()) {
    is_connected_ina219_2 = true;
    deviceStatus |= 0b0010;  // Set Bit 1
  }
  if (ina219_3.begin()) {
    is_connected_ina219_3 = true;
    deviceStatus |= 0b0100;  // Set Bit 2
  }

  if (ina219_4.begin()) {
    is_connected_ina219_4 = true;
    deviceStatus |= 0b1000;  // Set Bit 3
  }

  // Blink LED to indicate the connection status in decimal
  if (INA_LED_STATUS) {
    pinMode(13, OUTPUT);
    for (int i = 0; i < deviceStatus; i++) {
      digitalWrite(13, HIGH);  // Turn LED ON
      delay(100);              // Keep it ON for 100ms
      digitalWrite(13, LOW);   // Turn LED OFF
      delay(100);              // Wait for 100ms before the next blink
    }
    //delay(500);
    //digitalWrite(13, LOW);
  }
}

void setup() {

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  pinMode(JETI_RX, OUTPUT);
  JetiUartInit();

  JB.JetiBox(ABOUT_1, ABOUT_2);
  JB.Init(JB_INIT);

  JB.addData(ITEMNAME_1, ITEMTYPE_1);
  JB.setValue(1, ITEMVAL_1, 2);

  JB.addData(ITEMNAME_2, ITEMTYPE_2);
  JB.setValue(2, ITEMVAL_2, 2);

  JB.addData(ITEMNAME_3, ITEMTYPE_3);
  JB.setValue(3, ITEMVAL_3, 2);

  JB.addData(ITEMNAME_4, ITEMTYPE_4);
  JB.setValue(4, ITEMVAL_4, 2);

  do {
    JB.createFrame(1);
    SendFrame();
    delay(GETCHAR_TIMEOUT_ms);
  } while (sensorFrameName != 0);
  digitalWrite(13, LOW);
  //setup INA219
  setupINA219();
}

int header = 0;
int lastbtn = 240;
int current_screen = 0;
int current_config = 0;
char temp[LCDMaxPos / 2];
char msg_line1[LCDMaxPos / 2];
char msg_line2[LCDMaxPos / 2];

void process_screens() {
  switch (current_screen) {
    case 0:
      {
        JB.JetiBox(ABOUT_1, ABOUT_2);
        break;
      }
    case MAX_SCREEN:
      {
        JB.JetiBox(ABOUT_1, ABOUT_2);
        break;
      }
  }
}

void loop() {

  if (is_connected_ina219_1) {
    sens_voltage1 = ina219_1.getBusVoltage_V();
  }
  if (is_connected_ina219_2) {
    sens_voltage2 = ina219_2.getBusVoltage_V();
  }

  if (is_connected_ina219_3) {
    sens_voltage3 = ina219_3.getBusVoltage_V();
  }

  if (is_connected_ina219_4) {
    sens_voltage4 = ina219_4.getBusVoltage_V();
  }

  unsigned long time = millis();
  SendFrame();
  time = millis();
  int read = 0;
  pinMode(JETI_RX, INPUT);
  pinMode(JETI_TX, INPUT_PULLUP);

  JetiSerial.listen();
  JetiSerial.flush();

  while (JetiSerial.available() == 0) {

    if (millis() - time > 5)
      break;
  }

  if (JetiSerial.available() > 0) {
    read = JetiSerial.read();

    if (lastbtn != read) {
      lastbtn = read;
      switch (read) {
        case 224:  // RIGHT
          break;
        case 112:  // LEFT
          break;
        case 208:  // UP
          break;
        case 176:  // DOWN
          break;
        case 144:  // UP+DOWN
          break;
        case 96:  // LEFT+RIGHT
          break;
      }
    }
  }

  if (current_screen != MAX_SCREEN)
    current_config = 0;
  process_screens();
  header++;
  if (header >= 5) {
    JB.createFrame(1);
    header = 0;
  } else {
    JB.createFrame(0);
  }

  long wait = GETCHAR_TIMEOUT_ms;
  long milli = millis() - time;
  if (milli > wait)
    wait = 0;
  else
    wait = wait - milli;
  pinMode(JETI_TX, OUTPUT);
}
