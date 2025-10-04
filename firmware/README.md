# B2SRGB Controller - ESP32 Firmware

## 📋 สารบัญ
- [คุณสมบัติ](#คุณสมบัติ)
- [ฮาร์ดแวร์ที่ต้องใช้](#ฮาร์ดแวร์ที่ต้องใช้)
- [ไลบรารีที่ต้องติดตั้ง](#ไลบรารีที่ต้องติดตั้ง)
- [การติดตั้ง](#การติดตั้ง)
- [การตั้งค่า](#การตั้งค่า)
- [API Commands](#api-commands)
- [การทำงาน](#การทำงาน)

## 📚 คู่มือเพิ่มเติม

- **[🔌 WIRING_GUIDE.md](WIRING_GUIDE.md)** - คู่มือการเชื่อมสาย LED Strip แบบละเอียด
- **[📤 UPLOAD_GUIDE.md](UPLOAD_GUIDE.md)** - คู่มือการอัปโหลด Firmware ทีละขั้นตอน
- **[🔧 TROUBLESHOOTING.md](TROUBLESHOOTING.md)** - แก้ปัญหาทุกอาการที่เจอบ่อย
- **[🎨 CUSTOMIZATION.md](CUSTOMIZATION.md)** - ปรับแต่ง Code และเพิ่มฟีเจอร์ใหม่

## 🌟 คุณสมบัติ

### 🔌 การเชื่อมต่อ
- **Wi-Fi Provisioning** - ตั้งค่า Wi-Fi ผ่าน Captive Portal (เด้งหน้าต่างอัตโนมัติบนมือถือ)
- **Wi-Fi Control** - ควบคุมผ่าน HTTP API
- **Bluetooth Low Energy (BLE)** - ควบคุมผ่าน BLE
- **บันทึกการตั้งค่า** - จดจำ Wi-Fi อัตโนมัติ (ใช้ NVS)

### 💡 ควบคุม LED
- รองรับ LED Strip ขนาด **1200 LEDs** (ปรับได้ตามต้องการ)
- รองรับ **WS2812B** และ LED ที่เข้ากันได้กับ FastLED
- ควบคุมสี, ความสว่าง, โหมด, และความเร็ว
- รองรับ Music Reactive Mode

## 🔧 ฮาร์ดแวร์ที่ต้องใช้

| ชิ้นส่วน | รายละเอียด |
|---------|-----------|
| **Microcontroller** | ESP32 (ทุกรุ่น) |
| **LED Strip** | WS2812B, WS2811, SK6812 (1200 LEDs หรือปรับได้) |
| **Power Supply** | 5V DC (กำลังวัตต์ตามจำนวน LED) |
| **Data Pin** | GPIO 5 (ปรับได้ในโค้ด) |

### 📐 การเชื่อมต่อ
```
ESP32          LED Strip
-----          ---------
GPIO 5  -----> DIN (Data In)
GND     -----> GND
5V      -----> 5V (ใช้ Power Supply ภายนอก)
```

> ⚠️ **คำเตือน**: สำหรับ LED จำนวนมาก ต้องใช้ Power Supply แยกต่างหาก ห้ามใช้ไฟจาก ESP32 โดยตรง!

## 📚 ไลบรารีที่ต้องติดตั้ง

ติดตั้งผ่าน **Arduino Library Manager**:

1. **ArduinoJson** (เวอร์ชั่น 6.x)
   ```
   Sketch -> Include Library -> Manage Libraries -> ค้นหา "ArduinoJson"
   ```

2. **FastLED** (เวอร์ชั่นล่าสุด)
   ```
   Sketch -> Include Library -> Manage Libraries -> ค้นหา "FastLED"
   ```

3. **ESP32 BLE Arduino** (มีมาพร้อม ESP32 Core)
   - ติดตั้ง ESP32 board: `https://dl.espressif.com/dl/package_esp32_index.json`

4. **DNSServer & WebServer** (มีมาพร้อม ESP32 Core)

## 🚀 การติดตั้ง

### 1. เตรียม Arduino IDE
```bash
# ติดตั้ง ESP32 Board Manager
File -> Preferences -> Additional Board Manager URLs:
https://dl.espressif.com/dl/package_esp32_index.json

# เลือก Board
Tools -> Board -> ESP32 Arduino -> ESP32 Dev Module
```

### 2. ปรับค่าคอนฟิก (ถ้าต้องการ)
แก้ไขใน `B2SRGB_Controller.ino`:

```cpp
// ปรับพินและจำนวน LED
#define LED_PIN     5        // เปลี่ยนพิน GPIO
#define NUM_LEDS    1200     // เปลี่ยนจำนวน LED
#define LED_TYPE    WS2812B  // เปลี่ยนประเภท LED
#define COLOR_ORDER GRB      // เปลี่ยน Color Order (RGB, GRB, etc.)
#define DEFAULT_BRIGHTNESS 200 // ความสว่างเริ่มต้น (0-255)
```

### 3. อัปโหลดโค้ด
```bash
1. เชื่อมต่อ ESP32 กับคอมพิวเตอร์ผ่าน USB
2. เลือก Port: Tools -> Port -> COM# หรือ /dev/ttyUSB#
3. กด Upload (ปุ่มลูกศรขวา)
4. รอจนกว่าจะเสร็จ (ประมาณ 30 วินาที)
```

## ⚙️ การตั้งค่า

### 🆕 ครั้งแรก (Provisioning Mode)

1. **ESP32 จะสร้าง Wi-Fi Access Point** ชื่อ `B2SRGB_Setup`
2. **เชื่อมต่อมือถือกับ Wi-Fi นี้**
3. **หน้าต่างจะเด้งขึ้นอัตโนมัติ** (Captive Portal)
4. **กรอก SSID และรหัสผ่าน Wi-Fi บ้านคุณ**
5. **กด "บันทึกและเชื่อมต่อ"**
6. ESP32 จะรีสตาร์ทและเชื่อมต่อกับ Wi-Fi ของคุณ

### 🔄 การใช้งานปกติ (Normal Mode)

เมื่อตั้งค่าเรียบร้อยแล้ว ESP32 จะ:
- ✅ เชื่อมต่อกับ Wi-Fi อัตโนมัติ
- ✅ เปิด HTTP Server (Port 80)
- ✅ เปิด BLE Server (ชื่อ "B2SRGB")
- ✅ แสดง IP Address ใน Serial Monitor

### 🔍 ดู IP Address
```bash
Tools -> Serial Monitor -> Set baud rate to 115200

# ดูข้อความ:
WiFi connected! IP: 192.168.1.XXX
```

### 🔁 รีเซ็ตการตั้งค่า Wi-Fi

ถ้าต้องการตั้งค่า Wi-Fi ใหม่:
1. ลบข้อมูลจาก NVS ด้วยโค้ด:
```cpp
nvs_flash_erase();
ESP.restart();
```
หรือ
2. Flash ESP32 ใหม่ทั้งหมด

## 📡 API Commands

### HTTP API (Wi-Fi)

**Endpoint**: `http://<ESP32_IP>/command`  
**Method**: `POST`  
**Content-Type**: `application/json`

### Command Examples:

#### 1. Power Control
```json
{
  "type": "POWER",
  "value": true
}
```

#### 2. Set Color
```json
{
  "type": "COLOR",
  "value": {
    "r": 255,
    "g": 0,
    "b": 0
  }
}
```

#### 3. Set Brightness
```json
{
  "type": "BRIGHTNESS",
  "value": 200
}
```
*(0-255)*

#### 4. Set Mode
```json
{
  "type": "MODE",
  "value": "สีเดียว"
}
```
*โหมด: "สีเดียว", "เปลี่ยนสี", "กระพริบ", "เอฟเฟกต์", "ตามเพลง"*

#### 5. Set Pattern
```json
{
  "type": "PATTERN",
  "value": "รุ้งวนลูป"
}
```

#### 6. Set Palette
```json
{
  "type": "PALETTE",
  "value": [
    {"r": 255, "g": 0, "b": 0},
    {"r": 0, "g": 255, "b": 0},
    {"r": 0, "g": 0, "b": 255}
  ]
}
```

#### 7. Set Speed
```json
{
  "type": "SPEED",
  "value": 50
}
```
*(0-100)*

#### 8. Music Beat
```json
{
  "type": "MUSIC_BEAT",
  "color": {
    "r": 255,
    "g": 0,
    "b": 0
  },
  "brightness": 255
}
```

### BLE API

**Service UUID**: `4fafc201-1fb5-459e-8fcc-c5c9c331914b`  
**Characteristic UUID**: `beb5483e-36e1-4688-b7f5-ea07361b26a8`

เขียนคำสั่ง JSON เดียวกับ HTTP API ลงใน Characteristic

## 🔄 การทำงาน

### Flowchart

```
Start
  |
  ├─> มี Wi-Fi ที่บันทึกไว้?
  |   |
  |   ├─> Yes: เชื่อมต่อ Wi-Fi
  |   |   |
  |   |   ├─> Success: Normal Mode
  |   |   |   ├─> เปิด HTTP Server
  |   |   |   ├─> เปิด BLE Server
  |   |   |   └─> รอรับคำสั่ง
  |   |   |
  |   |   └─> Fail: ลบข้อมูลและรีสตาร์ท
  |   |
  |   └─> No: Provisioning Mode
  |       ├─> สร้าง AP "B2SRGB_Setup"
  |       ├─> เปิด Captive Portal
  |       ├─> รอรับข้อมูล Wi-Fi
  |       └─> บันทึกและรีสตาร์ท
  |
Loop
```

## 🛠️ Troubleshooting

### ปัญหา: Captive Portal ไม่เด้งขึ้น
- ✅ ตรวจสอบว่าเชื่อมต่อกับ Wi-Fi "B2SRGB_Setup" แล้ว
- ✅ ลองเปิดเบราว์เซอร์แล้วเข้า `http://192.168.4.1`
- ✅ ปิด Mobile Data ของมือถือก่อน

### ปัญหา: เชื่อมต่อ Wi-Fi ไม่ได้
- ✅ ตรวจสอบ SSID และรหัสผ่านว่าถูกต้อง
- ✅ ตรวจสอบว่า Wi-Fi เป็น 2.4GHz (ESP32 ไม่รองรับ 5GHz)
- ✅ ตรวจสอบว่า Router อนุญาตให้อุปกรณ์ใหม่เข้าร่วม

### ปัญหา: LED ไม่ติด
- ✅ ตรวจสอบการเชื่อมต่อสายไฟ
- ✅ ตรวจสอบว่า LED_PIN ตรงกับการเชื่อมต่อ
- ✅ ตรวจสอบว่า Power Supply เพียงพอ
- ✅ ลองปรับ COLOR_ORDER (RGB, GRB, BRG)

### ปัญหา: BLE ไม่เจอ
- ✅ ตรวจสอบว่ามือถือเปิด Bluetooth แล้ว
- ✅ ค้นหาชื่อ "B2SRGB"
- ✅ ลองรีสตาร์ท ESP32

## 📝 หมายเหตุ

- ⚡ **การใช้ไฟ**: LED 1200 ดวงใช้ไฟสูงสุดประมาณ 72W (60mA/ดวง)
- 🔒 **ความปลอดภัย**: ระบบไม่มี Authentication ควรใช้ในเครือข่ายส่วนตัวเท่านั้น
- 🎨 **Patterns**: ต้องเขียนโค้ดเพิ่มเติมในฟังก์ชัน `handlePattern()` และ `handleMode()`
- 💾 **NVS**: ข้อมูล Wi-Fi จะไม่หายแม้ปิดเครื่อง

## 📞 ติดต่อ

- **GitHub**: [johnnckr/B2SRGB-Controller](https://github.com/johnnckr/B2SRGB-Controller)
- **Issues**: [Report a bug](https://github.com/johnnckr/B2SRGB-Controller/issues)

---

**สร้างด้วย ❤️ สำหรับ ESP32 RGB LED Control**
