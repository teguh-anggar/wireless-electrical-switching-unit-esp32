// SWESU MVP - ESP32 Wokwi Simulation
// CORRECTED: Uses GPIO13 to match your EasyEDA schematic!
// Version: 0.2 - Fixed Pin Assignment

#include <WiFi.h>
#include <WebServer.h>

// ============ CONFIGURATION (UPDATED!) ============
#define RELAY_PIN 13         // GPIO13 - Matches your EasyEDA design!
                             // This is pin 7 on ESP8266-12F
#define LED_STATUS 2         // Built-in LED on ESP32

const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ============ GLOBALS ============
WebServer server(80);
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
    .pin-info {
      margin-top: 20px;
      padding: 15px;
      background: #f9fafb;
      border-radius: 8px;
      font-size: 11px;
      color: #666;
      text-align: left;
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
    
    <div class="pin-info">
      <strong>Hardware Info:</strong><br>
      Control Pin: GPIO13 (ESP8266-12F Pin 7)<br>
      Simulation: ESP32 Pin 13<br>
      Target: MOC3041 → BT136 TRIAC
    </div>
    
    <div class="footer">
      MVP v0.2 | GPIO13 Corrected | ESP32 → ESP8266
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
    
    setInterval(getStatus, 2000);
    getStatus();
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
      Serial.println("✅ GPIO13 (Relay) turned ON");
      server.send(200, "text/plain", "ON");
    } 
    else if (state == "off") {
      digitalWrite(RELAY_PIN, LOW);
      relayState = false;
      Serial.println("⭕ GPIO13 (Relay) turned OFF");
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
  Serial.begin(115200);
  delay(100);
  Serial.println("\n\n");
  Serial.println("╔════════════════════════════════════════╗");
  Serial.println("║   SWESU MVP v0.2 - GPIO13 CORRECTED   ║");
  Serial.println("╚════════════════════════════════════════╝");
  
  // Initialize GPIO
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  
  Serial.println("\n[1/3] Initializing GPIO...");
  Serial.println("      ✓ Control pin: GPIO13");
  Serial.println("      ✓ Matches EasyEDA schematic!");
  
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
    digitalWrite(LED_STATUS, !digitalRead(LED_STATUS));
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n      ✓ WiFi Connected!");
    Serial.print("      IP Address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(LED_STATUS, LOW);
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
  Serial.println("\n📌 Pin Configuration:");
  Serial.println("   ESP32 Sim: GPIO13 (pin labeled '13')");
  Serial.println("   ESP8266-12F: GPIO13 (physical pin 7)");
  Serial.println("   Hardware: GPIO13 → MOC3041 → TRIAC");
  Serial.println("\n🔄 To convert to ESP8266 hardware:");
  Serial.println("   1) Change: WiFi.h → ESP8266WiFi.h");
  Serial.println("   2) Change: WebServer.h → ESP8266WebServer.h");
  Serial.println("   3) Change: WebServer → ESP8266WebServer");
  Serial.println("   4) GPIO13 stays the same!");
  Serial.println();
}

// ============ MAIN LOOP ============
void loop() {
  server.handleClient();
}
