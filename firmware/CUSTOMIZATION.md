# 🎨 คู่มือปรับแต่ง Code และเพิ่มฟีเจอร์

## 📋 สารบัญ

- [การตั้งค่าพื้นฐาน](#การตั้งค่าพื้นฐาน)
- [เพิ่ม Pattern ใหม่](#เพิ่ม-pattern-ใหม่)
- [ปรับแต่ง Captive Portal](#ปรับแต่ง-captive-portal)
- [เพิ่มคำสั่ง Custom](#เพิ่มคำสั่ง-custom)
- [เพิ่ม Multi-Strip Support](#เพิ่ม-multi-strip-support)
- [ปรับแต่ง BLE](#ปรับแต่ง-ble)

---

## ⚙️ การตั้งค่าพื้นฐาน

### 1. เปลี่ยนพิน GPIO

```cpp
// ในไฟล์ B2SRGB_Controller.ino

// เดิม
#define LED_PIN     5

// เปลี่ยนเป็นพินอื่น เช่น
#define LED_PIN     16    // GPIO 16
#define LED_PIN     17    // GPIO 17
#define LED_PIN     18    // GPIO 18
```

**GPIO ที่แนะนำ:** 5, 16, 17, 18, 19, 21, 22, 23

**GPIO ที่หลีกเลี่ยง:**
- GPIO 0, 2 (ใช้ตอน boot)
- GPIO 6-11 (ใช้กับ Flash)
- GPIO 34-39 (Input only)

---

### 2. เปลี่ยนจำนวน LED

```cpp
// เดิม
#define NUM_LEDS    1200

// เปลี่ยนตามจำนวนจริง
#define NUM_LEDS    60      // 1 เมตร (60 LED/m)
#define NUM_LEDS    144     // 1 เมตร (144 LED/m)
#define NUM_LEDS    300     // 5 เมตร (60 LED/m)
```

---

### 3. เปลี่ยนประเภท LED

```cpp
// เดิม
#define LED_TYPE    WS2812B

// เปลี่ยนเป็นชนิดอื่น
#define LED_TYPE    WS2811      // WS2811
#define LED_TYPE    SK6812      // SK6812
#define LED_TYPE    APA102      // APA102 (มี Clock pin)
#define LED_TYPE    WS2801      // WS2801 (มี Clock pin)
```

**สำหรับ LED ที่มี Clock Pin (APA102, WS2801):**
```cpp
#define LED_PIN     5       // Data pin
#define CLOCK_PIN   18      // Clock pin

// ใน setup()
FastLED.addLeds<APA102, LED_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
```

---

### 4. เปลี่ยนลำดับสี (Color Order)

```cpp
// เดิม
#define COLOR_ORDER GRB

// ลองเปลี่ยนถ้าสีไม่ถูก
#define COLOR_ORDER RGB     // แดง-เขียว-น้ำเงิน
#define COLOR_ORDER BGR     // น้ำเงิน-เขียว-แดง
#define COLOR_ORDER BRG     // น้ำเงิน-แดง-เขียว
```

**วิธีหาลำดับสีที่ถูก:**
```cpp
// ทดสอบสีแดง
fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0));
// ถ้าออกสีแดง = ถูกต้อง
// ถ้าออกสีอื่น = ลองเปลี่ยน COLOR_ORDER
```

---

### 5. ปรับความสว่างเริ่มต้น

```cpp
// เดิม
#define DEFAULT_BRIGHTNESS 200    // 78%

// เปลี่ยนเป็นค่าอื่น (0-255)
#define DEFAULT_BRIGHTNESS 50     // 20% (ประหยัดไฟ)
#define DEFAULT_BRIGHTNESS 100    // 39%
#define DEFAULT_BRIGHTNESS 150    // 59%
#define DEFAULT_BRIGHTNESS 255    // 100% (สว่างสุด)
```

---

### 6. เปลี่ยนชื่อ Wi-Fi AP (Provisioning Mode)

```cpp
// ในฟังก์ชัน setup() หา:
WiFi.softAP("B2SRGB_Setup");

// เปลี่ยนเป็น
WiFi.softAP("MyLED_Setup");          // ชื่อสั้นๆ
WiFi.softAP("Living_Room_LED");      // ระบุห้อง
WiFi.softAP("B2SRGB_" + String(ESP.getEfuseMac(), HEX));  // มี MAC Address
```

---

### 7. เปลี่ยนชื่อ BLE Device

```cpp
// ในฟังก์ชัน setup() หา:
BLEDevice::init("B2SRGB");

// เปลี่ยนเป็น
BLEDevice::init("MyLED");
BLEDevice::init("B2SRGB_" + String(ESP.getEfuseMac() & 0xFFFF, HEX));  // มี ID
```

---

## 🌈 เพิ่ม Pattern ใหม่

### Pattern 1: Rainbow Cycle (วนสีรุ้ง)

เพิ่มในฟังก์ชัน `handlePattern()`:

```cpp
void handlePattern(String pattern) {
  if (pattern == "รุ้งวนลูป") {
    static uint8_t hue = 0;
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(hue + (i * 255 / NUM_LEDS), 255, 255);
    }
    FastLED.show();
    hue++;
  }
  // ... patterns อื่นๆ
}
```

เพิ่มใน `loop()`:
```cpp
void loop() {
  if (strlen(saved_ssid) > 0) {
    server.handleClient();
    
    // เพิ่มบรรทัดนี้
    if (currentPattern == "รุ้งวนลูป") {
      handlePattern(currentPattern);
      delay(20);  // ความเร็ว
    }
  } else {
    dnsServer.processNextRequest();
    server.handleClient();
  }
}
```

---

### Pattern 2: Fire (เปลวไฟ)

```cpp
void fireEffect() {
  static byte heat[NUM_LEDS];
  
  // Step 1: ทำให้เย็นลง
  for(int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8(heat[i], random8(0, ((55 * 10) / NUM_LEDS) + 2));
  }
  
  // Step 2: แพร่กระจายความร้อน
  for(int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
  
  // Step 3: เพิ่มความร้อนที่ฐาน
  if(random8() < 120) {
    int y = random8(7);
    heat[y] = qadd8(heat[y], random8(160, 255));
  }
  
  // Step 4: แปลงความร้อนเป็นสี
  for(int j = 0; j < NUM_LEDS; j++) {
    byte colorindex = scale8(heat[j], 240);
    
    CRGB color = HeatColor(colorindex);
    leds[j] = color;
  }
  
  FastLED.show();
}
```

ใช้งานใน `handlePattern()`:
```cpp
void handlePattern(String pattern) {
  if (pattern == "เปลวไฟ") {
    fireEffect();
  }
  // ...
}
```

---

### Pattern 3: Meteor Rain (ดาวตก)

```cpp
void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {
  FastLED.clear();
  
  for(int i = 0; i < NUM_LEDS + NUM_LEDS; i++) {
    // ลบหาง
    for(int j = 0; j < NUM_LEDS; j++) {
      if((!meteorRandomDecay) || (random(10) > 5)) {
        leds[j].fadeToBlackBy(meteorTrailDecay);
      }
    }
    
    // วาด meteor
    for(int j = 0; j < meteorSize; j++) {
      if((i - j < NUM_LEDS) && (i - j >= 0)) {
        leds[i - j] = CRGB(red, green, blue);
      }
    }
    
    FastLED.show();
    delay(SpeedDelay);
  }
}
```

ใช้งาน:
```cpp
void handlePattern(String pattern) {
  if (pattern == "ดาวตก") {
    meteorRain(255, 255, 255, 10, 64, true, 30);
  }
  // ...
}
```

---

### Pattern 4: Ocean Wave (คลื่นทะเล)

```cpp
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
    
    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS - 1) - pixelnumber;
    
    nblend(leds[pixelnumber], newcolor, 64);
  }
  
  FastLED.show();
}
```

---

## 🎨 ปรับแต่ง Captive Portal

### 1. เปลี่ยนสีของหน้า Setup

แก้ในฟังก์ชัน `handleRoot()`:

```cpp
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE HTML><html><head>
<title>B2SRGB Wi-Fi Setup</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
  body{
    font-family: Arial, sans-serif; 
    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);  /* เปลี่ยนพื้นหลัง */
    color: #FFFFFF; 
    display: flex; 
    justify-content: center; 
    align-items: center; 
    height: 100vh; 
    margin: 0;
  }
  .container{
    background-color: rgba(255, 255, 255, 0.1);  /* กล่องโปร่งแสง */
    backdrop-filter: blur(10px);
    padding: 25px; 
    border-radius: 15px;  /* มุมโค้งมากขึ้น */
    box-shadow: 0 8px 32px rgba(0,0,0,0.3);
    width: 90%; 
    max-width: 400px;
  }
  h2{
    color: #FFFFFF;  /* เปลี่ยนสีหัวข้อ */
    text-align: center;
  }
  input{
    width: 100%; 
    padding: 12px; 
    margin-bottom: 15px; 
    border-radius: 8px;  /* มุมโค้ง */
    border: 2px solid rgba(255,255,255,0.3); 
    background-color: rgba(255,255,255,0.2);
    color: #FFFFFF; 
    box-sizing: border-box;
  }
  input::placeholder{
    color: rgba(255,255,255,0.7);
  }
  button{
    width: 100%; 
    padding: 12px; 
    border-radius: 8px; 
    border: none; 
    background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);  /* ปุ่มสีสวย */
    color: #FFFFFF; 
    font-weight: bold; 
    font-size: 16px; 
    cursor: pointer;
    transition: transform 0.2s;
  }
  button:hover{
    transform: scale(1.05);  /* ขยายเมื่อชี้ */
  }
</style>
</head><body>
<div class="container">
  <h2>🎨 ตั้งค่า Wi-Fi</h2>
  <form action="/save-wifi" method="POST">
    <input type="text" name="ssid" placeholder="ชื่อ Wi-Fi (SSID)" required><br>
    <input type="password" name="pass" placeholder="รหัสผ่าน"><br>
    <button type="submit">✨ เชื่อมต่อ</button>
  </form>
</div>
</body></html>)rawliteral";
  server.send(200, "text/html", html);
}
```

---

### 2. เพิ่มฟีเจอร์สแกน Wi-Fi

```cpp
String scanWiFi() {
  String options = "";
  int n = WiFi.scanNetworks();
  
  for(int i = 0; i < n; i++) {
    options += "<option value=\"" + WiFi.SSID(i) + "\">";
    options += WiFi.SSID(i) + " (" + String(WiFi.RSSI(i)) + " dBm)";
    options += "</option>";
  }
  
  return options;
}

void handleRoot() {
  String wifiOptions = scanWiFi();
  
  String html = R"rawliteral(
<!DOCTYPE HTML><html><head>
<title>B2SRGB Wi-Fi Setup</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>/* ... สไตล์เดิม ... */</style>
</head><body>
<div class="container">
  <h2>ตั้งค่า Wi-Fi สำหรับ B2SRGB</h2>
  <form action="/save-wifi" method="POST">
    <select name="ssid" id="ssid" onchange="document.getElementById('manual').style.display='none'">
      <option value="">-- เลือก Wi-Fi --</option>
      )rawliteral" + wifiOptions + R"rawliteral(
      <option value="__manual__">ใส่เอง...</option>
    </select><br>
    <input type="text" id="manual" name="manual_ssid" placeholder="ชื่อ Wi-Fi (ถ้าไม่มีในรายการ)" style="display:none"><br>
    <input type="password" name="pass" placeholder="รหัสผ่าน"><br>
    <button type="submit">บันทึกและเชื่อมต่อ</button>
  </form>
</div>
<script>
  document.getElementById('ssid').addEventListener('change', function() {
    if(this.value === '__manual__') {
      document.getElementById('manual').style.display='block';
    }
  });
</script>
</body></html>)rawliteral";
  
  server.send(200, "text/html", html);
}
```

---

## 🔧 เพิ่มคำสั่ง Custom

### 1. เพิ่มคำสั่ง "FADE_SPEED"

```cpp
// เพิ่มตัวแปร global
int fadeSpeed = 50;

// เพิ่มในฟังก์ชัน parseAndExecuteCommand()
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
  else if (strcmp(type, "FADE_SPEED") == 0) handleFadeSpeed(doc["value"]);  // ← เพิ่มนี้
  else if (strcmp(type, "MUSIC_BEAT") == 0) handleMusicBeat(doc["color"].as<JsonObject>(), doc["brightness"]);
}

// สร้างฟังก์ชันใหม่
void handleFadeSpeed(int speed) {
  fadeSpeed = constrain(speed, 1, 100);
  Serial.println("Fade speed: " + String(fadeSpeed));
}
```

---

### 2. เพิ่มคำสั่ง "GET_STATUS"

```cpp
// เพิ่ม endpoint ใหม่ใน setup()
server.on("/status", HTTP_GET, [](){
  DynamicJsonDocument doc(512);
  
  doc["power"] = powerState;
  doc["brightness"] = FastLED.getBrightness();
  doc["num_leds"] = NUM_LEDS;
  doc["wifi_rssi"] = WiFi.RSSI();
  doc["ip"] = WiFi.localIP().toString();
  doc["mac"] = WiFi.macAddress();
  doc["uptime"] = millis() / 1000;
  doc["free_heap"] = ESP.getFreeHeap();
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
});
```

ใช้งาน:
```bash
# GET request
curl http://192.168.1.50/status

# Response
{
  "power": true,
  "brightness": 200,
  "num_leds": 1200,
  "wifi_rssi": -45,
  "ip": "192.168.1.50",
  "mac": "AA:BB:CC:DD:EE:FF",
  "uptime": 12345,
  "free_heap": 234567
}
```

---

## 📡 เพิ่ม Multi-Strip Support

รองรับหลาย LED Strip:

```cpp
// ปรับแต่งการกำหนดค่า
#define LED_PIN_1     5
#define LED_PIN_2     16
#define NUM_LEDS_1    300
#define NUM_LEDS_2    300

CRGB leds1[NUM_LEDS_1];
CRGB leds2[NUM_LEDS_2];

void setup() {
  // ... โค้ดอื่นๆ
  
  FastLED.addLeds<WS2812B, LED_PIN_1, GRB>(leds1, NUM_LEDS_1);
  FastLED.addLeds<WS2812B, LED_PIN_2, GRB>(leds2, NUM_LEDS_2);
  
  // ... โค้ดอื่นๆ
}

// แก้ฟังก์ชัน handleColor
void handleColor(JsonObject color) {
  if (powerState) {
    CRGB c = CRGB(color["r"], color["g"], color["b"]);
    
    // ถ้ามี strip parameter
    if(color.containsKey("strip")) {
      int stripNum = color["strip"];
      if(stripNum == 1) {
        fill_solid(leds1, NUM_LEDS_1, c);
      } else if(stripNum == 2) {
        fill_solid(leds2, NUM_LEDS_2, c);
      }
    } else {
      // ไม่มี strip = ตั้งค่าทั้งหมด
      fill_solid(leds1, NUM_LEDS_1, c);
      fill_solid(leds2, NUM_LEDS_2, c);
    }
    
    FastLED.show();
  }
}
```

ใช้งาน:
```json
// ตั้งค่า strip 1 เป็นสีแดง
{"type": "COLOR", "value": {"r": 255, "g": 0, "b": 0, "strip": 1}}

// ตั้งค่าทั้งหมดเป็นสีน้ำเงิน
{"type": "COLOR", "value": {"r": 0, "g": 0, "b": 255}}
```

---

## 🎯 ปรับแต่ง BLE

### 1. เพิ่ม BLE Characteristics สำหรับอ่านค่า

```cpp
BLECharacteristic *pStatusCharacteristic;

void setup() {
  // ... โค้ดอื่นๆ
  
  // BLE Server Setup
  BLEDevice::init("B2SRGB");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  // Write Characteristic (เดิม)
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );
  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());
  
  // Read Characteristic (ใหม่)
  pStatusCharacteristic = pService->createCharacteristic(
    "beb5483e-36e1-4688-b7f5-ea07361b26a9",  // UUID ใหม่
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  
  pService->start();
  
  // ... โค้ดอื่นๆ
}

// ฟังก์ชันอัปเดตสถานะผ่าน BLE
void updateBLEStatus() {
  String status = "{\"power\":" + String(powerState ? "true" : "false") + 
                  ",\"brightness\":" + String(FastLED.getBrightness()) + "}";
  pStatusCharacteristic->setValue(status.c_str());
  pStatusCharacteristic->notify();
}
```

---

## 📝 ตัวอย่างโปรเจ็กต์เต็มรูปแบบ

ดูตัวอย่าง code เต็มๆ ได้ที่:
- `B2SRGB_Controller.ino` - ไฟล์หลัก
- GitHub Repository: https://github.com/johnnckr/B2SRGB-Controller

---

**Happy Coding! 💻✨**
