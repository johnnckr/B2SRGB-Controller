# 🔧 Arduino IDE Setup Guide

## 1. ติดตั้ง Arduino IDE

### ดาวน์โหลด:
- เว็บไซต์: https://www.arduino.cc/en/software
- เลือกเวอร์ชั่น: Arduino IDE 2.x (ล่าสุด)

### ติดตั้ง (Windows):
```
1. ดาวน์โหลดไฟล์ .exe
2. รันและติดตั้งตามขั้นตอน
3. เปิด Arduino IDE
```

---

## 2. ติดตั้ง ESP32 Board Support

### เพิ่ม Board Manager URL:

1. เปิด Arduino IDE
2. ไปที่: **File → Preferences** (หรือกด `Ctrl+,`)
3. ในช่อง **"Additional boards manager URLs"** ใส่:
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
4. กด **OK**

### ติดตั้ง ESP32:

1. ไปที่: **Tools → Board → Boards Manager**
2. พิมพ์ค้นหา: **"esp32"**
3. เลือก **"esp32 by Espressif Systems"**
4. กด **Install** (รอสักครู่ ขนาดประมาณ 250MB)
5. เมื่อเสร็จแล้ว กด **Close**

---

## 3. ติดตั้งไลบรารี

### ไลบรารีที่จำเป็น:

#### 3.1 ArduinoJson
1. ไปที่: **Tools → Manage Libraries** (หรือกด `Ctrl+Shift+I`)
2. พิมพ์ค้นหา: **"ArduinoJson"**
3. เลือก **"ArduinoJson by Benoit Blanchon"**
4. เลือกเวอร์ชั่น **6.x** (ล่าสุด)
5. กด **Install**

#### 3.2 FastLED
1. ในหน้าต่าง Library Manager เดิม
2. พิมพ์ค้นหา: **"FastLED"**
3. เลือก **"FastLED by Daniel Garcia"**
4. กด **Install**

> ℹ️ **หมายเหตุ**: ไลบรารีอื่นๆ (BLE, DNSServer, WebServer) มีมากับ ESP32 Core อยู่แล้ว

---

## 4. เปิดไฟล์ Firmware

1. ไปที่โฟลเดอร์โปรเจ็กต์: `D:\9.15\B2SRGB\firmware\`
2. ดับเบิลคลิกที่: **`B2SRGB_Controller.ino`**
3. Arduino IDE จะเปิดไฟล์อัตโนมัติ

---

## 5. ตั้งค่า Board และ Port

### เลือก Board:
1. ไปที่: **Tools → Board → esp32**
2. เลือก: **"ESP32 Dev Module"** (หรือ board ที่คุณมี เช่น ESP32-WROOM)

### ตั้งค่า Upload Speed:
- **Tools → Upload Speed → 115200** (หรือ 921600 ถ้าต้องการเร็วขึ้น)

### ตั้งค่า Flash Frequency:
- **Tools → Flash Frequency → 80MHz**

### ตั้งค่า Flash Mode:
- **Tools → Flash Mode → QIO**

### ตั้งค่า Flash Size:
- **Tools → Flash Size → 4MB (32Mb)**

### ตั้งค่า Partition Scheme:
- **Tools → Partition Scheme → Default 4MB with spiffs**

---

## 6. เชื่อมต่อ ESP32

1. เชื่อมต่อ ESP32 กับคอมพิวเตอร์ผ่านสาย USB
2. รอให้ Windows ติดตั้ง Driver อัตโนมัติ
3. ไปที่: **Tools → Port**
4. เลือก: **COM# (USB-SERIAL CH340)** หรือ **COM# (Silicon Labs)**

> ⚠️ **ถ้าไม่เห็น Port**: ต้องติดตั้ง CH340 Driver หรือ CP2102 Driver

---

## 7. ปรับแต่งการตั้งค่า (ถ้าต้องการ)

เปิดไฟล์ `.ino` แล้วแก้ไข:

```cpp
// ปรับพิน GPIO ที่เชื่อมต่อกับ LED Strip
#define LED_PIN     5        // เปลี่ยนเป็นพินที่คุณใช้

// ปรับจำนวน LED
#define NUM_LEDS    1200     // เปลี่ยนตามจำนวนจริง (เช่น 60, 144, 300)

// ปรับประเภท LED
#define LED_TYPE    WS2812B  // หรือ WS2811, SK6812, APA102

// ปรับลำดับสี
#define COLOR_ORDER GRB      // ลอง RGB, BRG ถ้าสีไม่ถูก

// ปรับความสว่างเริ่มต้น
#define DEFAULT_BRIGHTNESS 200 // 0-255 (200 = 78%)
```

**บันทึกไฟล์**: กด `Ctrl+S`

---

## 8. Verify & Upload

### Verify (ตรวจสอบโค้ด):
1. กดปุ่ม **✓ (Verify)** หรือกด `Ctrl+R`
2. รอจนขึ้น "Done compiling"
3. ตรวจสอบว่าไม่มี error (ถ้ามี ให้แก้ไขก่อน)

### Upload (อัปโหลด):
1. กดปุ่ม **→ (Upload)** หรือกด `Ctrl+U`
2. รอให้โค้ด compile
3. เมื่อขึ้น **"Connecting..."**:
   - **กดปุ่ม BOOT บน ESP32 ค้างไว้**
   - รอจนกว่าจะเริ่ม upload (ขึ้น %)
   - ปล่อยปุ่ม BOOT

4. รอจนขึ้น:
   ```
   Writing at 0x00010000... (100%)
   Wrote XXXXX bytes
   Hash of data verified.
   Leaving...
   Hard resetting via RTS pin...
   ```

5. เมื่อเสร็จ LED บน ESP32 จะกระพริบและรีสตาร์ท

---

## 9. ตรวจสอบการทำงาน

### เปิด Serial Monitor:
1. กดปุ่ม **Serial Monitor** (ขวาบน) หรือกด `Ctrl+Shift+M`
2. ตั้งค่า Baud Rate: **115200**
3. ดูข้อความ:

**ถ้าเป็นครั้งแรก (Provisioning Mode):**
```
No Wi-Fi config found. Starting in Provisioning Mode...
AP IP address: 192.168.4.1
HTTP server and Captive Portal started.
```

**ถ้ามีการตั้งค่าแล้ว (Normal Mode):**
```
Found saved Wi-Fi. Connecting in Normal Mode...
...
WiFi connected! IP: 192.168.1.XXX
BLE server started. Device name: B2SRGB
```

---

## 10. ตั้งค่า Wi-Fi (ครั้งแรก)

1. เปิด Wi-Fi บนมือถือ
2. เชื่อมต่อกับ: **B2SRGB_Setup** (ไม่ต้องใส่รหัส)
3. หน้าต่างจะเด้งขึ้นอัตโนมัติ (Captive Portal)
4. กรอก:
   - **ชื่อ Wi-Fi (SSID)**: ชื่อ Wi-Fi บ้านคุณ
   - **รหัสผ่าน**: รหัสผ่าน Wi-Fi
5. กด **"บันทึกและเชื่อมต่อ"**
6. ESP32 จะรีสตาร์ทและเชื่อมต่อกับ Wi-Fi ของคุณ

---

## 11. เชื่อมต่อกับ Web App

1. ดู IP Address จาก Serial Monitor (เช่น `192.168.1.50`)
2. เปิด Web App: http://localhost:3000
3. กดปุ่ม **"เชื่อมต่ออุปกรณ์"**
4. เลือก **"เชื่อมต่อผ่าน Wi-Fi"**
5. ใส่ IP Address ของ ESP32
6. กด **"เชื่อมต่อ"**

✅ **เสร็จสิ้น!** ตอนนี้คุณสามารถควบคุม LED ผ่าน Web App แล้ว!

---

## 🔧 Troubleshooting

### ❌ ปัญหา: ไม่เห็น Port (COM#)

**วิธีแก้**:
1. ติดตั้ง Driver:
   - **CH340**: https://sparks.gogo.co.nz/ch340.html
   - **CP2102**: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers

2. ลอง:
   - ถอดสายแล้วเสียบใหม่
   - ลองใช้สาย USB อื่น (ต้องเป็นสาย Data)
   - ลองใช้ USB Port อื่น

### ❌ ปัญหา: Upload Error / Failed to connect

**วิธีแก้**:
1. **กดปุ่ม BOOT บน ESP32 ค้างไว้** ตอน upload
2. ลด Upload Speed: **Tools → Upload Speed → 115200**
3. ลองกดปุ่ม **RESET** บน ESP32 แล้ว upload ใหม่

### ❌ ปัญหา: Compilation Error

**วิธีแก้**:
1. ตรวจสอบว่าติดตั้งไลบรารีครบ:
   - ArduinoJson 6.x
   - FastLED
2. ตรวจสอบว่าเลือก Board เป็น ESP32
3. ลอง Restart Arduino IDE

### ❌ ปัญหา: LED ไม่ติด

**วิธีแก้**:
1. ตรวจสอบการเชื่อมต่อ:
   ```
   ESP32 GPIO 5 → LED Strip DIN
   GND          → LED Strip GND
   5V           → LED Strip 5V (ใช้ Power Supply แยก!)
   ```
2. ลองปรับ `COLOR_ORDER`:
   ```cpp
   #define COLOR_ORDER RGB  // หรือ GRB, BRG
   ```
3. ตรวจสอบว่า LED Strip ได้ไฟเพียงพอ

### ❌ ปัญหา: Captive Portal ไม่เด้ง

**วิธีแก้**:
1. เชื่อมต่อ Wi-Fi "B2SRGB_Setup"
2. ปิด Mobile Data
3. เปิดเบราว์เซอร์ไปที่: `http://192.168.4.1`

---

## 📞 ต้องการความช่วยเหลือ?

- **GitHub Issues**: https://github.com/johnnckr/B2SRGB-Controller/issues
- **Documentation**: `firmware/README.md`

---

**สร้างด้วย ❤️ สำหรับ ESP32 & RGB LED**
