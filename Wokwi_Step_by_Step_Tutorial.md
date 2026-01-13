# 🎓 SWESU Wokwi Tutorial - Your First Simulation

## Step-by-Step Walkthrough (15 Minutes)

**Goal:** Build and test a working WiFi-controlled relay in Wokwi simulator

**What you'll learn:**

- ✅ How to use Wokwi interface
- ✅ Build a circuit visually
- ✅ Write ESP8266 code
- ✅ Test web control
- ✅ Debug with Serial Monitor

---

## 📍 STEP 1: Open Wokwi and Create Project (2 min)

### Actions:

1. **Open your browser** and go to: [https://wokwi.com](https://wokwi.com)
2. **Click "Start Simulating"** button (no account needed!)
3. **Select "ESP32"** from the board options
   - **Note:** ESP32 is recommended because it's widely available in Wokwi
   - **Don't worry:** ESP32 and ESP8266 are very similar - your code will work with both!
   - For your real hardware MVP, you'll use ESP8266 as planned
4. You'll see a blank project with:
   - 🖥️ **Code editor** (left side)
   - 🔌 **Circuit diagram** (right side)
   - 📊 **Serial monitor** (bottom, hidden initially)

### What you should see:

```
┌─────────────────────────────────────────────────────────────┐
│  Wokwi - Online Simulator                                   │
├─────────────┬───────────────────────────────────────────────┤
│             │                                               │
│   CODE      │          CIRCUIT DIAGRAM                      │
│   EDITOR    │      (ESP32 Dev Module)                       │
│             │                                               │
│             │                                               │
│   sketch.   │           [ESP32]                             │
│   ino       │                                               │
│             │                                               │
│             │     ▶️ Start Simulation (green button)        │
└─────────────┴───────────────────────────────────────────────┘
```

**✅ Checkpoint:** You should see an ESP32 board in the diagram

**💡 Quick Tip:** The principles you learn here apply directly to ESP8266! The only differences:

- Pin names might vary slightly (D1 vs D5)
- ESP32 has more pins and features
- Code libraries are nearly identical

---

## 📍 STEP 2: Add Components to Circuit (3 min)

### Actions:

1. **Click the "+" (Plus) button** in the diagram area (top-right of circuit panel)
2. **Search and add these components one by one:**

#### Component 1: Relay Module

- Type: `relay` in search box
- Select: **"Relay Module (1 Channel)"**
- Click to place it to the right of ESP8266

#### Component 2: LED (Red)

- Click "+" again
- Type: `led`
- Select: **"Red LED"**
- Place it below the relay module

#### Component 3: Resistor

- Click "+" again
- Type: `resistor`
- Select: **"Resistor"**
- Place it below the LED
- **Click on the resistor** and change its value to **220** in the properties panel

### What you should have now:

```
Circuit Layout:
┌──────────────────┐      ┌──────────────┐
│                  │      │ Relay Module │
│   ESP8266        │      │  VCC  IN  GND│
│   NodeMCU        │      │  COM  NO  NC │
│                  │      └──────────────┘
│  D1 D2 D3...     │
│  3V3 GND         │           { LED }
└──────────────────┘              │
                               [220Ω]
```

**✅ Checkpoint:** You have 4 components: ESP32, Relay, LED, Resistor

---

## 📍 STEP 3: Wire the Components (5 min)

**Important:** Click and drag from one pin to another to create wires

**Note:** Your EasyEDA design uses **GPIO13** for TRIAC control. We'll use the same pin in Wokwi!

### ESP32 Wokwi Pin Layout Reference:

**LEFT SIDE (top to bottom):**

```
3V3, EN, VP, VN, 34, 35, 32, 33, 25, 26, 27, 14, 12, GND, 13, D2, D3, CMD, 5V
```

**GPIO13 is on LEFT side, 5th pin from bottom (between GND above and D2 below)**

---

### Wire 1: Control Signal (ESP32 → Relay)

- **From:** ESP32 pin labeled **"13"** (LEFT side, 5th from bottom)
- **To:** Relay module pin labeled **"IN"**
- Color will be auto-assigned (usually green)
- **This is the control signal** that tells the relay when to switch

### Wire 2: Power to Relay Module (ESP32 → Relay)

- **From:** ESP32 pin labeled **"5V"** (LEFT side, very bottom)
- **To:** Relay module pin labeled **"VCC"**
- Wokwi will color it red automatically

### Wire 3: Ground to Relay (ESP32 → Relay)

- **From:** ESP32 pin labeled **"GND"** (LEFT side, 6th from bottom, above "13")
- **To:** Relay module pin labeled **"GND"**
- Should be black/dark colored
- **This completes the power circuit for the relay coil**

### Wire 4: Power Source for Load (ESP32 → Relay)

- **From:** ESP32 pin labeled **"3V3"** (LEFT side, very top)
- **To:** Relay pin labeled **"NO"** (Normally Open contact)
- **This is what powers your "load" (the LED in this case)**

### Wire 5: Relay Common to LED (Relay → LED)

- **From:** Relay pin labeled **"COM"** (Common contact)
- **To:** LED **anode (+)** (longer leg, marked with +)
- **When relay activates, it connects NO to COM, powering the LED**

### Wire 6: LED to Resistor (LED → Resistor)

- **From:** LED **cathode (-)** (shorter leg)
- **To:** One end of the **resistor (220Ω)**
- **Current limiting for the LED**

### Wire 7: Resistor to Ground (Resistor → ESP32)

- **From:** Other end of **resistor**
- **To:** **GND** on ESP32 (RIGHT side, top or 7th from top)
- **Completes the circuit**

### Complete Wiring Diagram:

```
         ESP32 Wokwi Board
    ┌────────────────────────┐
    │  LEFT          RIGHT   │
    │  3V3 ─────────────────────────┐ (LED power via relay)
    │  ...            ...    │      │
    │  GND ──────────────────────► Relay GND
    │  13  ──────────────────────► Relay IN  ⭐ GPIO13
    │  ...            ...    │
    │  5V  ──────────────────────► Relay VCC
    └────────────────────────┘
                                    │
                              ┌─────┴─────┐
                              │   Relay   │
                              │  NO ──────┼───┐ (from 3V3)
                              │  COM ─────┼───┼──► LED+
                              └───────────┘   │
                                              │
                                           [ LED ]
                                              │
                                           [220Ω]
                                              │
                                            GND

How it works:
1. GPIO13 sends control signal to relay IN pin
2. When HIGH, relay energizes and connects NO to COM
3. Current flows: 3V3 → NO → COM → LED+ → LED- → Resistor → GND
4. LED lights up! 💡
```

**Pin Location Summary (LEFT side, from bottom):**

- **5V** = 1st from bottom (Relay VCC)
- **13** = 5th from bottom (Relay IN) ⭐
- **GND** = 6th from bottom (Relay GND)
- **3V3** = Very top (LED power via relay NO)

**Pro Tips:**

- ✨ **Right-click a wire** to delete it if you make a mistake
- ✨ **Click and drag components** to reposition them
- ✨ **Zoom in/out** with mouse wheel for better precision

**✅ Checkpoint:** All 7 wires connected, LED circuit complete

---

## 📍 STEP 4: Write the Control Code (3 min)

### Actions:

1. **Click on the code editor** (left side)
2. **Delete** all existing code
3. **Copy and paste** this complete code:

```cpp
// SWESU MVP - Basic Relay Control
// Version: 0.1 - Wokwi Test

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// ============ CONFIGURATION ============
#define RELAY_PIN 5          // D1 on NodeMCU = GPIO5
#define LED_STATUS 2         // Built-in LED (optional)

const char* ssid = "Wokwi-GUEST";      // Wokwi default WiFi
const char* password = "";              // No password needed in Wokwi

// ============ GLOBALS ============
ESP8266WebServer server(80);
bool relayState = false;

// ============ HTML WEB PAGE ============
const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>SWESU Control Panel</title>
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body {
      font-family: 'Segoe UI', Arial, sans-serif;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      display: flex;
      justify-content: center;
      align-items: center;
      min-height: 100vh;
      padding: 20px;
    }
    .container {
      background: white;
      border-radius: 20px;
      box-shadow: 0 20px 60px rgba(0,0,0,0.3);
      padding: 40px;
      max-width: 400px;
      width: 100%;
      text-align: center;
    }
    h1 {
      color: #333;
      margin-bottom: 10px;
      font-size: 28px;
    }
    .subtitle {
      color: #888;
      font-size: 14px;
      margin-bottom: 30px;
    }
    .status {
      background: #f0f0f0;
      border-radius: 10px;
      padding: 20px;
      margin-bottom: 30px;
    }
    .status-label {
      color: #666;
      font-size: 12px;
      text-transform: uppercase;
      letter-spacing: 1px;
      margin-bottom: 10px;
    }
    .status-value {
      font-size: 36px;
      font-weight: bold;
    }
    .status-on { color: #10b981; }
    .status-off { color: #ef4444; }
    .buttons {
      display: flex;
      gap: 15px;
    }
    button {
      flex: 1;
      padding: 18px;
      border: none;
      border-radius: 12px;
      font-size: 16px;
      font-weight: bold;
      cursor: pointer;
      transition: all 0.3s ease;
      text-transform: uppercase;
      letter-spacing: 1px;
    }
    .btn-on {
      background: linear-gradient(135deg, #10b981, #059669);
      color: white;
    }
    .btn-on:hover {
      transform: translateY(-2px);
      box-shadow: 0 10px 20px rgba(16, 185, 129, 0.3);
    }
    .btn-off {
      background: linear-gradient(135deg, #ef4444, #dc2626);
      color: white;
    }
    .btn-off:hover {
      transform: translateY(-2px);
      box-shadow: 0 10px 20px rgba(239, 68, 68, 0.3);
    }
    .footer {
      margin-top: 30px;
      color: #999;
      font-size: 12px;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>🔌 SWESU</h1>
    <div class="subtitle">Smart Wireless Electrical Switching Unit</div>

    <div class="status">
      <div class="status-label">Current Status</div>
      <div class="status-value" id="status">Loading...</div>
    </div>

    <div class="buttons">
      <button class="btn-on" onclick="control('on')">⚡ Turn ON</button>
      <button class="btn-off" onclick="control('off')">⭕ Turn OFF</button>
    </div>

    <div class="footer">
      MVP v0.1 | ESP8266 | Wokwi Simulation
    </div>
  </div>

  <script>
    function control(action) {
      fetch('/control?state=' + action)
        .then(response => response.text())
        .then(data => {
          updateStatus(data);
        })
        .catch(error => {
          alert('Error: ' + error);
        });
    }

    function updateStatus(state) {
      const statusEl = document.getElementById('status');
      if (state === 'ON') {
        statusEl.textContent = 'ON';
        statusEl.className = 'status-value status-on';
      } else {
        statusEl.textContent = 'OFF';
        statusEl.className = 'status-value status-off';
      }
    }

    function getStatus() {
      fetch('/status')
        .then(response => response.text())
        .then(data => updateStatus(data));
    }

    // Update status every 2 seconds
    setInterval(getStatus, 2000);
    getStatus(); // Initial load
  </script>
</body>
</html>
)=====";

// ============ WEB SERVER HANDLERS ============
void handleRoot() {
  server.send(200, "text/html", webpage);
}

void handleControl() {
  if (server.hasArg("state")) {
    String state = server.arg("state");

    if (state == "on") {
      digitalWrite(RELAY_PIN, HIGH);
      relayState = true;
      Serial.println("✅ Relay turned ON");
      server.send(200, "text/plain", "ON");
    }
    else if (state == "off") {
      digitalWrite(RELAY_PIN, LOW);
      relayState = false;
      Serial.println("⭕ Relay turned OFF");
      server.send(200, "text/plain", "OFF");
    }
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void handleStatus() {
  server.send(200, "text/plain", relayState ? "ON" : "OFF");
}

// ============ SETUP ============
void setup() {
  // Initialize Serial
  Serial.begin(115200);
  delay(100);
  Serial.println("\n\n");
  Serial.println("╔════════════════════════════════════════╗");
  Serial.println("║   SWESU MVP v0.1 - Wokwi Simulation   ║");
  Serial.println("╚════════════════════════════════════════╝");

  // Initialize GPIO
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Start with relay OFF

  Serial.println("\n[1/3] Initializing GPIO...");
  Serial.println("      ✓ Relay pin: GPIO5 (D1)");

  // Connect to WiFi
  Serial.println("\n[2/3] Connecting to WiFi...");
  Serial.print("      Network: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_STATUS, !digitalRead(LED_STATUS)); // Blink during connection
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n      ✓ WiFi Connected!");
    Serial.print("      IP Address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(LED_STATUS, LOW); // LED off when connected
  } else {
    Serial.println("\n      ✗ WiFi Connection Failed!");
    return;
  }

  // Start Web Server
  Serial.println("\n[3/3] Starting Web Server...");
  server.on("/", handleRoot);
  server.on("/control", handleControl);
  server.on("/status", handleStatus);
  server.begin();

  Serial.println("      ✓ Server started on port 80");
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║         🎉 SYSTEM READY! 🎉           ║");
  Serial.println("╚════════════════════════════════════════╝");
  Serial.print("\n👉 Open browser: http://");
  Serial.println(WiFi.localIP());
  Serial.println("\n");
}

// ============ MAIN LOOP ============
void loop() {
  server.handleClient();
}
```

**What this code does:**

- ✅ Connects to WiFi (Wokwi's simulated network)
- ✅ Creates a beautiful web control panel
- ✅ Controls relay via ON/OFF buttons
- ✅ Shows real-time status
- ✅ Prints detailed debug info to Serial Monitor

**✅ Checkpoint:** Code is pasted in editor, no red errors visible

---

## 📍 STEP 5: Run the Simulation (2 min)

### Actions:

1. **Click the green "▶️ Start Simulation" button** at the top
2. **Wait 3-5 seconds** for ESP8266 to boot
3. **Look at the bottom panel** - Serial Monitor should appear automatically
4. **Watch the boot sequence** in Serial Monitor

### What you should see in Serial Monitor:

```
╔════════════════════════════════════════╗
║   SWESU MVP v0.1 - Wokwi Simulation   ║
╚════════════════════════════════════════╝

[1/3] Initializing GPIO...
      ✓ Relay pin: GPIO5 (D1)

[2/3] Connecting to WiFi...
      Network: Wokwi-GUEST
..........
      ✓ WiFi Connected!
      IP Address: 192.168.1.1

[3/3] Starting Web Server...
      ✓ Server started on port 80

╔════════════════════════════════════════╗
║         🎉 SYSTEM READY! 🎉           ║
╚════════════════════════════════════════╝

👉 Open browser: http://192.168.1.1
```

**✅ Checkpoint:** You see "SYSTEM READY!" and an IP address (usually 192.168.1.1)

---

## 📍 STEP 6: Open the Web Interface (2 min)

### Actions:

1. **Look for the WiFi/Network icon** in the Wokwi simulator (top-right area of circuit)
2. **Click it** - a small browser window will appear
3. **Type the IP address** shown in Serial Monitor (e.g., `192.168.1.1`)
4. **Press Enter**

### What you should see:

**A beautiful purple gradient web page with:**

- 🔌 **"SWESU" heading**
- 📊 **Status display** (showing "OFF" in red)
- 🟢 **Green "Turn ON" button**
- 🔴 **Red "Turn OFF" button**

**✅ Checkpoint:** Web page loaded successfully

---

## 📍 STEP 7: Test the Control! (2 min)

### Actions:

1. **Click the "⚡ Turn ON" button**
2. **Watch what happens:**

   - ✨ Status changes to "ON" (green)
   - ✨ Relay module clicks (visual animation)
   - ✨ LED lights up! 💡
   - ✨ Serial Monitor shows: "✅ Relay turned ON"

3. **Click the "⭕ Turn OFF" button**
4. **Watch what happens:**
   - ✨ Status changes to "OFF" (red)
   - ✨ Relay de-energizes
   - ✨ LED turns off
   - ✨ Serial Monitor shows: "⭕ Relay turned OFF"

### Try This:

- Click ON and OFF multiple times
- Watch the relay animation
- Notice the LED turning on/off
- Check Serial Monitor for debug messages

**✅ Checkpoint:** LED responds to button clicks! 🎉

---

## 🎊 CONGRATULATIONS! You've Successfully:

✅ Built your first IoT circuit in Wokwi  
✅ Programmed ESP8266 to control a relay  
✅ Created a WiFi-controlled web interface  
✅ Tested remote ON/OFF control  
✅ Debugged using Serial Monitor

---

## 🔍 Understanding What Happened

### The Circuit:

```
ESP8266 sends 3.3V signal on GPIO5 (D1)
    ↓
Relay module activates (coil energizes)
    ↓
Relay switches COM to NO (normally open contact)
    ↓
Current flows: 3V3 → NO → COM → LED → Resistor → GND
    ↓
LED lights up! 💡
```

### The Software:

```
Browser sends HTTP request: /control?state=on
    ↓
ESP8266 receives request
    ↓
Code executes: digitalWrite(RELAY_PIN, HIGH)
    ↓
GPIO5 outputs 3.3V
    ↓
Relay activates → LED turns ON
```

---

## 🚀 Next Challenges (Optional)

Want to learn more? Try these modifications:

### Challenge 1: Add a Timer

**Goal:** Automatically turn OFF after 10 seconds

**Hint:** Add this to `handleControl()`:

```cpp
if (state == "on") {
  digitalWrite(RELAY_PIN, HIGH);
  delay(10000);  // Wait 10 seconds
  digitalWrite(RELAY_PIN, LOW);
}
```

### Challenge 2: Add Second LED

**Goal:** Show WiFi connection status with a blue LED

**Steps:**

1. Add blue LED to circuit
2. Connect to GPIO4 (D2)
3. Turn it ON when WiFi connects

### Challenge 3: Add Schedule

**Goal:** Turn ON at 18:00, OFF at 22:00

**Library needed:** NTPClient (for time)

---

## 📊 Troubleshooting Guide

| Problem                 | Solution                                              |
| ----------------------- | ----------------------------------------------------- |
| **"Compilation error"** | Check for typos in code, make sure all { } are closed |
| **LED doesn't light**   | Check wiring - verify resistor is 220Ω, not 220kΩ     |
| **Web page won't load** | Check IP address in Serial Monitor, try 192.168.1.1   |
| **WiFi won't connect**  | Make sure SSID is "Wokwi-GUEST" (case-sensitive)      |
| **Relay doesn't click** | Verify D1 is connected to relay IN pin                |

---

## 📝 What You Learned

**Hardware Skills:**

- ✅ How relays work
- ✅ Circuit building basics
- ✅ LED current limiting (resistors)
- ✅ Pin connections (GPIO, power, ground)

**Software Skills:**

- ✅ ESP8266 WiFi setup
- ✅ Web server creation
- ✅ HTML/CSS interface design
- ✅ HTTP request handling
- ✅ Serial debugging

**Development Skills:**

- ✅ Using online simulators
- ✅ Rapid prototyping
- ✅ Testing before hardware
- ✅ Debug techniques

---

## 🎯 Apply to Your MVP

**You can now:**

1. ✅ Move to physical hardware with confidence
2. ✅ Modify the code for your specific needs
3. ✅ Add scheduling, MQTT, or other features
4. ✅ Test new ideas safely in Wokwi first

**Your homework:**

- Try adding a second relay channel
- Experiment with different web UI designs
- Test error handling (WiFi disconnection)
- Add persistent settings (EEPROM)

---

## 📚 Next Steps

1. **Save your Wokwi project** (click "Save" button, create free account)
2. **Share the link** with team members for feedback
3. **Modify and experiment** - Wokwi autosaves!
4. **When ready:** Copy code to Arduino IDE → Upload to real ESP8266

---

**🎓 Tutorial Complete!**  
**Time invested:** ~15 minutes  
**Skills gained:** Foundation for entire MVP project!  
**Money saved:** Testing before buying = fewer mistakes

**Ready for Phase 2?** See `Wokwi_Simulation_Guide.md` for advanced features!

---

**Created:** January 13, 2026  
**Tutorial Level:** Beginner  
**Estimated Time:** 15 minutes  
**Prerequisites:** None - complete beginner friendly!
