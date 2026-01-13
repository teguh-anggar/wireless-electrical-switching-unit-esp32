# Wokwi Board Selection Guide for SWESU ESP8266 Project

## 🎯 Quick Answer

**For your ESP8266-12F PCB design, use:**

### **Primary Choice: ESP8266 (NodeMCU)**

**Direct Link:** [https://wokwi.com/projects/new/esp8266](https://wokwi.com/projects/new/esp8266)

**Why:** 100% compatible with your hardware design!

---

## 📋 Step-by-Step: Finding ESP8266 in Wokwi

### Method 1: Direct URL (Fastest!)

1. **Copy this link:** `https://wokwi.com/projects/new/esp8266`
2. **Paste in browser** and press Enter
3. **Done!** You now have ESP8266 project

### Method 2: Through Wokwi Interface

1. Go to [https://wokwi.com](https://wokwi.com)
2. Click **"+ New Project"**
3. In the search box, type: **"esp8266"**
4. Select **"ESP8266 Arduino"** or **"NodeMCU ESP8266"**
5. Click to create

### Method 3: From Examples

1. Visit [https://wokwi.com/projects](https://wokwi.com/projects)
2. Search for: **"esp8266 relay"** or **"esp8266 web server"**
3. Open any example
4. Click **"Copy and Edit"**
5. Modify for your needs

---

## 🔧 Pin Mapping: ESP8266-12F to Wokwi NodeMCU

Your PCB design uses ESP8266-12F pins. Here's how they map to the NodeMCU board shown in Wokwi:

| ESP8266-12F Pin | NodeMCU Label | GPIO Number | Your Usage in SWESU              |
| --------------- | ------------- | ----------- | -------------------------------- |
| GPIO5           | **D1**        | GPIO5       | **TRIAC Trigger** (Main control) |
| GPIO4           | **D2**        | GPIO4       | **Status LED**                   |
| GPIO0           | **D3**        | GPIO0       | **Flash Button** (pull-up)       |
| GPIO14          | **D5**        | GPIO14      | **Zero-Cross Detect** (optional) |
| GPIO12          | **D6**        | GPIO12      | (Reserve for future)             |
| GPIO13          | **D7**        | GPIO13      | (Reserve for future)             |
| GPIO15          | **D8**        | GPIO15      | (Avoid - boot select)            |
| GPIO16          | **D0**        | GPIO16      | (Wake from sleep)                |
| 3V3             | **3V3**       | Power       | **3.3V Power**                   |
| GND             | **GND**       | Ground      | **Ground**                       |
| VIN/USB         | **VIN**       | 5V input    | **5V Power Input**               |
| EN              | **EN**        | Chip enable | **Pull-up to 3.3V**              |

### Wokwi Code Example with Correct Pins:

```cpp
// SWESU - ESP8266 Pin Configuration
#define RELAY_PIN 5      // D1 on NodeMCU = GPIO5 = TRIAC Trigger
#define LED_STATUS 4     // D2 on NodeMCU = GPIO4 = Status LED
#define FLASH_BTN 0      // D3 on NodeMCU = GPIO0 = Flash Button
#define ZERO_CROSS 14    // D5 on NodeMCU = GPIO14 = Optional

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);
  pinMode(FLASH_BTN, INPUT_PULLUP);

  digitalWrite(RELAY_PIN, LOW);  // Start OFF
}
```

---

## 🆚 Comparison: Board Options

### ✅ Best Match: ESP8266 (NodeMCU)

**Similarities to your ESP8266-12F:**

- ✅ Same WiFi chip (ESP8266)
- ✅ Same CPU architecture (Tensilica L106)
- ✅ Same libraries (`ESP8266WiFi.h`, `ESP8266WebServer.h`)
- ✅ Same GPIO pins (GPIO5, GPIO4, etc.)
- ✅ Same voltage (3.3V logic)
- ✅ Same flash size (4MB typical)
- ✅ Same power consumption (~80mA active)

**Code Transfer:**

```
Wokwi ESP8266 → Your Hardware ESP8266 = 100% compatible!
Just copy-paste the code and upload directly!
```

**Visual Difference:**

- Wokwi shows **NodeMCU board** = ESP8266-12E/F + USB, voltage regulator, buttons
- Your PCB will have **bare ESP8266-12F** + your own voltage regulator (AMS1117)
- **Logic is identical!** Only physical layout differs.

---

### ⚠️ Acceptable Alternative: ESP32

**If ESP8266 is truly not available**, use basic ESP32:

**Similarities:**

- ✅ WiFi (same 2.4GHz)
- ✅ Similar libraries (small changes needed)
- ✅ Same voltage (3.3V)
- ✅ Same programming method (Arduino IDE)

**Differences:**

- ⚠️ More GPIO pins (34 vs 11) - you won't use extras
- ⚠️ Dual-core CPU (vs single-core) - overkill for your use
- ⚠️ Has Bluetooth (you won't use it)
- ⚠️ Different library names:
  ```cpp
  // ESP8266                    // ESP32
  #include <ESP8266WiFi.h>   →  #include <WiFi.h>
  #include <ESP8266WebServer.h> → #include <WebServer.h>
  ```

**Code Changes Needed (ESP32 vs ESP8266):**

```cpp
// ESP8266 Version (Your Hardware)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);

// ESP32 Version (Wokwi Simulation)
#include <WiFi.h>
#include <WebServer.h>
WebServer server(80);

// Everything else is the SAME!
```

**Transfer Process:**

```
Wokwi ESP32 → Test logic → Change headers → Upload to ESP8266
(requires minor code changes)
```

---

## 🚫 Avoid These Options

| Board             | Why Avoid                                            |
| ----------------- | ---------------------------------------------------- |
| **ESP32-S2**      | No ESP8266 compatibility, different architecture     |
| **ESP32-S3**      | Too advanced (AI features), unnecessary              |
| **ESP32-C3**      | RISC-V CPU (different architecture), avoid confusion |
| **ESP32-C6**      | WiFi 6, Zigbee - features you don't need             |
| **ESP32-H2**      | **No WiFi!** Won't work at all                       |
| **M5Stack**       | Specific device with LCD, not relevant               |
| **XIAO ESP32-C3** | Tiny board, different pin layout                     |

---

## 🎓 Understanding the Architecture

### Your MVP Design (from MVP_ESP8266_Prototype.md):

```
┌──────────────────────────────────────────────────┐
│  Your PCB Design - ESP8266-12F Based             │
├──────────────────────────────────────────────────┤
│                                                  │
│  Components:                                     │
│  ├─ ESP8266-12F (bare chip)                      │
│  ├─ AMS1117-3.3V (voltage regulator)             │
│  ├─ HLK-PM01 (AC-DC 5V power supply)             │
│  ├─ MOC3041 + BT136 (TRIAC control)              │
│  └─ Supporting resistors, caps                   │
│                                                  │
│  Interface:                                      │
│  ├─ GPIO5 → MOC3041 (TRIAC trigger)              │
│  ├─ GPIO4 → Status LED                           │
│  └─ WiFi antenna (on-board)                      │
│                                                  │
└──────────────────────────────────────────────────┘
```

### Wokwi ESP8266 NodeMCU Simulation:

```
┌──────────────────────────────────────────────────┐
│  Wokwi ESP8266 NodeMCU Board                     │
├──────────────────────────────────────────────────┤
│                                                  │
│  Components (built-in):                          │
│  ├─ ESP8266-12E (same as 12F!)                   │
│  ├─ AMS1117-3.3V (same as yours!)                │
│  ├─ CP2102 USB-Serial (for programming)          │
│  ├─ Voltage regulator 5V→3.3V                    │
│  └─ Flash & Reset buttons                        │
│                                                  │
│  Your additions:                                 │
│  ├─ Relay Module (simulates TRIAC)               │
│  ├─ LED (simulates load)                         │
│  └─ Wires connecting GPIO5, 3V3, GND             │
│                                                  │
└──────────────────────────────────────────────────┘
```

**Key Insight:**

- **Core logic is IDENTICAL!**
- Wokwi board = Convenience wrapper around ESP8266-12E/F
- Your PCB = Custom implementation of same chip
- **Code transfers directly with zero changes!**

---

## 📱 Practical Workflow

### Recommended Development Flow:

```
┌─────────────────────────────────────────────────┐
│ PHASE 1: Wokwi Simulation (This Week)          │
├─────────────────────────────────────────────────┤
│ 1. Open: https://wokwi.com/projects/new/esp8266 │
│ 2. Build circuit with Relay module              │
│ 3. Write and test firmware                      │
│ 4. Validate WiFi, web server, control logic     │
│ 5. Debug until everything works                 │
│                                                 │
│ Output: Working code, verified logic            │
└─────────────────────────────────────────────────┘
                    ↓
┌─────────────────────────────────────────────────┐
│ PHASE 2: Physical Prototype (Week 2-3)         │
├─────────────────────────────────────────────────┤
│ 1. Order components (ESP8266-12F, etc.)         │
│ 2. Copy code from Wokwi (no changes needed!)    │
│ 3. Upload to real ESP8266-12F via FTDI          │
│ 4. Test with breadboard first                   │
│ 5. Then test with real TRIAC circuit (careful!) │
│                                                 │
│ Output: Working physical prototype              │
└─────────────────────────────────────────────────┘
```

---

## ✅ Final Recommendation

**For your SWESU project:**

1. **Use ESP8266 in Wokwi** (NodeMCU variant)

   - Direct link: https://wokwi.com/projects/new/esp8266
   - 100% compatible with your ESP8266-12F design

2. **Pin mapping is direct:**

   - Wokwi D1 (GPIO5) = Your GPIO5 = TRIAC control
   - Wokwi D2 (GPIO4) = Your GPIO4 = Status LED
   - Wokwi 3V3/GND = Your 3V3/GND

3. **Code transfers with ZERO changes:**

   ```cpp
   // This exact code works in:
   // ✅ Wokwi ESP8266 simulation
   // ✅ Your physical ESP8266-12F hardware

   #include <ESP8266WiFi.h>
   #include <ESP8266WebServer.h>
   #define RELAY_PIN 5  // GPIO5
   // ... rest of your code
   ```

4. **Workflow:**
   - Test in Wokwi → Copy code → Upload to hardware → Done!

---

## 🔗 Quick Links

- **Start ESP8266 Project:** [https://wokwi.com/projects/new/esp8266](https://wokwi.com/projects/new/esp8266)
- **ESP8266 Examples:** [https://wokwi.com/projects?tag=esp8266](https://wokwi.com/projects?tag=esp8266)
- **ESP8266 Docs:** [https://docs.wokwi.com/parts/wokwi-esp8266](https://docs.wokwi.com/parts/wokwi-esp8266)

---

**Created:** January 13, 2026  
**For:** SWESU MVP Development  
**Hardware:** ESP8266-12F PCB Design  
**Simulator:** Wokwi ESP8266 NodeMCU
