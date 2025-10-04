# 🔧 คู่มือแก้ปัญหาการ Upload Firmware

## 📋 สารบัญปัญหา

- [ไม่เห็น COM Port](#1-ไม่เห็น-com-port)
- [Upload Failed / Timeout](#2-upload-failed--timeout)
- [Serial Monitor แสดงข้อความแปลกๆ](#3-serial-monitor-แสดงข้อความแปลกๆ)
- [Compilation Error](#4-compilation-error)
- [ESP32 รีสตาร์ทเรื่อยๆ](#5-esp32-รีสตาร์ทเรื่อยๆ)
- [LED ไม่ติด](#6-led-ไม่ติด)
- [Wi-Fi เชื่อมต่อไม่ได้](#7-wi-fi-เชื่อมต่อไม่ได้)
- [Captive Portal ไม่เด้ง](#8-captive-portal-ไม่เด้ง)

---

## 1. ไม่เห็น COM Port

### อาการ:
- เปิด **Tools → Port** แล้วไม่มี COM# ให้เลือก
- หรือมีแต่เป็นสีเทา (เลือกไม่ได้)

### สาเหตุ:
- ❌ ไม่ได้ติดตั้ง Driver
- ❌ ใช้สาย Micro USB แบบชาร์จอย่างเดียว (ไม่มี Data)
- ❌ USB Port บนคอมพิวเตอร์เสีย
- ❌ ESP32 เสีย

### วิธีแก้:

#### แก้ที่ 1: ติดตั้ง Driver

**สำหรับ CH340/CH341:**
```
1. ดาวน์โหลด: https://sparks.gogo.co.nz/ch340.html
2. แตกไฟล์ zip
3. รัน CH341SER.EXE
4. กด Install
5. Restart คอมพิวเตอร์
6. เสียบ ESP32 ใหม่
```

**สำหรับ CP2102/CP2104:**
```
1. ดาวน์โหลด: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
2. เลือก Windows
3. ติดตั้งตามขั้นตอน
4. Restart คอมพิวเตอร์
5. เสียบ ESP32 ใหม่
```

**ตรวจสอบ Chip ที่ใช้:**
- ดูที่ตัว ESP32 จะมีชิปสี่เหลี่ยมเล็กๆ
- มักจะเขียน CH340G หรือ CP2102

#### แก้ที่ 2: ตรวจสอบสาย USB

```
✅ ลองใช้สายอื่น (ต้องเป็นสาย Data ไม่ใช่สายชาร์จอย่างเดียว)
✅ ลองเสียบ USB Port อื่น
✅ ลองเสียบ USB Hub (ถ้ามี)
```

#### แก้ที่ 3: ตรวจสอบใน Device Manager

```
1. กด Win + X
2. เลือก Device Manager
3. ดูที่ Ports (COM & LPT)
   
✅ ถ้าเห็น "USB-SERIAL CH340 (COM#)" = สาย OK, Driver OK
❌ ถ้าเห็นเครื่องหมายตกใจสีเหลือง = Driver ไม่ถูกต้อง
❌ ถ้าไม่เห็นอะไรเลย = สายเสีย หรือ ESP32 เสีย
```

#### แก้ที่ 4: ทดสอบด้วยเครื่องอื่น

```
✅ ลองเสียบกับคอมพิวเตอร์เครื่องอื่น
✅ ลองเสียบกับ Laptop
✅ ถ้าทุกเครื่องไม่เห็น = ESP32 เสีย
```

---

## 2. Upload Failed / Timeout

### อาการ:
```
Connecting........_____....._____....._____....._____....._____
A fatal error occurred: Failed to connect to ESP32: Timeout
```

### สาเหตุ:
- ❌ ESP32 ไม่เข้า Boot Mode
- ❌ Upload Speed สูงเกินไป
- ❌ Port เลือกผิด

### วิธีแก้:

#### แก้ที่ 1: กดปุ่ม BOOT (สำคัญที่สุด!)

```
วิธีการ:
1. กดปุ่ม Upload ใน Arduino IDE
2. รอจนขึ้นข้อความ "Connecting........"
3. **กดปุ่ม BOOT บน ESP32 ค้างไว้** (ปุ่มใกล้ USB)
4. เมื่อเริ่ม Upload (เห็น %) ให้ปล่อยปุ่ม
5. รอจนเสร็จ
```

**ตำแหน่งปุ่ม BOOT:**
```
┌─────────────────────┐
│                     │
│        ESP32        │
│                     │
│  [BOOT]      [EN]   │ ← ปุ่ม BOOT อยู่ด้านซ้าย
│    ↑          ↑     │
│   กดตรงนี้    รีเซ็ต│
│                     │
│      [USB Port]     │
└─────────────────────┘
```

#### แก้ที่ 2: ลด Upload Speed

```
Tools → Upload Speed → 115200 (เริ่มต้น)

ถ้ายังไม่ได้ ลองลดเป็น:
- 57600
- 38400
```

#### แก้ที่ 3: ตรวจสอบ Port

```
1. ไปที่ Tools → Port
2. ถอดสาย USB ออก
3. ดู Port ตัวไหนหาย
4. เสียบสายกลับ
5. เลือก Port ที่ปรากฏขึ้นมาใหม่
```

#### แก้ที่ 4: ใช้วิธี Manual Boot

```
1. กดปุ่ม BOOT ค้างไว้
2. กดปุ่ม EN (Reset) 1 ครั้ง
3. ปล่อยปุ่ม EN
4. ยังกดปุ่ม BOOT ค้างอยู่
5. กด Upload
6. เมื่อเริ่ม Upload ให้ปล่อยปุ่ม BOOT
```

#### แก้ที่ 5: เปลี่ยนการตั้งค่า Flash

```
Tools → Flash Mode → DIO (ลอง)
Tools → Flash Frequency → 40MHz (ลอง)
```

---

## 3. Serial Monitor แสดงข้อความแปลกๆ

### อาการ:
```
⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮
Garbled text
�������������
```

### สาเหตุ:
- ❌ Baud Rate ไม่ตรงกัน

### วิธีแก้:

```
1. เปิด Serial Monitor (Ctrl + Shift + M)
2. ตั้งค่า Baud Rate ขวาล่าง
3. เลือก: 115200 baud

✅ ถ้ายังไม่ชัด ลอง: 9600, 57600, 115200
```

---

## 4. Compilation Error

### อาการ 1: ArduinoJson.h: No such file

```
fatal error: ArduinoJson.h: No such file or directory
```

**วิธีแก้:**
```
1. Tools → Manage Libraries (Ctrl + Shift + I)
2. ค้นหา: ArduinoJson
3. ติดตั้ง: ArduinoJson by Benoit Blanchon (เวอร์ชั่น 6.x)
4. Restart Arduino IDE
```

### อาการ 2: FastLED.h: No such file

```
fatal error: FastLED.h: No such file or directory
```

**วิธีแก้:**
```
1. Tools → Manage Libraries
2. ค้นหา: FastLED
3. ติดตั้ง: FastLED by Daniel Garcia
4. Restart Arduino IDE
```

### อาการ 3: 'BLEDevice' was not declared

```
error: 'BLEDevice' was not declared in this scope
```

**วิธีแก้:**
```
ตรวจสอบว่าเลือก Board เป็น ESP32 แล้ว:
Tools → Board → esp32 → ESP32 Dev Module
```

### อาการ 4: Multiple libraries found

```
Multiple libraries were found for "WiFi.h"
```

**วิธีแก้:**
```
1. ไปที่ Arduino/libraries/
2. ลบโฟลเดอร์ WiFi ที่ไม่ใช่ของ ESP32
3. เก็บไว้แค่ใน: 
   AppData/Local/Arduino15/packages/esp32/hardware/esp32/x.x.x/libraries/WiFi
```

---

## 5. ESP32 รีสตาร์ทเรื่อยๆ

### อาการ:
```
rst:0x10 (RTCWDT_RTC_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
Brownout detector was triggered
```

### สาเหตุ:
- ❌ ไฟไม่พอ (Brownout)
- ❌ LED ใช้ไฟมากเกินไป

### วิธีแก้:

#### แก้ที่ 1: ใช้ Power Supply แยก

```
✅ ต่อ LED Strip กับ Power Supply 5V แยกต่างหาก
✅ ห้ามต่อ LED > 10 ดวง กับ ESP32 โดยตรง
✅ GND ต้องต่อร่วมกัน (ESP32 + Power Supply + LED)
```

#### แก้ที่ 2: ลดความสว่าง

```cpp
// ในโค้ด แก้เป็น
#define DEFAULT_BRIGHTNESS 50  // แทน 200
```

#### แก้ที่ 3: ใช้สาย USB ที่ดีกว่า

```
✅ ใช้สาย USB ที่หนา
✅ เสียบกับ USB Port หลังเครื่อง (ไฟแรงกว่า)
✅ ใช้ USB Hub ที่มีไฟเลี้ยง
```

---

## 6. LED ไม่ติด

### อาการ:
- Upload สำเร็จ
- Serial Monitor แสดงข้อความปกติ
- แต่ LED ไม่ติด

### วิธีแก้:

#### แก้ที่ 1: ตรวจสอบการต่อสาย

```
✅ ESP32 GPIO 5 → (Resistor 470Ω) → LED DIN
✅ ESP32 GND → LED GND
✅ Power Supply 5V → LED 5V
✅ Power Supply GND → LED GND + ESP32 GND (ต่อร่วม!)
```

#### แก้ที่ 2: ตรวจสอบทิศทาง LED Strip

```
LED Strip มี 2 ปลาย:
✅ DIN (Data Input) ← ต่อตรงนี้
❌ DOUT (Data Output) ← ห้ามต่อตรงนี้
```

#### แก้ที่ 3: เปลี่ยน COLOR_ORDER

```cpp
// ลองเปลี่ยนในโค้ด
#define COLOR_ORDER GRB  // ลองเป็น RGB, BRG, BGR
```

#### แก้ที่ 4: ลดจำนวน LED ทดสอบ

```cpp
// ลองทดสอบกับ LED 10 ดวงก่อน
#define NUM_LEDS    10  // แทน 1200
```

#### แก้ที่ 5: ใช้โค้ดทดสอบ

```cpp
#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    10
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  Serial.println("Starting LED test...");
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
}

void loop() {
  // สีแดง
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
  Serial.println("Red");
  delay(1000);
  
  // สีเขียว
  fill_solid(leds, NUM_LEDS, CRGB::Green);
  FastLED.show();
  Serial.println("Green");
  delay(1000);
  
  // สีน้ำเงิน
  fill_solid(leds, NUM_LEDS, CRGB::Blue);
  FastLED.show();
  Serial.println("Blue");
  delay(1000);
}
```

---

## 7. Wi-Fi เชื่อมต่อไม่ได้

### อาการ:
```
Failed to connect to saved Wi-Fi. Entering provisioning mode.
```

### วิธีแก้:

#### แก้ที่ 1: ตรวจสอบความถี่ Wi-Fi

```
❌ ESP32 ไม่รองรับ Wi-Fi 5GHz
✅ ต้องใช้ Wi-Fi 2.4GHz เท่านั้น

วิธีตั้งค่า Router:
1. เข้า Router Settings
2. แยก SSID ของ 2.4GHz และ 5GHz
3. เชื่อมต่อกับ SSID ของ 2.4GHz
```

#### แก้ที่ 2: ตรวจสอบ SSID และรหัสผ่าน

```
✅ พิมพ์ SSID และรหัสผ่านให้ถูกต้อง
✅ สังเกตตัวพิมพ์ใหญ่-เล็ก
✅ สังเกตช่องว่าง
```

#### แก้ที่ 3: ตรวจสอบ Router

```
1. เปิด Router Settings
2. ตรวจสอบว่า MAC Filtering ปิดอยู่
3. ตรวจสอบว่าไม่ได้บล็อก ESP32
```

#### แก้ที่ 4: รีเซ็ตการตั้งค่า Wi-Fi

```cpp
// เพิ่มโค้ดนี้ใน setup() แล้ว upload
nvs_flash_erase();
ESP.restart();
```

จากนั้น ESP32 จะกลับเข้า Provisioning Mode ให้ตั้งค่าใหม่

---

## 8. Captive Portal ไม่เด้ง

### อาการ:
- เชื่อมต่อ Wi-Fi "B2SRGB_Setup" แล้ว
- แต่หน้าต่างไม่เด้งขึ้น

### วิธีแก้:

#### แก้ที่ 1: เปิดเบราว์เซอร์เอง

```
1. เชื่อมต่อ Wi-Fi "B2SRGB_Setup"
2. เปิดเบราว์เซอร์
3. พิมพ์: http://192.168.4.1
4. จะเห็นหน้าตั้งค่า
```

#### แก้ที่ 2: ปิด Mobile Data

```
1. ปิด Mobile Data บนมือถือ
2. ถอด SIM Card (ถ้าจำเป็น)
3. เชื่อมต่อ Wi-Fi "B2SRGB_Setup" ใหม่
4. หน้าต่างควรเด้งขึ้น
```

#### แก้ที่ 3: ลืม Wi-Fi และเชื่อมต่อใหม่

```
1. ลืม Wi-Fi "B2SRGB_Setup"
2. รีสตาร์ท ESP32 (กดปุ่ม EN)
3. เชื่อมต่อ Wi-Fi ใหม่
4. หน้าต่างควรเด้งขึ้น
```

#### แก้ที่ 4: ใช้คอมพิวเตอร์

```
1. เชื่อมต่อคอมพิวเตอร์กับ "B2SRGB_Setup"
2. เปิดเบราว์เซอร์
3. ไปที่: http://192.168.4.1
4. ตั้งค่า Wi-Fi
```

---

## 🛠️ เครื่องมือช่วยแก้ปัญหา

### 1. ตรวจสอบว่า ESP32 ทำงาน

อัปโหลดโค้ดทดสอบนี้:

```cpp
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 is working!");
  pinMode(2, OUTPUT); // LED onboard
}

void loop() {
  digitalWrite(2, HIGH);
  Serial.println("LED ON");
  delay(500);
  
  digitalWrite(2, LOW);
  Serial.println("LED OFF");
  delay(500);
}
```

เปิด Serial Monitor ควรเห็น "ESP32 is working!" และ LED onboard กระพริบ

### 2. ตรวจสอบ Wi-Fi

```cpp
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.begin("YOUR_SSID", "YOUR_PASSWORD");
  
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {}
```

---

## 📞 ยังแก้ไม่ได้?

1. ตรวจสอบ Serial Monitor แล้วบันทึกข้อความ error
2. ถ่ายรูปวงจร
3. เปิด Issue ที่: https://github.com/johnnckr/B2SRGB-Controller/issues
4. ให้ข้อมูล:
   - รุ่น ESP32 ที่ใช้
   - ข้อความ error จาก Serial Monitor
   - รูปการต่อสาย
   - ขั้นตอนที่ทำมาแล้ว

---

**Good Luck! 🍀**
