/*
 * B2SRGB Controller Firmware - Captive Portal Edition
 * 
 * เวอร์ชั่นอัปเกรด: เมื่อ ESP32 อยู่ในโหมดตั้งค่า (AP Mode)
 * มันจะสร้าง Captive Portal เพื่อให้หน้าต่างตั้งค่าเด้งขึ้นมาบนมือถือโดยอัตโนมัติ
 * 
 * ไลบรารีที่ต้องติดตั้ง:
 * 1. ArduinoJson
 * 2. FastLED
 * 3. ESP32 BLE Arduino
 * (DNSServer และ WebServer เป็นส่วนหนึ่งของ ESP32 core)
 */

//================================================================
// LIBRARIES
//================================================================
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h> // <--- ไลบรารีใหม่สำหรับ Captive Portal
#include <ArduinoJson.h>
#include <FastLED.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "nvs_flash.h" // <--- ไลบรารีสำหรับบันทึกข้อมูล Wi-Fi
#include "nvs.h"
#include "wifi_scanner.h" // <--- WiFi Scanner Feature

//================================================================
// HARDWARE & WIFI CONFIGURATION
//================================================================
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
#define LED_PIN     5
#define NUM_LEDS    1200
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define DEFAULT_BRIGHTNESS 200

//================================================================
// BLUETOOTH LOW ENERGY (BLE) CONFIGURATION
//================================================================
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

//================================================================
// GLOBAL VARIABLES
//================================================================
WebServer server(80);
DNSServer dnsServer; // <--- สร้าง object ของ DNS Server
bool powerState = true;
nvs_handle_t my_handle;
char saved_ssid[32] = "";
char saved_password[64] = "";
char device_name[32] = "B2SRGB"; // ชื่ออุปกรณ์ (จะถูกสร้างอัตโนมัติครั้งแรก)

// Mode and Pattern Variables
String currentMode = "สีเดียว";
String currentPattern = "รุ้งวนลูป";
CRGB currentColor = CRGB::White;
CRGB palette[5] = {CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::Green, CRGB::Blue};
int paletteSize = 5;
int currentSpeed = 50;
int currentBrightness = DEFAULT_BRIGHTNESS;

// Animation Variables
uint8_t gHue = 0;
uint8_t gCurrentPatternNumber = 0;
unsigned long lastUpdate = 0;

//================================================================
// FORWARD DECLARATION & CALLBACKS
//================================================================
void parseAndExecuteCommand(String json);
class MyCharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();
      if (value.length() > 0) {
        parseAndExecuteCommand(value.c_str());
      }
    }
};

//================================================================
// ANIMATION PATTERNS
//================================================================

// Rainbow Cycle Pattern
void rainbowCycle() {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(gHue + (i * 255 / NUM_LEDS), 255, 255);
  }
  FastLED.show();
  gHue++;
}

// Rainbow Chase Pattern
void rainbowChase() {
  static uint8_t startIndex = 0;
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV((startIndex + i * 3) % 255, 255, 255);
  }
  FastLED.show();
  startIndex += 2;
}

// Rainbow Twinkle Pattern
void rainbowTwinkle() {
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = random16(NUM_LEDS);
  leds[pos] = CHSV(random8(), 255, 255);
  FastLED.show();
}

// Fire Pattern
void fireEffect() {
  static byte heat[NUM_LEDS];
  
  // Step 1: Cool down every cell
  for(int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8(heat[i], random8(0, ((55 * 10) / NUM_LEDS) + 2));
  }
  
  // Step 2: Heat from each cell drifts up
  for(int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
  
  // Step 3: Randomly ignite new sparks
  if(random8() < 120) {
    int y = random8(7);
    heat[y] = qadd8(heat[y], random8(160, 255));
  }
  
  // Step 4: Convert heat to LED colors
  for(int j = 0; j < NUM_LEDS; j++) {
    byte colorindex = scale8(heat[j], 240);
    CRGB color = HeatColor(colorindex);
    leds[j] = color;
  }
  
  FastLED.show();
}

// Ocean Wave Pattern
void oceanWave() {
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
  
  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);
  
  uint16_t hue16 = sHue16;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for(uint16_t i = 0; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;
    
    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;
    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV(hue8, sat8, bri8);
    nblend(leds[i], newcolor, 64);
  }
  
  FastLED.show();
}

// Palette Fade
void paletteFade() {
  static uint8_t paletteIndex = 0;
  static uint8_t blendAmount = 0;
  static unsigned long lastChange = 0;
  
  if(millis() - lastChange > 50) {
    CRGB color1 = palette[paletteIndex % paletteSize];
    CRGB color2 = palette[(paletteIndex + 1) % paletteSize];
    
    CRGB blendedColor = blend(color1, color2, blendAmount);
    fill_solid(leds, NUM_LEDS, blendedColor);
    FastLED.show();
    
    blendAmount += 5;
    if(blendAmount == 0) {
      paletteIndex++;
    }
    lastChange = millis();
  }
}

// Palette Strobe
void paletteStrobe() {
  static uint8_t paletteIndex = 0;
  static unsigned long lastChange = 0;
  int strobeDelay = map(currentSpeed, 0, 100, 500, 50);
  
  if(millis() - lastChange > strobeDelay) {
    fill_solid(leds, NUM_LEDS, palette[paletteIndex % paletteSize]);
    FastLED.show();
    paletteIndex++;
    lastChange = millis();
  }
}

//================================================================
// COMMAND HANDLER FUNCTIONS
//================================================================

void handlePower(bool on) {
  powerState = on;
  if (!on) {
    FastLED.clear();
    FastLED.show();
  } else {
    // Restore last state
    FastLED.setBrightness(currentBrightness);
    FastLED.show();
  }
  Serial.println("Power: " + String(on ? "ON" : "OFF"));
}

void handleMode(String mode) {
  currentMode = mode;
  Serial.println("Mode changed to: " + mode);
  
  // Reset animation state when mode changes
  gHue = 0;
  lastUpdate = 0;
}

void handlePattern(String pattern) {
  currentPattern = pattern;
  Serial.println("Pattern changed to: " + pattern);
  
  // Reset animation state
  gHue = 0;
  lastUpdate = 0;
}

void handleColor(JsonObject color) {
  if (powerState) {
    currentColor = CRGB(color["r"], color["g"], color["b"]);
    fill_solid(leds, NUM_LEDS, currentColor);
    FastLED.show();
    Serial.println("Color set to RGB(" + String(color["r"].as<int>()) + "," + 
                   String(color["g"].as<int>()) + "," + String(color["b"].as<int>()) + ")");
  }
}

void handlePalette(JsonArray paletteArray) {
  paletteSize = min((int)paletteArray.size(), 10); // Max 10 colors
  
  for(int i = 0; i < paletteSize; i++) {
    JsonObject colorObj = paletteArray[i];
    palette[i] = CRGB(colorObj["r"], colorObj["g"], colorObj["b"]);
  }
  
  Serial.println("Palette updated with " + String(paletteSize) + " colors");
}

void handleBrightness(int brightness) {
  // Convert 0-100 to 0-255
  currentBrightness = map(brightness, 0, 100, 0, 255);
  FastLED.setBrightness(currentBrightness);
  FastLED.show();
  Serial.println("Brightness set to: " + String(brightness) + "%");
}

void handleSpeed(int speed) {
  currentSpeed = constrain(speed, 0, 100);
  Serial.println("Speed set to: " + String(currentSpeed));
}

void handleMusicBeat(JsonObject color, int brightness) {
  if (powerState) {
    CRGB beatColor = CRGB(color["r"], color["g"], color["b"]);
    int beatBrightness = map(brightness, 0, 100, 0, 255);
    
    // Flash effect
    fill_solid(leds, NUM_LEDS, beatColor);
    FastLED.setBrightness(beatBrightness);
    FastLED.show();
    
    Serial.println("Music beat detected");
  }
}

//================================================================
// JSON PARSING LOGIC (เหมือนเดิม)
//================================================================
void parseAndExecuteCommand(String json) {
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, json);
  if (error) return;
  const char* type = doc["type"];
  if (strcmp(type, "POWER") == 0) handlePower(doc["value"]);
  else if (strcmp(type, "MODE") == 0) handleMode(doc["value"].as<String>());
  else if (strcmp(type, "PATTERN") == 0) handlePattern(doc["value"].as<String>());
  else if (strcmp(type, "COLOR") == 0) handleColor(doc["value"].as<JsonObject>());
  else if (strcmp(type, "PALETTE") == 0) handlePalette(doc["value"].as<JsonArray>());
  else if (strcmp(type, "BRIGHTNESS") == 0) handleBrightness(doc["value"]);
  else if (strcmp(type, "SPEED") == 0) handleSpeed(doc["value"]);
  else if (strcmp(type, "MUSIC_BEAT") == 0) handleMusicBeat(doc["color"].as<JsonObject>(), doc["brightness"]);
}

//================================================================
// PROVISIONING MODE HANDLERS (ส่วนสำหรับตั้งค่า)
//================================================================
// หน้า HTML สำหรับการตั้งค่า (จะถูกส่งไปให้เบราว์เซอร์)
void handleRoot() {
  server.send(200, "text/html", WIFI_SETUP_HTML);
}

// Handler สำหรับบันทึกข้อมูล Wi-Fi
void handleSaveWifi() {
  String new_ssid = server.arg("ssid");
  String new_pass = server.arg("pass");

  Serial.println("Saving Wi-Fi credentials...");
  nvs_open("storage", NVS_READWRITE, &my_handle);
  nvs_set_str(my_handle, "ssid", new_ssid.c_str());
  nvs_set_str(my_handle, "password", new_pass.c_str());
  nvs_commit(my_handle);
  nvs_close(my_handle);
  
  String html = R"rawliteral(
<!DOCTYPE HTML><html><head><title>Setup Success</title><meta name="viewport" content="width=device-width, initial-scale=1">
<style>body{font-family: Arial; background-color: #121212; color: #03DAC6; text-align: center; padding-top: 50px;}</style>
</head><body><h2>ตั้งค่าสำเร็จ!</h2><p>อุปกรณ์จะรีสตาร์ทและเชื่อมต่อกับ Wi-Fi ของคุณใน 5 วินาที</p><p>กรุณาเชื่อมต่อมือถือของคุณกลับไปยัง Wi-Fi เดิมเพื่อใช้งานต่อ</p></body></html>
)rawliteral";
  server.send(200, "text/html", html);

  delay(5000);
  ESP.restart();
}

//================================================================
// DEVICE NAME MANAGEMENT (สร้างชื่ออัตโนมัติครั้งแรก)
//================================================================
void generateDeviceName() {
  // สร้างชื่ออุปกรณ์จาก MAC Address ของ ESP32
  // รูปแบบ: B2SRGB-XXXXXX (6 หลักท้ายของ MAC)
  uint8_t mac[6];
  WiFi.macAddress(mac);
  
  // ใช้ 3 bytes สุดท้ายของ MAC เป็นหมายเลขเครื่อง
  snprintf(device_name, sizeof(device_name), "B2SRGB-%02X%02X%02X",
           mac[3], mac[4], mac[5]);
  
  Serial.print("Generated device name from MAC: ");
  Serial.println(device_name);
}

void loadDeviceName() {
  // โหลดชื่ออุปกรณ์จาก NVS
  size_t required_size;
  nvs_handle_t handle;
  esp_err_t err = nvs_open("storage", NVS_READONLY, &handle);
  
  if (err == ESP_OK) {
    err = nvs_get_str(handle, "device_name", NULL, &required_size);
    if (err == ESP_OK && required_size > 0) {
      nvs_get_str(handle, "device_name", device_name, &required_size);
      Serial.print("Loaded saved device name: ");
      Serial.println(device_name);
    } else {
      // ไม่มีชื่อเก่า สร้างใหม่
      Serial.println("No saved device name found. Generating new one...");
      generateDeviceName();
      
      // บันทึกชื่อใหม่ลง NVS
      nvs_close(handle);
      nvs_open("storage", NVS_READWRITE, &handle);
      nvs_set_str(handle, "device_name", device_name);
      nvs_commit(handle);
      Serial.println("Device name saved to NVS");
    }
    nvs_close(handle);
  } else {
    // NVS ยังไม่เปิด ใช้ชื่อเริ่มต้น
    Serial.println("NVS not ready, using default name");
  }
}

//================================================================
// SETUP FUNCTION
//================================================================
void setup() {
  Serial.begin(115200);
  
  // Initialize LEDs
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);

  // Initialize NVS (Non-Volatile Storage)
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      nvs_flash_erase();
      err = nvs_flash_init();
  }
  
  // Load or generate device name (ชื่ออุปกรณ์จะถูกสร้างครั้งแรกและจำไว้ตลอดไป)
  loadDeviceName();
  
  // Load saved Wi-Fi credentials
  size_t required_size;
  nvs_open("storage", NVS_READONLY, &my_handle);
  nvs_get_str(my_handle, "ssid", NULL, &required_size);
  nvs_get_str(my_handle, "ssid", saved_ssid, &required_size);
  nvs_get_str(my_handle, "password", NULL, &required_size);
  nvs_get_str(my_handle, "password", saved_password, &required_size);
  nvs_close(my_handle);
  
  if (strlen(saved_ssid) > 0) {
    // --- NORMAL MODE ---
    Serial.println("Found saved Wi-Fi. Connecting in Normal Mode...");
    WiFi.begin(saved_ssid, saved_password);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(500);
      Serial.print(".");
      attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWiFi connected! IP: " + WiFi.localIP().toString());
      
      // ===== HTTP API Endpoints สำหรับ iOS และอุปกรณ์ทั่วไป =====
      // รับคำสั่งควบคุม LED
      server.on("/command", HTTP_POST, [](){ 
        server.send(200, "text/plain", "OK"); 
        parseAndExecuteCommand(server.arg("plain")); 
      });
      
      // ตั้งค่าเปิด/ปิดไฟ
      server.on("/power", HTTP_POST, [](){
        String state = server.arg("state");
        handlePower(state == "on" || state == "true" || state == "1");
        server.send(200, "application/json", "{\"status\":\"ok\",\"power\":" + String(powerState ? "true" : "false") + "}");
      });
      
      // ตั้งค่าโหมด
      server.on("/mode", HTTP_POST, [](){
        String mode = server.arg("mode");
        handleMode(mode);
        server.send(200, "application/json", "{\"status\":\"ok\",\"mode\":\"" + currentMode + "\"}");
      });
      
      // ตั้งค่าเอฟเฟกต์
      server.on("/pattern", HTTP_POST, [](){
        String pattern = server.arg("pattern");
        handlePattern(pattern);
        server.send(200, "application/json", "{\"status\":\"ok\",\"pattern\":\"" + currentPattern + "\"}");
      });
      
      // ตั้งค่าสี RGB
      server.on("/color", HTTP_POST, [](){
        int r = server.arg("r").toInt();
        int g = server.arg("g").toInt();
        int b = server.arg("b").toInt();
        
        DynamicJsonDocument doc(128);
        doc["r"] = r;
        doc["g"] = g;
        doc["b"] = b;
        handleColor(doc.as<JsonObject>());
        
        server.send(200, "application/json", "{\"status\":\"ok\",\"color\":{\"r\":" + String(r) + ",\"g\":" + String(g) + ",\"b\":" + String(b) + "}}");
      });
      
      // ตั้งค่าความสว่าง
      server.on("/brightness", HTTP_POST, [](){
        int brightness = server.arg("value").toInt();
        handleBrightness(brightness);
        server.send(200, "application/json", "{\"status\":\"ok\",\"brightness\":" + String(brightness) + "}");
      });
      
      // ตั้งค่าความเร็ว
      server.on("/speed", HTTP_POST, [](){
        int speed = server.arg("value").toInt();
        handleSpeed(speed);
        server.send(200, "application/json", "{\"status\":\"ok\",\"speed\":" + String(speed) + "}");
      });
      
      // ดึงสถานะปัจจุบัน
      server.on("/status", HTTP_GET, [](){
        String json = "{";
        json += "\"power\":" + String(powerState ? "true" : "false") + ",";
        json += "\"mode\":\"" + currentMode + "\",";
        json += "\"pattern\":\"" + currentPattern + "\",";
        json += "\"brightness\":" + String(map(currentBrightness, 0, 255, 0, 100)) + ",";
        json += "\"speed\":" + String(currentSpeed) + ",";
        json += "\"deviceName\":\"" + String(device_name) + "\"";
        json += "}";
        server.send(200, "application/json", json);
      });
      
      // หน้า Control Panel สำหรับเปิดใน Browser
      server.on("/", HTTP_GET, [](){
        String html = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>B2SRGB Control Panel</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial; margin: 0; padding: 20px; background: #1a1a1a; color: #fff; }
    h1 { color: #03DAC6; text-align: center; }
    .container { max-width: 600px; margin: 0 auto; }
    .card { background: #2a2a2a; border-radius: 10px; padding: 20px; margin: 15px 0; }
    button { width: 100%; padding: 15px; margin: 5px 0; border: none; border-radius: 5px; 
             font-size: 16px; cursor: pointer; background: #03DAC6; color: #000; font-weight: bold; }
    button:hover { background: #00bfa5; }
    button.off { background: #555; color: #fff; }
    input[type="range"] { width: 100%; }
    .color-picker { display: flex; gap: 10px; align-items: center; }
    input[type="color"] { width: 60px; height: 60px; border: none; border-radius: 5px; cursor: pointer; }
    .status { text-align: center; padding: 10px; background: #333; border-radius: 5px; margin: 10px 0; }
  </style>
</head>
<body>
  <div class="container">
    <h1>🎨 B2SRGB Control</h1>
    <div class="status" id="status">กำลังโหลด...</div>
    
    <div class="card">
      <h3>⚡ เปิด/ปิด</h3>
      <button id="powerBtn" onclick="togglePower()">กำลังโหลด...</button>
    </div>
    
    <div class="card">
      <h3>🎭 โหมด</h3>
      <button onclick="setMode('สีเดียว')">สีเดียว</button>
      <button onclick="setMode('เปลี่ยนสี')">เปลี่ยนสี</button>
      <button onclick="setMode('กระพริบ')">กระพริบ</button>
      <button onclick="setMode('เอฟเฟกต์')">เอฟเฟกต์</button>
      <button onclick="setMode('ตามเพลง')">ตามเพลง</button>
    </div>
    
    <div class="card">
      <h3>✨ เอฟเฟกต์</h3>
      <button onclick="setPattern('รุ้งวนลูป')">รุ้งวนลูป</button>
      <button onclick="setPattern('รุ้งวิ่งไล่')">รุ้งวิ่งไล่</button>
      <button onclick="setPattern('เปลวไฟ')">เปลวไฟ</button>
      <button onclick="setPattern('คลื่นทะเล')">คลื่นทะเล</button>
      <button onclick="setPattern('ประกาย')">ประกาย</button>
    </div>
    
    <div class="card">
      <h3>🎨 สี</h3>
      <div class="color-picker">
        <input type="color" id="colorPicker" value="#ffffff">
        <button onclick="setColor()">ตั้งค่าสี</button>
      </div>
    </div>
    
    <div class="card">
      <h3>💡 ความสว่าง: <span id="brightnessValue">50</span>%</h3>
      <input type="range" id="brightness" min="0" max="100" value="50" oninput="setBrightness(this.value)">
    </div>
    
    <div class="card">
      <h3>⚡ ความเร็ว: <span id="speedValue">50</span></h3>
      <input type="range" id="speed" min="0" max="100" value="50" oninput="setSpeed(this.value)">
    </div>
  </div>
  
  <script>
    let powerState = true;
    
    function updateStatus() {
      fetch('/status')
        .then(r => r.json())
        .then(data => {
          powerState = data.power;
          document.getElementById('powerBtn').textContent = powerState ? '🟢 เปิด' : '🔴 ปิด';
          document.getElementById('powerBtn').className = powerState ? '' : 'off';
          document.getElementById('status').textContent = 
            `📱 ${data.deviceName} | โหมด: ${data.mode} | ${powerState ? 'เปิด' : 'ปิด'}`;
          document.getElementById('brightness').value = data.brightness;
          document.getElementById('brightnessValue').textContent = data.brightness;
          document.getElementById('speed').value = data.speed;
          document.getElementById('speedValue').textContent = data.speed;
        });
    }
    
    function togglePower() {
      powerState = !powerState;
      fetch('/power', {
        method: 'POST',
        headers: {'Content-Type': 'application/x-www-form-urlencoded'},
        body: 'state=' + (powerState ? 'on' : 'off')
      }).then(() => updateStatus());
    }
    
    function setMode(mode) {
      fetch('/mode', {
        method: 'POST',
        headers: {'Content-Type': 'application/x-www-form-urlencoded'},
        body: 'mode=' + encodeURIComponent(mode)
      }).then(() => updateStatus());
    }
    
    function setPattern(pattern) {
      fetch('/pattern', {
        method: 'POST',
        headers: {'Content-Type': 'application/x-www-form-urlencoded'},
        body: 'pattern=' + encodeURIComponent(pattern)
      }).then(() => updateStatus());
    }
    
    function setColor() {
      const color = document.getElementById('colorPicker').value;
      const r = parseInt(color.substr(1,2), 16);
      const g = parseInt(color.substr(3,2), 16);
      const b = parseInt(color.substr(5,2), 16);
      
      fetch('/color', {
        method: 'POST',
        headers: {'Content-Type': 'application/x-www-form-urlencoded'},
        body: `r=${r}&g=${g}&b=${b}`
      });
    }
    
    function setBrightness(value) {
      document.getElementById('brightnessValue').textContent = value;
      fetch('/brightness', {
        method: 'POST',
        headers: {'Content-Type': 'application/x-www-form-urlencoded'},
        body: 'value=' + value
      });
    }
    
    function setSpeed(value) {
      document.getElementById('speedValue').textContent = value;
      fetch('/speed', {
        method: 'POST',
        headers: {'Content-Type': 'application/x-www-form-urlencoded'},
        body: 'value=' + value
      });
    }
    
    // อัปเดตสถานะทุก 3 วินาที
    updateStatus();
    setInterval(updateStatus, 3000);
  </script>
</body>
</html>
)rawliteral";
        server.send(200, "text/html", html);
      });
      
      server.onNotFound([](){ server.send(404, "text/plain", "Not found"); });
      server.begin();
      
      // Initialize BLE with auto-generated device name
      BLEDevice::init(device_name);
      BLEServer *pServer = BLEDevice::createServer();
      BLEService *pService = pServer->createService(SERVICE_UUID);
      BLECharacteristic *pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
      );
      pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());
      pService->start();
      BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
      pAdvertising->addServiceUUID(SERVICE_UUID);
      pAdvertising->setScanResponse(true);
      pAdvertising->setMinPreferred(0x06);
      pAdvertising->setMinPreferred(0x12);
      BLEDevice::startAdvertising();
      Serial.print("BLE server started. Device name: ");
      Serial.println(device_name);
      
    } else {
      Serial.println("\nFailed to connect to saved Wi-Fi. Entering provisioning mode.");
      // ถ้าเชื่อมต่อ Wi-Fi ที่เคยบันทึกไว้ไม่ได้ ให้ลบข้อมูลเก่าแล้วเข้าโหมดตั้งค่า
      nvs_open("storage", NVS_READWRITE, &my_handle);
      nvs_erase_key(my_handle, "ssid");
      nvs_erase_key(my_handle, "password");
      nvs_commit(my_handle);
      nvs_close(my_handle);
      ESP.restart();
    }

  } else {
    // --- PROVISIONING MODE (CAPTIVE PORTAL) ---
    Serial.println("No Wi-Fi config found. Starting in Provisioning Mode...");
    
    // สร้าง SSID จากชื่ออุปกรณ์
    char ap_ssid[40];
    snprintf(ap_ssid, sizeof(ap_ssid), "%s-Setup", device_name);
    
    WiFi.softAP(ap_ssid);
    IPAddress apIP = WiFi.softAPIP();
    Serial.print("AP SSID: ");
    Serial.println(ap_ssid);
    Serial.println("AP IP address: " + apIP.toString());
    
    // Start DNS Server for Captive Portal
    dnsServer.start(53, "*", apIP);
    
    // Setup Web Server for configuration page
    server.on("/scan-wifi", HTTP_GET, []() { handleWifiScan(server); });
    server.on("/save-wifi", HTTP_POST, handleSaveWifi);
    server.onNotFound(handleRoot); // <--- ดักทุก request ให้ไปที่หน้าตั้งค่า
    server.begin();
    Serial.println("HTTP server and Captive Portal started.");
  }
}

//================================================================
// ANIMATION UPDATE
//================================================================
void updateAnimation() {
  if (!powerState) return;
  
  // Calculate delay based on speed (0-100 maps to slower-faster)
  int animationDelay = map(currentSpeed, 0, 100, 100, 10);
  
  if (millis() - lastUpdate > animationDelay) {
    // Execute animation based on mode and pattern
    if (currentMode == "สีเดียว") {
      // Solid color - already handled in handleColor
    }
    else if (currentMode == "เปลี่ยนสี") {
      // Fade mode
      paletteFade();
    }
    else if (currentMode == "กระพริบ") {
      // Strobe mode
      paletteStrobe();
    }
    else if (currentMode == "เอฟเฟกต์") {
      // Effects mode - run pattern
      if (currentPattern == "รุ้งวนลูป") {
        rainbowCycle();
      }
      else if (currentPattern == "รุ้งวิ่งไล่") {
        rainbowChase();
      }
      else if (currentPattern == "รุ้งระยิบ") {
        rainbowTwinkle();
      }
      else if (currentPattern == "เปลวไฟ") {
        fireEffect();
      }
      else if (currentPattern == "คลื่นทะเล") {
        oceanWave();
      }
      else if (currentPattern == "ป่าไม้") {
        // Forest effect (green tones)
        static uint8_t pos = 0;
        for(int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CHSV(96 + sin8(i * 10 + pos), 255, 255);
        }
        FastLED.show();
        pos++;
      }
      else if (currentPattern == "พระอาทิตย์ตก") {
        // Sunset effect
        for(int i = 0; i < NUM_LEDS; i++) {
          int hue = map(i, 0, NUM_LEDS, 0, 32); // Red to Orange
          leds[i] = CHSV(hue, 255, 255);
        }
        FastLED.show();
      }
      else if (currentPattern == "แสงเหนือ") {
        // Aurora effect
        static uint8_t pos = 0;
        for(int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CHSV((128 + sin8(i * 5 + pos)) % 255, 200, 200);
        }
        FastLED.show();
        pos += 2;
      }
      else if (currentPattern == "ลูกกวาด") {
        // Candy effect (pink and white)
        for(int i = 0; i < NUM_LEDS; i++) {
          if ((i + gHue) % 10 < 5) {
            leds[i] = CRGB::HotPink;
          } else {
            leds[i] = CRGB::White;
          }
        }
        FastLED.show();
        gHue++;
      }
      else if (currentPattern == "ลาวา") {
        // Lava effect (red and orange)
        fireEffect(); // Reuse fire effect
      }
      else if (currentPattern == "ดาวตก") {
        // Meteor effect
        static int meteorPos = 0;
        fadeToBlackBy(leds, NUM_LEDS, 64);
        
        for(int i = 0; i < 10; i++) {
          if (meteorPos - i >= 0 && meteorPos - i < NUM_LEDS) {
            leds[meteorPos - i] = CRGB::White;
          }
        }
        FastLED.show();
        
        meteorPos++;
        if (meteorPos > NUM_LEDS + 10) meteorPos = 0;
      }
      else if (currentPattern == "ประกาย") {
        // Sparkle effect
        fadeToBlackBy(leds, NUM_LEDS, 10);
        
        if (random8() < 50) {
          int pos = random16(NUM_LEDS);
          leds[pos] = CRGB::White;
        }
        FastLED.show();
      }
    }
    else if (currentMode == "ตามเพลง") {
      // Music mode - handled by handleMusicBeat from web app
    }
    
    lastUpdate = millis();
  }
}

//================================================================
// LOOP FUNCTION
//================================================================
void loop() {
  if (strlen(saved_ssid) > 0) {
    // Normal Mode Loop
    server.handleClient();
    updateAnimation(); // Update LED animations
  } else {
    // Provisioning Mode Loop
    dnsServer.processNextRequest(); // <--- สำคัญมากสำหรับ Captive Portal
    server.handleClient();
    
    // Show status LED pattern in provisioning mode
    static unsigned long lastBlink = 0;
    if (millis() - lastBlink > 500) {
      fill_solid(leds, min(10, NUM_LEDS), CRGB::Cyan);
      FastLED.show();
      delay(100);
      FastLED.clear();
      FastLED.show();
      lastBlink = millis();
    }
  }
}
