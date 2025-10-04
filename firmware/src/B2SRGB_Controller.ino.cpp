# 1 "C:\\Users\\johnn\\AppData\\Local\\Temp\\tmp8lj4kklt"
#include <Arduino.h>
# 1 "D:/9.15/B2SRGB/firmware/src/B2SRGB_Controller.ino"
# 17 "D:/9.15/B2SRGB/firmware/src/B2SRGB_Controller.ino"
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h>
#include <FastLED.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "wifi_scanner.h"




const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
#define LED_PIN 5
#define NUM_LEDS 1200
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define DEFAULT_BRIGHTNESS 200




#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"




WebServer server(80);
DNSServer dnsServer;
bool powerState = true;
nvs_handle_t my_handle;
char saved_ssid[32] = "";
char saved_password[64] = "";


String currentMode = "สีเดียว";
String currentPattern = "รุ้งวนลูป";
CRGB currentColor = CRGB::White;
CRGB palette[5] = {CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::Green, CRGB::Blue};
int paletteSize = 5;
int currentSpeed = 50;
int currentBrightness = DEFAULT_BRIGHTNESS;


uint8_t gHue = 0;
uint8_t gCurrentPatternNumber = 0;
unsigned long lastUpdate = 0;




void parseAndExecuteCommand(String json);
class MyCharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();
      if (value.length() > 0) {
        parseAndExecuteCommand(value.c_str());
      }
    }
};
void rainbowCycle();
void rainbowChase();
void rainbowTwinkle();
void fireEffect();
void oceanWave();
void paletteFade();
void paletteStrobe();
void handlePower(bool on);
void handleMode(String mode);
void handlePattern(String pattern);
void handleColor(JsonObject color);
void handlePalette(JsonArray paletteArray);
void handleBrightness(int brightness);
void handleSpeed(int speed);
void handleMusicBeat(JsonObject color, int brightness);
void handleRoot();
void handleSaveWifi();
void setup();
void updateAnimation();
void loop();
#line 89 "D:/9.15/B2SRGB/firmware/src/B2SRGB_Controller.ino"
void rainbowCycle() {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(gHue + (i * 255 / NUM_LEDS), 255, 255);
  }
  FastLED.show();
  gHue++;
}


void rainbowChase() {
  static uint8_t startIndex = 0;
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV((startIndex + i * 3) % 255, 255, 255);
  }
  FastLED.show();
  startIndex += 2;
}


void rainbowTwinkle() {
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = random16(NUM_LEDS);
  leds[pos] = CHSV(random8(), 255, 255);
  FastLED.show();
}


void fireEffect() {
  static byte heat[NUM_LEDS];


  for(int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8(heat[i], random8(0, ((55 * 10) / NUM_LEDS) + 2));
  }


  for(int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }


  if(random8() < 120) {
    int y = random8(7);
    heat[y] = qadd8(heat[y], random8(160, 255));
  }


  for(int j = 0; j < NUM_LEDS; j++) {
    byte colorindex = scale8(heat[j], 240);
    CRGB color = HeatColor(colorindex);
    leds[j] = color;
  }

  FastLED.show();
}


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





void handlePower(bool on) {
  powerState = on;
  if (!on) {
    FastLED.clear();
    FastLED.show();
  } else {

    FastLED.setBrightness(currentBrightness);
    FastLED.show();
  }
  Serial.println("Power: " + String(on ? "ON" : "OFF"));
}

void handleMode(String mode) {
  currentMode = mode;
  Serial.println("Mode changed to: " + mode);


  gHue = 0;
  lastUpdate = 0;
}

void handlePattern(String pattern) {
  currentPattern = pattern;
  Serial.println("Pattern changed to: " + pattern);


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
  paletteSize = min((int)paletteArray.size(), 10);

  for(int i = 0; i < paletteSize; i++) {
    JsonObject colorObj = paletteArray[i];
    palette[i] = CRGB(colorObj["r"], colorObj["g"], colorObj["b"]);
  }

  Serial.println("Palette updated with " + String(paletteSize) + " colors");
}

void handleBrightness(int brightness) {

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


    fill_solid(leds, NUM_LEDS, beatColor);
    FastLED.setBrightness(beatBrightness);
    FastLED.show();

    Serial.println("Music beat detected");
  }
}




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





void handleRoot() {
  server.send(200, "text/html", WIFI_SETUP_HTML);
}


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




void setup() {
  Serial.begin(115200);


  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);


  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      nvs_flash_erase();
      err = nvs_flash_init();
  }


  size_t required_size;
  nvs_open("storage", NVS_READONLY, &my_handle);
  nvs_get_str(my_handle, "ssid", NULL, &required_size);
  nvs_get_str(my_handle, "ssid", saved_ssid, &required_size);
  nvs_get_str(my_handle, "password", NULL, &required_size);
  nvs_get_str(my_handle, "password", saved_password, &required_size);
  nvs_close(my_handle);

  if (strlen(saved_ssid) > 0) {

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

      nvs_open("storage", NVS_READWRITE, &my_handle);
      nvs_erase_key(my_handle, "ssid");
      nvs_erase_key(my_handle, "password");
      nvs_commit(my_handle);
      nvs_close(my_handle);
      ESP.restart();
    }

  } else {

    Serial.println("No Wi-Fi config found. Starting in Provisioning Mode...");
    WiFi.softAP("B2SRGB_Setup");
    IPAddress apIP = WiFi.softAPIP();
    Serial.println("AP IP address: " + apIP.toString());


    dnsServer.start(53, "*", apIP);


    server.on("/scan-wifi", HTTP_GET, []() { handleWifiScan(server); });
    server.on("/save-wifi", HTTP_POST, handleSaveWifi);
    server.onNotFound(handleRoot);
    server.begin();
    Serial.println("HTTP server and Captive Portal started.");
  }
}




void updateAnimation() {
  if (!powerState) return;


  int animationDelay = map(currentSpeed, 0, 100, 100, 10);

  if (millis() - lastUpdate > animationDelay) {

    if (currentMode == "สีเดียว") {

    }
    else if (currentMode == "เปลี่ยนสี") {

      paletteFade();
    }
    else if (currentMode == "กระพริบ") {

      paletteStrobe();
    }
    else if (currentMode == "เอฟเฟกต์") {

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

        static uint8_t pos = 0;
        for(int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CHSV(96 + sin8(i * 10 + pos), 255, 255);
        }
        FastLED.show();
        pos++;
      }
      else if (currentPattern == "พระอาทิตย์ตก") {

        for(int i = 0; i < NUM_LEDS; i++) {
          int hue = map(i, 0, NUM_LEDS, 0, 32);
          leds[i] = CHSV(hue, 255, 255);
        }
        FastLED.show();
      }
      else if (currentPattern == "แสงเหนือ") {

        static uint8_t pos = 0;
        for(int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CHSV((128 + sin8(i * 5 + pos)) % 255, 200, 200);
        }
        FastLED.show();
        pos += 2;
      }
      else if (currentPattern == "ลูกกวาด") {

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

        fireEffect();
      }
      else if (currentPattern == "ดาวตก") {

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

        fadeToBlackBy(leds, NUM_LEDS, 10);

        if (random8() < 50) {
          int pos = random16(NUM_LEDS);
          leds[pos] = CRGB::White;
        }
        FastLED.show();
      }
    }
    else if (currentMode == "ตามเพลง") {

    }

    lastUpdate = millis();
  }
}




void loop() {
  if (strlen(saved_ssid) > 0) {

    server.handleClient();
    updateAnimation();
  } else {

    dnsServer.processNextRequest();
    server.handleClient();


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