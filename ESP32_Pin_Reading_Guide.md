# 📌 ESP32 Wokwi Pin Guide for SWESU Project

## 🎯 The Golden Rule

**On ESP32: Use the GPIO NUMBER printed on the board, NOT the physical position!**

---

## 📋 Exact ESP32 Pin Layout in Wokwi

### LEFT SIDE (top to bottom):

```
3V3  ← 3.3V Power
EN   ← Enable
VP
VN
34
35
32
33
25
26
27
14
12
GND  ← Ground
13   ← ⭐ GPIO13 - YOUR TRIAC CONTROL PIN! ⭐
D2
D3
CMD
5V   ← 5V Power (for relay VCC)
```

### RIGHT SIDE (top to bottom):

```
GND  ← Ground
23
22
TX
RX
21
GND  ← Ground
19
18
5    ← GPIO5
17
16
4    ← GPIO4
0
2
15
D1
D0
CLK
```

---

## 🔌 Your SWESU Wiring (Correct Pins)

| Function              | Pin Label | Side     | Position                      |
| --------------------- | --------- | -------- | ----------------------------- |
| **Relay IN (GPIO13)** | **13**    | **LEFT** | **5th from bottom**           |
| Relay VCC             | **5V**    | LEFT     | 1st from bottom (very bottom) |
| Relay GND             | **GND**   | LEFT     | 6th from bottom (above "13")  |
| LED Power (3V3)       | **3V3**   | LEFT     | Very top                      |
| Additional GND        | **GND**   | RIGHT    | Top or 7th from top           |

---

## 📍 Finding GPIO13 - Visual Guide

```
LEFT SIDE (bottom portion):

5V   ← 1st from bottom (Relay VCC)
CMD  ← 2nd from bottom
D3   ← 3rd from bottom
D2   ← 4th from bottom
13   ← 5th from bottom ⭐ RELAY IN HERE! ⭐
GND  ← 6th from bottom (Relay GND)
12   ← 7th from bottom
14
...
```

---

## ✅ Quick Wiring Checklist

- [ ] GPIO13 (left side, 5th from bottom) → Relay IN
- [ ] 5V (left side, very bottom) → Relay VCC
- [ ] GND (left side, above GPIO13) → Relay GND
- [ ] 3V3 (left side, very top) → Relay NO (for LED power)

---

## 🔄 ESP8266-12F to ESP32 Mapping

| Your Hardware (ESP8266-12F)   | Wokwi Simulator (ESP32)             |
| ----------------------------- | ----------------------------------- |
| GPIO13 = Physical Pin 7       | GPIO13 = Left side, 5th from bottom |
| Code: `#define RELAY_PIN 13`  | Code: `#define RELAY_PIN 13`        |
| **Same GPIO number in code!** | **Same GPIO number in code!**       |

---

**Created:** January 13, 2026  
**Verified:** Pin layout confirmed from Wokwi screenshot
