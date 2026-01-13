# 🎯 SWESU Pin Mapping Clarification

## EasyEDA Design → Wokwi Simulator

**Issue Identified:** Your actual EasyEDA schematic uses **GPIO13 (pin 7)** for TRIAC control, but previous guidance suggested GPIO5. Let's fix this!

---

## 📋 Your ACTUAL EasyEDA Design

Based on your schematic description:

### ESP8266-12F Pin Assignment (YOUR DESIGN):

| Pin Number | Pin Name   | Function in YOUR Design        | Status                 |
| ---------- | ---------- | ------------------------------ | ---------------------- |
| **7**      | **GPIO13** | **TRIAC Control (to MOC3041)** | ✅ **USED - Active**   |
| 14         | GPIO5      | Not used                       | ❌ Marked with green X |
| 8          | GPIO12     | Not specified                  | Unknown                |
| 1          | GPIO14     | Not specified                  | Unknown                |
| 5          | GPIO4      | Possibly status LED            | Check schematic        |
| 6          | GPIO0      | Flash/programming              | Boot mode select       |
| 7 (left)   | VCC        | 3.3V Power                     | From AMS1117           |
| P (bottom) | GND        | Ground                         | Common ground          |

### Critical Finding:

**YOUR TRIAC CONTROL PIN IS GPIO13, NOT GPIO5!**

---

## 🔧 Updated Wokwi Simulator Setup

### **NEW Configuration (Matching Your Design):**

**For Wokwi ESP32:**

| Function                | YOUR Hardware (ESP8266) | Wokwi ESP32 Pin  | Code Setting           |
| ----------------------- | ----------------------- | ---------------- | ---------------------- |
| **TRIAC/Relay Control** | GPIO13 (pin 7)          | Pin labeled "13" | `#define RELAY_PIN 13` |
| Status LED (if used)    | GPIO4 (pin 5)           | Pin labeled "4"  | `#define LED_STATUS 4` |
| Power                   | VCC (3.3V)              | 3V3              | N/A                    |
| Ground                  | GND                     | GND              | N/A                    |
| Relay Power             | N/A (Wokwi only)        | VIN (5V)         | N/A                    |

---

## 💻 Updated Code for Wokwi

### Correct Pin Definition:

```cpp
// ============ UPDATED CONFIGURATION ============
// Matches your EasyEDA schematic!

#define RELAY_PIN 13     // GPIO13 = Pin 7 on ESP8266-12F
                         // This is what YOUR schematic uses!

#define LED_STATUS 4     // GPIO4 (optional, if you have status LED)

// NOT using GPIO5 - it's marked as unused in your design
```

---

## 🔌 Wokwi Wiring Instructions (CORRECTED)

**ESP32 Wokwi Pin Layout (LEFT side, top to bottom):**

```
3V3, EN, VP, VN, 34, 35, 32, 33, 25, 26, 27, 14, 12, GND, 13, D2, D3, CMD, 5V
```

### Connection 1: Control Signal

- **From:** ESP32 pin **"13"** (LEFT side, 5th from bottom)
- **To:** Relay Module "IN" pin

### Connection 2: Relay Power

- **From:** ESP32 pin **"5V"** (LEFT side, very bottom)
- **To:** Relay Module "VCC"

### Connection 3: Relay Ground

- **From:** ESP32 pin **"GND"** (LEFT side, 6th from bottom, above "13")
- **To:** Relay Module "GND"

### Connections 4-7: LED Load Circuit

- **3V3** (LEFT, very top) → Relay "NO"
- Relay "COM" → LED anode (+)
- LED cathode (-) → 220Ω resistor
- Resistor → GND (RIGHT side, top)

---

## 📊 Why GPIO13 in Your Design?

**Your choice of GPIO13 is actually GOOD!** Here's why:

| GPIO Pin   | Boot Behavior | Safe for TRIAC? | Notes                              |
| ---------- | ------------- | --------------- | ---------------------------------- |
| **GPIO13** | ✅ Safe       | ✅ **Yes**      | No special boot function           |
| GPIO5      | ✅ Safe       | ✅ Yes          | Also safe, but you're not using it |
| GPIO0      | ⚠️ Boot mode  | ❌ No           | Must be HIGH for normal boot       |
| GPIO15     | ⚠️ Boot mode  | ❌ No           | Must be LOW for normal boot        |
| GPIO2      | ⚠️ Boot mode  | ⚠️ Risky        | Must be HIGH for normal boot       |

**GPIO13 has no boot conflicts** - excellent choice for TRIAC control!

---

## 🎯 Complete Pin Mapping Table

### ESP8266-12F Physical Pinout:

```
        ESP8266-12F Module
    ┌──────────────────────┐
    │    [WiFi Antenna]    │
    ├──────────────────────┤
 1  │ REST                 │ (Reset)
 2  │ ADC                  │ (Analog input)
 3  │ EN                   │ (Enable, pull-up)
 4  │ GPIO16               │
 5  │ GPIO14               │
 6  │ GPIO12               │
 7  │ GPIO13 ──────────────┼──► TO TRIAC! ✅
 8  │ VCC (3.3V)           │
    ├──────────────────────┤
    │                      │
    ├──────────────────────┤
 9  │ GND                  │
10  │ GPIO15               │
11  │ GPIO2                │
12  │ GPIO0                │ (Flash button)
13  │ GPIO4                │ (Status LED?)
14  │ GPIO5  ─X──          │ NOT USED ❌
15  │ RXD0                 │
16  │ TXD0                 │
    └──────────────────────┘
         P = GND (bottom)
```

---

## 🔄 Code Migration Path

### Step 1: Test in Wokwi (ESP32) with GPIO13

```cpp
// Wokwi ESP32 Code
#include <WiFi.h>
#include <WebServer.h>

#define RELAY_PIN 13  // Matches your GPIO13 design!

WebServer server(80);

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  // ... WiFi setup ...
}

void loop() {
  server.handleClient();
}
```

### Step 2: Convert to ESP8266-12F (Your Hardware)

```cpp
// Your ESP8266-12F Hardware Code
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define RELAY_PIN 13  // Same GPIO13! No change needed!

ESP8266WebServer server(80);

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  // ... WiFi setup ...
}

void loop() {
  server.handleClient();
}
```

**Notice:** The `GPIO13` pin number **stays the same** in both versions!

---

## ✅ What You Need to Change in Wokwi

### In Your Current Wokwi Setup:

1. **Move the wire** from pin "5" to pin "**13**"

   - Disconnect: ESP32 pin "5" from Relay "IN"
   - Connect: ESP32 pin "**13**" to Relay "IN"

2. **Update the code:**

   ```cpp
   // Change this line:
   #define RELAY_PIN 5   // OLD - Wrong for your design

   // To this:
   #define RELAY_PIN 13  // NEW - Matches your EasyEDA schematic ✅
   ```

3. **All other wiring stays the same:**
   - VIN → Relay VCC
   - GND → Relay GND
   - 3V3 → Relay NO
   - Relay COM → LED → Resistor → GND

---

## 🎨 Visual Comparison

### What I Previously Said (WRONG for your design):

```
ESP8266 GPIO5 (pin 14) → MOC3041 → TRIAC
```

❌ **This doesn't match your schematic!**

### Your ACTUAL Design (CORRECT):

```
ESP8266 GPIO13 (pin 7) → MOC3041 → TRIAC
```

✅ **This is what you have!**

### Wokwi Simulation (Should Match Your Design):

```
ESP32 GPIO13 (pin labeled "13") → Relay Module
```

✅ **Use this in Wokwi!**

---

## 📝 Summary: What Was Wrong and How to Fix It

### The Confusion:

1. I **incorrectly assumed** standard GPIO5 was used
2. Your **actual schematic uses GPIO13**
3. Previous Wokwi guidance was for wrong pin
4. This created mismatch between simulation and hardware

### The Solution:

1. ✅ Update Wokwi code: `RELAY_PIN 13`
2. ✅ Move Wokwi wire to pin "13"
3. ✅ This now matches your EasyEDA design perfectly!
4. ✅ Code will transfer directly to hardware with just library name changes

---

## 🚀 Action Items for You

### Immediate (In Wokwi):

- [ ] Change code: `#define RELAY_PIN 13`
- [ ] Move wire from pin "5" to pin "13"
- [ ] Test simulation - relay should work!

### When Moving to Hardware:

- [ ] Verify GPIO13 (pin 7) connects to MOC3041 in your PCB
- [ ] Change 3 library includes (WiFi.h → ESP8266WiFi.h, etc.)
- [ ] Upload to ESP8266-12F
- [ ] No pin number changes needed - GPIO13 stays GPIO13!

---

## ❓ FAQ

**Q: Why did you say GPIO5 before?**
A: I made an assumption based on common ESP8266 designs. Your design is different (and equally valid!). GPIO13 is actually a great choice.

**Q: Will the Wokwi test still be valid?**
A: Yes! Using GPIO13 in Wokwi tests the exact same logic. The GPIO number just needs to match your actual design.

**Q: Do I need to redesign my PCB?**
A: No! Your GPIO13 design is perfectly fine. The issue was only with my simulation guidance.

**Q: Is GPIO13 better or worse than GPIO5?**
A: Both are equally good for TRIAC control. GPIO13 has no boot mode conflicts, making it a safe choice.

---

## 📞 Next Steps

1. **Update your Wokwi simulation** to use GPIO13
2. **Test the relay control** - should work identically
3. **Keep your EasyEDA design as-is** - GPIO13 is correct!
4. **When ready for hardware** - just change the library includes

**Your hardware design is fine. The confusion was only in the simulation guidance!**

---

**Created:** January 13, 2026  
**Issue:** Pin mapping mismatch  
**Resolution:** Use GPIO13 in both Wokwi sim and hardware  
**Status:** ✅ Clarified
