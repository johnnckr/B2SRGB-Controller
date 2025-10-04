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
// DEVICE IDENTIFICATION (แก้ไขให้แต่ละเครื่องไม่ซ้ำกัน)
//================================================================
// ตัวอย่าง: "B2SRGB-001", "B2SRGB-002", "B2SRGB-Table01", "B2SRGB-Room05"
#define DEVICE_NAME "B2SRGB-001"  // <--- เปลี่ยนให้แต่ละเครื่องไม่ซ้ำกัน!

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

// Mode and Pattern Variables
String currentMode = "สีเดียว";
String currentPattern = "รุ้งวนลูป";
CRGB currentColor = CRGB::White;

// Default Color Palettes (ชุดสีแนะนำ)
CRGB rainbowPalette[6] = {CRGB(255,0,0), CRGB(255,127,0), CRGB(255,255,0), CRGB(0,255,0), CRGB(0,0,255), CRGB(148,0,211)};
CRGB firePalette[4] = {CRGB(255,0,0), CRGB(255,69,0), CRGB(255,140,0), CRGB(0,0,0)};
CRGB oceanPalette[3] = {CRGB(0,191,255), CRGB(0,0,255), CRGB(0,128,128)};
CRGB forestPalette[3] = {CRGB(34,139,34), CRGB(0,100,0), CRGB(85,107,47)};
CRGB sunsetPalette[4] = {CRGB(255,140,0), CRGB(255,20,147), CRGB(138,43,226), CRGB(220,20,60)};
CRGB neonPalette[3] = {CRGB(255,0,255), CRGB(0,255,0), CRGB(0,255,255)};

// Active Palette (default: Rainbow)
CRGB palette[10] = {CRGB(255,0,0), CRGB(255,127,0), CRGB(255,255,0), CRGB(0,255,0), CRGB(0,0,255), CRGB(148,0,211)};
int paletteSize = 6;
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

// Fire Pattern - ใช้ Fire Palette (แดง, ส้ม, เหลือง)
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
  
  // Step 4: Convert heat to LED colors using Fire Palette
  for(int j = 0; j < NUM_LEDS; j++) {
    byte temperature = heat[j];
    CRGB color;
    
    if(temperature < 85) {
      color = firePalette[3]; // Black/Dark (ดับ)
    } else if(temperature < 170) {
      color = blend(firePalette[0], firePalette[1], map(temperature, 85, 170, 0, 255)); // Red to Orange
    } else {
      color = blend(firePalette[1], firePalette[2], map(temperature, 170, 255, 0, 255)); // Orange to Yellow
    }
    
    leds[j] = color;
  }
  
  FastLED.show();
}

// Ocean Wave Pattern - ใช้ Ocean Palette (ฟ้า, น้ำเงิน, เขียวมรกต)
void oceanWave() {
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint8_t colorIndex = 0;
  
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  uint16_t brightnesstheta16 = sPseudotime;
  
  for(uint16_t i = 0; i < NUM_LEDS; i++) {
    // เลือกสีจาก Ocean Palette แบบไล่เรียง
    uint8_t palettePos = (i * 3 + colorIndex) % 3;
    CRGB baseColor = oceanPalette[palettePos];
    
    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;
    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    // Apply brightness to base color
    CRGB newcolor = baseColor;
    newcolor.nscale8(bri8);
    
    nblend(leds[i], newcolor, 64);
  }
  
  colorIndex++;
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

// Forest Breeze Pattern - ใช้ Forest Palette (เขียว, เขียวเข้ม, น้ำตาล)
void forestBreeze() {
  static uint16_t offset = 0;
  
  for(uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t palettePos = ((i * 5) + offset) % 3;
    CRGB baseColor = forestPalette[palettePos];
    
    // Add gentle wave effect
    uint8_t brightness = beatsin8(30, 150, 255, 0, i * 10);
    CRGB color = baseColor;
    color.nscale8(brightness);
    
    leds[i] = color;
  }
  
  offset++;
  FastLED.show();
}

// Aurora Borealis Pattern - ใช้ Rainbow Palette แบบนุ่มนวล
void auroraBorealis() {
  static uint16_t offset = 0;
  
  for(uint16_t i = 0; i < NUM_LEDS; i++) {
    // สร้างคลื่นแสงเหนือ
    uint8_t wave1 = beatsin8(13, 0, 255, 0, i * 3 + offset);
    uint8_t wave2 = beatsin8(17, 0, 255, 0, i * 5 + offset * 2);
    uint8_t brightness = (wave1 + wave2) / 2;
    
    // เลือกสีจาก Rainbow Palette
    uint8_t paletteIndex = (i / 10 + offset / 10) % 6;
    CRGB color = rainbowPalette[paletteIndex];
    color.nscale8(brightness);
    
    leds[i] = color;
  }
  
  offset++;
  FastLED.show();
}

// Lava Lamp Pattern - ใช้ Sunset Palette (ส้ม, ชมพู, ม่วง)
void lavaLamp() {
  static uint16_t offset = 0;
  
  for(uint16_t i = 0; i < NUM_LEDS; i++) {
    // สร้างการเคลื่อนไหวแบบ Lava
    uint8_t pos = sin8(i * 16 + offset);
    uint8_t paletteIndex = (pos / 64) % 4; // 4 สีใน Sunset Palette
    
    CRGB color1 = sunsetPalette[paletteIndex];
    CRGB color2 = sunsetPalette[(paletteIndex + 1) % 4];
    uint8_t blendAmount = (pos % 64) * 4;
    
    leds[i] = blend(color1, color2, blendAmount);
  }
  
  offset += 2;
  FastLED.show();
}

// Police Siren Pattern - สลับแดง-น้ำเงิน
void policeSiren() {
  static bool isRed = true;
  static unsigned long lastSwitch = 0;
  int switchDelay = map(currentSpeed, 0, 100, 200, 50);
  
  if(millis() - lastSwitch > switchDelay) {
    if(isRed) {
      fill_solid(leds, NUM_LEDS / 2, CRGB::Red);
      fill_solid(leds + NUM_LEDS / 2, NUM_LEDS / 2, CRGB::Black);
    } else {
      fill_solid(leds, NUM_LEDS / 2, CRGB::Black);
      fill_solid(leds + NUM_LEDS / 2, NUM_LEDS / 2, CRGB::Blue);
    }
    FastLED.show();
    isRed = !isRed;
    lastSwitch = millis();
  }
}

// Lightning Storm Pattern - ฟ้าแลบแบบสุ่ม
void lightningStorm() {
  static unsigned long lastLightning = 0;
  int lightningInterval = map(currentSpeed, 0, 100, 3000, 500);
  
  // Background: มืด
  fadeToBlackBy(leds, NUM_LEDS, 50);
  
  // สุ่มเวลาฟ้าแลบ
  if(millis() - lastLightning > lightningInterval || random8() < 5) {
    // ฟ้าแลบ: ขาว-ฟ้า
    int pos = random16(NUM_LEDS);
    int length = random8(10, 50);
    
    for(int i = 0; i < length && pos + i < NUM_LEDS; i++) {
      leds[pos + i] = CHSV(160 + random8(40), 200, 255); // สีฟ้า-ขาว
    }
    
    lastLightning = millis();
  }
  
  FastLED.show();
}

// Strobe Pattern - กระพริบเร็ว ใช้ Active Palette
void strobeEffect() {
  static bool isOn = false;
  static unsigned long lastFlash = 0;
  static uint8_t colorIndex = 0;
  int flashDelay = map(currentSpeed, 0, 100, 200, 20);
  
  if(millis() - lastFlash > flashDelay) {
    if(isOn) {
      fill_solid(leds, NUM_LEDS, palette[colorIndex % paletteSize]);
    } else {
      FastLED.clear();
      colorIndex++;
    }
    FastLED.show();
    isOn = !isOn;
    lastFlash = millis();
  }
}

// Sparkle Pattern - ประกายแสง ใช้ Neon Palette
void sparkleEffect() {
  fadeToBlackBy(leds, NUM_LEDS, 30);
  
  // สร้างประกายแสงสุ่ม 3-5 จุด
  int numSparkles = random8(3, 6);
  for(int i = 0; i < numSparkles; i++) {
    int pos = random16(NUM_LEDS);
    uint8_t colorIndex = random8(3); // 3 สีใน Neon Palette
    leds[pos] = neonPalette[colorIndex];
  }
  
  FastLED.show();
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

void handlePaletteName(String paletteName) {
  // เลือกชุดสีตามชื่อ
  if (paletteName == "rainbow") {
    memcpy(palette, rainbowPalette, sizeof(rainbowPalette));
    paletteSize = 6;
    Serial.println("Palette changed to: Rainbow");
  }
  else if (paletteName == "fire") {
    memcpy(palette, firePalette, sizeof(firePalette));
    paletteSize = 4;
    Serial.println("Palette changed to: Fire");
  }
  else if (paletteName == "ocean") {
    memcpy(palette, oceanPalette, sizeof(oceanPalette));
    paletteSize = 3;
    Serial.println("Palette changed to: Ocean");
  }
  else if (paletteName == "forest") {
    memcpy(palette, forestPalette, sizeof(forestPalette));
    paletteSize = 3;
    Serial.println("Palette changed to: Forest");
  }
  else if (paletteName == "sunset") {
    memcpy(palette, sunsetPalette, sizeof(sunsetPalette));
    paletteSize = 4;
    Serial.println("Palette changed to: Sunset");
  }
  else if (paletteName == "neon") {
    memcpy(palette, neonPalette, sizeof(neonPalette));
    paletteSize = 3;
    Serial.println("Palette changed to: Neon");
  }
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
  else if (strcmp(type, "PALETTE_NAME") == 0) handlePaletteName(doc["value"].as<String>()); // ใหม่: เลือกชุดสี
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
      server.on("/command", HTTP_POST, [](){ server.send(200, "text/plain", "OK"); parseAndExecuteCommand(server.arg("plain")); });
      server.onNotFound([](){ server.send(404, "text/plain", "Not found"); });
      server.begin();
      
      // Initialize BLE with unique device name
      BLEDevice::init(DEVICE_NAME);
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
      Serial.println("BLE server started. Device name: " + String(DEVICE_NAME));
      
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
    // ใช้ชื่อ AP ที่ไม่ซ้ำกัน (เพิ่ม _Setup)
    String apName = String(DEVICE_NAME) + "_Setup";
    WiFi.softAP(apName.c_str());
    IPAddress apIP = WiFi.softAPIP();
    Serial.println("AP Name: " + apName);
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
        fireEffect(); // ใช้ Fire Palette
      }
      else if (currentPattern == "คลื่นทะเล") {
        oceanWave(); // ใช้ Ocean Palette
      }
      else if (currentPattern == "ป่าไม้") {
        forestBreeze(); // ใช้ Forest Palette
      }
      else if (currentPattern == "แสงเหนือ") {
        auroraBorealis(); // ใช้ Rainbow Palette
      }
      else if (currentPattern == "ลาวา") {
        lavaLamp(); // ใช้ Sunset Palette
      }
      else if (currentPattern == "ไซเรนตำรวจ") {
        policeSiren(); // แดง-น้ำเงิน
      }
      else if (currentPattern == "ฟ้าแลบ") {
        lightningStorm(); // ฟ้า-ขาว
      }
      else if (currentPattern == "กระพริบ") {
        strobeEffect(); // ใช้ Active Palette
      }
      else if (currentPattern == "ประกาย") {
        sparkleEffect(); // ใช้ Neon Palette
        
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
