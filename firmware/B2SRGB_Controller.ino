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
// COMMAND HANDLER FUNCTIONS (เหมือนเดิม)
//================================================================
void handlePower(bool on) { powerState = on; if (!on) { FastLED.clear(); FastLED.show(); } }
void handleMode(String mode) { /* Add your code */ }
void handlePattern(String pattern) { /* Add your code */ }
void handleColor(JsonObject color) { if (powerState) { fill_solid(leds, NUM_LEDS, CRGB(color["r"], color["g"], color["b"])); FastLED.show(); } }
void handlePalette(JsonArray palette) { /* Add your code */ }
void handleBrightness(int brightness) { FastLED.setBrightness(brightness); FastLED.show(); }
void handleSpeed(int speed) { /* Add your code */ }
void handleMusicBeat(JsonObject color, int brightness) { if (powerState) { /* Add your code */ } }

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
  String html = R"rawliteral(
<!DOCTYPE HTML><html><head>
<title>B2SRGB Wi-Fi Setup</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
  body{font-family: Arial, sans-serif; background-color: #121212; color: #E0E0E0; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0;}
  .container{background-color: #1E1E1E; padding: 25px; border-radius: 10px; box-shadow: 0 0 15px rgba(0,0,0,0.5); width: 90%; max-width: 400px;}
  h2{color: #03DAC6; text-align: center;}
  input{width: 100%; padding: 12px; margin-bottom: 15px; border-radius: 5px; border: 1px solid #333; background-color: #2C2C2C; color: #E0E0E0; box-sizing: border-box;}
  button{width: 100%; padding: 12px; border-radius: 5px; border: none; background-color: #03DAC6; color: #121212; font-weight: bold; font-size: 16px; cursor: pointer;}
</style>
</head><body>
<div class="container">
  <h2>ตั้งค่า Wi-Fi สำหรับ B2SRGB</h2>
  <form action="/save-wifi" method="POST">
    <input type="text" name="ssid" placeholder="ชื่อ Wi-Fi (SSID)" required><br>
    <input type="password" name="pass" placeholder="รหัสผ่าน"><br>
    <button type="submit">บันทึกและเชื่อมต่อ</button>
  </form>
</div>
</body></html>)rawliteral";
  server.send(200, "text/html", html);
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
      
      // Initialize BLE
      BLEDevice::init("B2SRGB");
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
      Serial.println("BLE server started. Device name: B2SRGB");
      
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
    WiFi.softAP("B2SRGB_Setup");
    IPAddress apIP = WiFi.softAPIP();
    Serial.println("AP IP address: " + apIP.toString());
    
    // Start DNS Server for Captive Portal
    dnsServer.start(53, "*", apIP);
    
    // Setup Web Server for configuration page
    server.on("/save-wifi", HTTP_POST, handleSaveWifi);
    server.onNotFound(handleRoot); // <--- ดักทุก request ให้ไปที่หน้าตั้งค่า
    server.begin();
    Serial.println("HTTP server and Captive Portal started.");
  }
}

//================================================================
// LOOP FUNCTION
//================================================================
void loop() {
  if (strlen(saved_ssid) > 0) {
    // Normal Mode Loop
    server.handleClient();
  } else {
    // Provisioning Mode Loop
    dnsServer.processNextRequest(); // <--- สำคัญมากสำหรับ Captive Portal
    server.handleClient();
  }
}
