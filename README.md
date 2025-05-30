# Reading INA219 Sensors

## Overview
- Supports up to 4 INA219 devices.
- Connects to **A4** (SDA) and **A5** (SCL) pins of the Arduino Pro Mini.

---

## Connection Diagram

### **RX to Arduino**
| RX Pin   | Arduino Pin     |
|----------|-----------------|
| GND      | GND             |
| VDD      | RAW             |
| Ext/E1   | D3              |
| -        | D3 <R2.4k> D4   |

---

### **INA219 to Arduino**
| INA219 Pin | Arduino Pin |
|------------|-------------|
| VCC        | VCC         |
| GND        | GND         |
| SDA        | A4          |
| SCL        | A5          |

---

### **INA219 to Battery**
| INA219 Pin | Battery Terminal |
|------------|------------------|
| VIN+       | Positive         |
| VIN-       | Negative         |

---

### **I2C Addresses for Multiple INA219 Devices**
| Device | A1 Pin | A0 Pin | I2C Address |
|--------|--------|--------|-------------|
| 1      | GND    | GND    | `0x40`      |
| 2      | GND    | VCC    | `0x41`      |
| 3      | VCC    | GND    | `0x42`      |
| 4      | VCC    | VCC    | `0x43`      |
