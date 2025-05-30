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
