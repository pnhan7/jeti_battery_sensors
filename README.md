# Reading INA219 Sensors

## Overview
- Supports up to 4 INA219 devices.
- Connects to **VCC** (VCC), **GND** (GND), **A4** (SDA) and **A5** (SCL) pins of the Arduino Pro Mini(5v/16MHz) to INA219.

---

## Connection Diagram

### **RX to Arduino**
| Ext/E1/E2 Port  | Arduino Pin         |
|----------|---------------------|
| -      | GND                 |
| +      | RAW                 |
| ~   | D3                  |
|         | D3 → R (2.4kΩ) → D4 |

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
| VIN-       | Positive         |
| GND       | Negative         |

---

### **I2C Addresses for Multiple INA219 Devices**
| Device | A1 Pin | A0 Pin | I2C Address |
|--------|--------|--------|-------------|
| 1      | GND    | GND    | `0x40`      |
| 2      | GND    | VCC    | `0x41`      |
| 3      | VCC    | GND    | `0x42`      |
| 4      | VCC    | VCC    | `0x43`      |
