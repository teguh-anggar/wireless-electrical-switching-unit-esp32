# SWESU Wokwi Simulation Guide

## 🎮 Getting Started with Wokwi Simulator

**Wokwi** is a free online electronics simulator perfect for testing ESP8266/ESP32 projects before building physical prototypes.

**Website:** [https://wokwi.com](https://wokwi.com)

---

## 🚀 Quick Start (5 Minutes)

### Step 1: Create Your First Simulation

1. **Go to** [https://wokwi.com/projects/new/esp8266](https://wokwi.com/projects/new/esp8266)
2. You'll see:
   - **Code editor** (left) - Write your Arduino/C++ code
   - **Diagram editor** (right) - Visual circuit builder
   - **Serial monitor** (bottom) - View debug output

### Step 2: Basic Blink Test

The default project has a simple blink sketch. Click **▶️ Start Simulation** to test it.

```cpp
// Default Wokwi ESP8266 code
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
```

**Result:** The built-in LED on the ESP8266 will blink every second! 🎉

---

## 🔧 Building Your SWESU Simulation

### Step-by-Step: Simulating Relay Control

**Goal:** Control a relay (simulating your TRIAC) via WiFi web server

#### 1. **Start New ESP8266 Project**

Go to: [https://wokwi.com/projects/new/esp8266](https://wokwi.com/projects/new/esp8266)

#### 2. **Add Components to Diagram**

Click the **"+"** button in the diagram area and add:

| Component           | Purpose                   | How to Add        |
| ------------------- | ------------------------- | ----------------- |
| **Relay Module**    | Simulates TRIAC switching | Search "relay"    |
| **LED**             | Visual load indicator     | Search "led"      |
| **Resistor (220Ω)** | LED current limiting      | Search "resistor" |

**Wiring Connections:**

```
ESP8266 NodeMCU         Relay Module
─────────────────       ────────────
D1 (GPIO5)    ────────► IN
3V3           ────────► VCC
GND           ────────► GND

Relay Module            LED Circuit
────────────            ───────────
COM           ────────► LED Anode (+)
NO (Normally Open) ───► LED Cathode (-) via 220Ω resistor to GND
```

#### 3. **Paste This Basic Control Code**

```cpp
#define RELAY_PIN 5  // GPIO5 = D1 on NodeMCU

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Relay OFF initially

  Serial.println("SWESU Relay Test Ready");
}

void loop() {
  // Turn ON for 2 seconds
  Serial.println("Relay ON");
  digitalWrite(RELAY_PIN, HIGH);
  delay(2000);

  // Turn OFF for 2 seconds
  Serial.println("Relay OFF");
  digitalWrite(RELAY_PIN, LOW);
  delay(2000);
}
```

#### 4. **Run Simulation**

Click **▶️ Start Simulation**

**Expected behavior:**

- Serial monitor shows "Relay ON" / "Relay OFF"
- LED blinks every 2 seconds
- Relay module clicks (visual animation)

---

## 🌐 Step 2: Adding WiFi Web Server

**Goal:** Control relay via web page (like your MVP requirement)

### Full Code Example

```cpp
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// WiFi Credentials (Wokwi has built-in WiFi simulation)
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Web server on port 80
ESP8266WebServer server(80);

// Pin definitions
#define RELAY_PIN 5  // D1 on NodeMCU

// Relay state
bool relayState = false;

// HTML for web interface
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <title>SWESU Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial; text-align: center; margin-top: 50px; }
    .button {
      padding: 20px 40px;
      font-size: 24px;
      margin: 10px;
      border: none;
      border-radius: 8px;
      cursor: pointer;
    }
    .on { background-color: #4CAF50; color: white; }
    .off { background-color: #f44336; color: white; }
  </style>
</head>
<body>
  <h1>🔌 SWESU MVP Control</h1>
  <p>Relay Status: <span id="status">Loading...</span></p>
  <button class="button on" onclick="toggleRelay('on')">Turn ON</button>
  <button class="button off" onclick="toggleRelay('off')">Turn OFF</button>

  <script>
    function toggleRelay(state) {
      fetch('/control?state=' + state)
        .then(response => response.text())
        .then(data => {
          document.getElementById('status').innerText = data;
        });
    }

    // Update status on load
    fetch('/status')
      .then(response => response.text())
      .then(data => document.getElementById('status').innerText = data);
  </script>
</body>
</html>
)=====";

void handleRoot() {
  server.send(200, "text/html", MAIN_page);
}

void handleControl() {
  String state = server.arg("state");

  if (state == "on") {
    digitalWrite(RELAY_PIN, HIGH);
    relayState = true;
    server.send(200, "text/plain", "ON");
  } else if (state == "off") {
    digitalWrite(RELAY_PIN, LOW);
    relayState = false;
    server.send(200, "text/plain", "OFF");
  }
}

void handleStatus() {
  server.send(200, "text/plain", relayState ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/control", handleControl);
  server.on("/status", handleStatus);

  server.begin();
  Serial.println("🌐 Web server started");
}

void loop() {
  server.handleClient();
}
```

### How to Test in Wokwi

1. **Paste the code** into Wokwi editor
2. **Start simulation** (▶️ button)
3. **Wait for WiFi connection** - Serial monitor will show IP address
4. **Click the WiFi icon** at the top-right of the browser area
5. **Navigate to the IP address** shown (usually `192.168.1.1` in Wokwi)
6. **Click ON/OFF buttons** to control the relay!

---

## 🎨 Advanced: Custom Diagram.json

Wokwi stores circuit configuration in `diagram.json`. Here's the complete file for SWESU simulation:

```json
{
  "version": 1,
  "author": "Teguh Anggar",
  "editor": "wokwi",
  "parts": [
    {
      "type": "wokwi-esp8266-nodemcu",
      "id": "esp",
      "top": 0,
      "left": 0,
      "attrs": {}
    },
    {
      "type": "wokwi-relay-module",
      "id": "relay1",
      "top": 100,
      "left": 200,
      "attrs": {}
    },
    {
      "type": "wokwi-led",
      "id": "led1",
      "top": 100,
      "left": 400,
      "attrs": { "color": "red" }
    },
    {
      "type": "wokwi-resistor",
      "id": "r1",
      "top": 150,
      "left": 400,
      "attrs": { "value": "220" }
    }
  ],
  "connections": [
    ["esp:D1", "relay1:IN", "green", ["v0"]],
    ["esp:3V3", "relay1:VCC", "red", ["v0"]],
    ["esp:GND.1", "relay1:GND", "black", ["v0"]],
    ["relay1:COM", "led1:A", "orange", ["v0"]],
    ["led1:C", "r1:1", "blue", ["v0"]],
    ["r1:2", "esp:GND.2", "black", ["v0"]]
  ],
  "dependencies": {}
}
```

---

## 📱 Step 3: Simulating WiFi Configuration Portal

**Goal:** Add AP mode for initial WiFi setup (like your MVP requirement)

### Using WiFiManager Library

```cpp
#include <ESP8266WiFi.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager
#include <ESP8266WebServer.h>

#define RELAY_PIN 5

ESP8266WebServer server(80);
WiFiManager wifiManager;
bool relayState = false;

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>SWESU Control</h1>";
  html += "<p>Status: " + String(relayState ? "ON" : "OFF") + "</p>";
  html += "<a href='/on'><button>Turn ON</button></a>";
  html += "<a href='/off'><button>Turn OFF</button></a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleOn() {
  digitalWrite(RELAY_PIN, HIGH);
  relayState = true;
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleOff() {
  digitalWrite(RELAY_PIN, LOW);
  relayState = false;
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);

  // Auto-connect to WiFi or start config portal
  wifiManager.autoConnect("SWESU-Setup");

  Serial.println("Connected to WiFi!");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.begin();
}

void loop() {
  server.handleClient();
}
```

**Note:** Wokwi doesn't fully support WiFiManager's captive portal, but you can test the logic.

---

## ⏰ Step 4: Adding Scheduler (Timer Function)

**Goal:** Automatically turn relay ON/OFF at scheduled times

### Code with NTP Time Sync

```cpp
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600); // UTC+7 for Indonesia

#define RELAY_PIN 5

// Schedule: Turn ON at 18:00, OFF at 22:00
int onHour = 18;
int offHour = 22;

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);

  WiFi.begin("Wokwi-GUEST", "");
  while (WiFi.status() != WL_CONNECTED) delay(500);

  timeClient.begin();
  Serial.println("NTP Time sync started");
}

void loop() {
  timeClient.update();

  int currentHour = timeClient.getHours();

  if (currentHour == onHour) {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Scheduled ON at " + timeClient.getFormattedTime());
  } else if (currentHour == offHour) {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Scheduled OFF at " + timeClient.getFormattedTime());
  }

  delay(60000); // Check every minute
}
```

**Testing in Wokwi:**

- Wokwi simulates NTP server
- Change `onHour` and `offHour` to current time +1 minute to test quickly

---

## 🔍 Debugging in Wokwi

### Using Serial Monitor

```cpp
void setup() {
  Serial.begin(115200);
  Serial.println("\n\n=== SWESU Starting ===");
  Serial.print("Free Heap: ");
  Serial.println(ESP.getFreeHeap());
}

void loop() {
  Serial.print("Relay state: ");
  Serial.println(digitalRead(RELAY_PIN) ? "ON" : "OFF");
  delay(1000);
}
```

**View output:** Click **Serial Monitor** tab at bottom of Wokwi

### Common Issues & Fixes

| Problem              | Solution                                 |
| -------------------- | ---------------------------------------- |
| WiFi won't connect   | Use `Wokwi-GUEST` with empty password    |
| Code won't compile   | Add missing libraries in `libraries.txt` |
| Relay doesn't switch | Check pin number (D1 = GPIO5)            |
| Web page won't load  | Check IP address in Serial Monitor       |

---

## 📦 Adding External Libraries

Wokwi supports Arduino libraries via `libraries.txt` file.

**How to add:**

1. Click **"+ New File"** button
2. Name it `libraries.txt`
3. Add library names (one per line):

```
ESPAsyncWebServer
ArduinoJson
NTPClient
WiFiManager
```

**Wokwi will auto-install** them when simulation starts!

---

## 🎯 Your SWESU MVP Testing Workflow

### Phase 1: Logic Testing (Week 1)

```
Wokwi Simulation Tasks:
├─ ✅ Test basic GPIO control (relay on/off)
├─ ✅ Test WiFi connection
├─ ✅ Test web server
├─ ✅ Test scheduler logic
└─ ✅ Test error handling
```

**Advantages:**

- Find bugs **before buying components**
- Test edge cases safely (e.g., WiFi disconnection)
- Experiment with different pins/configurations

### Phase 2: Real Hardware (Week 2+)

Once code works in Wokwi:

1. **Copy final code** from Wokwi
2. **Upload to physical ESP8266** via Arduino IDE
3. **Test with real relay** (safely!)

---

## 🔗 Pre-built SWESU Examples on Wokwi

I recommend these public Wokwi projects to study:

| Project                | Link                                                                    | What to Learn      |
| ---------------------- | ----------------------------------------------------------------------- | ------------------ |
| **ESP8266 Web Server** | [wokwi.com/projects/...](https://wokwi.com/projects/320964045018341972) | Basic HTTP control |
| **Relay Control**      | [wokwi.com/projects/...](https://wokwi.com/projects/323706614646309460) | Switching loads    |
| **WiFi Config Portal** | Search "ESP8266 WiFiManager"                                            | AP mode setup      |

---

## 🎓 Next Steps: Advanced Features

### 1. **MQTT Control** (for Home Assistant integration)

```cpp
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient mqtt(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, "swesu/relay") == 0) {
    if (payload[0] == '1') digitalWrite(RELAY_PIN, HIGH);
    else digitalWrite(RELAY_PIN, LOW);
  }
}

void setup() {
  mqtt.setServer("mqtt.broker.com", 1883);
  mqtt.setCallback(callback);
  mqtt.subscribe("swesu/relay");
}
```

### 2. **OTA Updates** (firmware update over WiFi)

```cpp
#include <ArduinoOTA.h>

void setup() {
  ArduinoOTA.setHostname("SWESU-01");
  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();
}
```

### 3. **Persistent Settings** (save WiFi credentials)

```cpp
#include <EEPROM.h>

void saveSettings() {
  EEPROM.begin(512);
  EEPROM.put(0, onHour);
  EEPROM.put(4, offHour);
  EEPROM.commit();
}

void loadSettings() {
  EEPROM.begin(512);
  EEPROM.get(0, onHour);
  EEPROM.get(4, offHour);
}
```

---

## 📊 Comparison: Wokwi vs Real Hardware

| Feature        | Wokwi Simulation        | Real Hardware                  |
| -------------- | ----------------------- | ------------------------------ |
| **Cost**       | 🟢 Free                 | 🟡 ~IDR 50K per unit           |
| **Risk**       | 🟢 No damage possible   | 🔴 Can burn components         |
| **Speed**      | 🟢 Instant code changes | 🟡 Must re-upload              |
| **Components** | 🟡 Limited library      | 🟢 Any component               |
| **220V AC**    | 🔴 Cannot simulate      | 🟢 Real-world testing          |
| **Learning**   | 🟢 Great for beginners  | 🟢 Essential for final product |

**Recommendation:** Use Wokwi for Weeks 1-2, then move to real hardware for Weeks 3+

---

## ✅ Wokwi Pre-flight Checklist

Before moving to physical hardware, verify in Wokwi:

- [ ] WiFi connects successfully
- [ ] Web page loads and renders correctly
- [ ] ON/OFF buttons control relay
- [ ] Scheduler triggers at correct time
- [ ] Serial monitor shows no errors
- [ ] Memory usage is acceptable (<80% heap)
- [ ] Code handles WiFi disconnection gracefully
- [ ] Settings persist across reboots (EEPROM)

---

## 🎬 Quick Start Template

**Copy-paste ready code for your first Wokwi test:**

```cpp
// SWESU MVP - Wokwi Test v0.1
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
ESP8266WebServer server(80);
#define RELAY_PIN 5

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  Serial.println("\nWiFi: " + WiFi.localIP().toString());

  server.on("/", []() {
    server.send(200, "text/html",
      "<h1>SWESU</h1><a href='/on'>[ON]</a> <a href='/off'>[OFF]</a>");
  });

  server.on("/on", []() {
    digitalWrite(RELAY_PIN, HIGH);
    server.send(200, "text/plain", "ON");
  });

  server.on("/off", []() {
    digitalWrite(RELAY_PIN, LOW);
    server.send(200, "text/plain", "OFF");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
```

**Test link:** [Wokwi ESP8266 New Project](https://wokwi.com/projects/new/esp8266)

---

## 📚 Resources

- **Wokwi Docs:** [https://docs.wokwi.com](https://docs.wokwi.com)
- **ESP8266 Reference:** [https://arduino-esp8266.readthedocs.io](https://arduino-esp8266.readthedocs.io)
- **Wokwi ESP8266 Guide:** [https://docs.wokwi.com/guides/esp8266](https://docs.wokwi.com/guides/esp8266)

---

**Created:** January 13, 2026  
**Author:** Teguh Anggar  
**Project:** SWESU MVP Development  
**Status:** ✅ Ready to Use
