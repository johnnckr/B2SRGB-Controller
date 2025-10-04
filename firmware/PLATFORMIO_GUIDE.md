# B2SRGB Controller - PlatformIO Edition

## 🚀 วิธีใช้งาน PlatformIO ใน VS Code

### 1. เปิดโฟลเดอร์ Firmware

```
File → Open Folder → เลือก D:\9.15\B2SRGB\firmware
```

หรือ

```powershell
cd D:\9.15\B2SRGB\firmware
code .
```

---

### 2. โครงสร้างโปรเจ็กต์

PlatformIO ต้องการโครงสร้างแบบนี้:

```
firmware/
├── platformio.ini          # Configuration file ✅
├── src/
│   └── B2SRGB_Controller.ino  # ย้ายไฟล์มาที่นี่
├── lib/                    # Custom libraries (ถ้ามี)
└── include/                # Header files (ถ้ามี)
```

---

### 3. ติดตั้ง Dependencies อัตโนมัติ

เมื่อเปิดโปรเจ็กต์ครั้งแรก PlatformIO จะ:
- ✅ ดาวน์โหลด ESP32 Platform
- ✅ ติดตั้งไลบรารี (ArduinoJson, FastLED, NimBLE)
- ✅ ตั้งค่าทุกอย่างอัตโนมัติ

---

### 4. การใช้งาน

#### 🔌 เชื่อมต่อ ESP32
1. เสียบ ESP32 กับคอมพิวเตอร์ผ่าน USB
2. PlatformIO จะหา COM Port อัตโนมัติ

#### ⚙️ Build (Compile)
- กดปุ่ม **✓ (Build)** ในแถบด้านล่าง
- หรือกด `Ctrl+Alt+B`
- หรือเปิด Command Palette: `PlatformIO: Build`

#### 📤 Upload (อัปโหลด)
- กดปุ่ม **→ (Upload)** ในแถบด้านล่าง
- หรือกด `Ctrl+Alt+U`
- หรือเปิด Command Palette: `PlatformIO: Upload`

> 💡 **เคล็ดลับ**: ถ้า upload ไม่ได้ ให้กดปุ่ม **BOOT** บน ESP32 ค้างไว้ตอน uploading

#### 📺 Serial Monitor
- กดปุ่ม **🔌 (Serial Monitor)** ในแถบด้านล่าง
- หรือกด `Ctrl+Alt+S`
- หรือเปิด Command Palette: `PlatformIO: Serial Monitor`

#### 🧹 Clean
- เปิด Command Palette: `PlatformIO: Clean`

---

### 5. แถบเครื่องมือ PlatformIO

ดูที่แถบด้านล่างของ VS Code:

```
🏠 Home | ✓ Build | → Upload | 🧹 Clean | 🔬 Test | 🔌 Serial Monitor | 📊 Tasks
```

---

### 6. การตั้งค่าขั้นสูง

#### เปลี่ยน COM Port
แก้ไขใน `platformio.ini`:
```ini
upload_port = COM3  ; เปลี่ยนเป็น port ที่คุณใช้
```

#### เปลี่ยน Upload Speed
```ini
upload_speed = 115200  ; ลดความเร็วถ้า upload ไม่ได้
```

#### เปลี่ยน Board
```ini
board = esp32doit-devkit-v1  ; สำหรับ ESP32 DevKit v1
board = nodemcu-32s          ; สำหรับ NodeMCU-32S
board = esp32-s3-devkitc-1   ; สำหรับ ESP32-S3
```

#### เพิ่มไลบรารี
```ini
lib_deps = 
	bblanchon/ArduinoJson@^6.21.3
	fastled/FastLED@^3.6.0
	adafruit/Adafruit NeoPixel@^1.11.0  ; ← เพิ่มนี้
```

---

### 7. คำสั่งที่ใช้บ่อย

| คำสั่ง | Shortcut | คำอธิบาย |
|--------|----------|----------|
| Build | `Ctrl+Alt+B` | Compile โค้ด |
| Upload | `Ctrl+Alt+U` | อัปโหลดลง ESP32 |
| Serial Monitor | `Ctrl+Alt+S` | เปิด Serial Monitor |
| Clean | - | ลบไฟล์ build |
| Upload and Monitor | - | Upload + เปิด Monitor |

---

### 8. Troubleshooting

#### ❌ ไม่เห็น COM Port
```
1. ติดตั้ง Driver (CH340 หรือ CP2102)
2. ตรวจสอบใน Device Manager
3. ระบุ port ใน platformio.ini: upload_port = COM3
```

#### ❌ Upload Failed
```
1. กดปุ่ม BOOT บน ESP32 ค้างไว้ตอน upload
2. ลด upload_speed = 115200
3. ลองกดปุ่ม RESET แล้ว upload ใหม่
```

#### ❌ Library Not Found
```
1. เปิด PlatformIO Home
2. Libraries → ค้นหาไลบรารี
3. Install
หรือ
เพิ่มใน platformio.ini → lib_deps
```

#### ❌ Board Not Found
```
1. เปิด PlatformIO Home
2. Platforms → ค้นหา "Espressif 32"
3. Install
```

---

### 9. ข้อดีของ PlatformIO

✅ **จัดการไลบรารีอัตโนมัติ** - ไม่ต้องติดตั้งเอง
✅ **รองรับหลาย Board** - ESP32, ESP8266, Arduino, STM32, ฯลฯ
✅ **Build เร็วกว่า** - ใช้ Incremental Build
✅ **Serial Monitor ดีกว่า** - มี filter, colorize, decoder
✅ **Unit Testing** - ทดสอบโค้ดได้
✅ **OTA Update** - อัปโหลดผ่าน Wi-Fi ได้
✅ **Debugging** - Debug ด้วย GDB ได้

---

### 10. เปรียบเทียบกับ Arduino IDE

| ฟีเจอร์ | Arduino IDE | PlatformIO |
|---------|-------------|------------|
| ติดตั้งไลบรารี | Manual | อัตโนมัติ |
| Build Speed | ช้า | เร็วกว่า 2-3 เท่า |
| Serial Monitor | พื้นฐาน | Advanced + Filters |
| Multi-Project | ยาก | ง่าย |
| Version Control | ไม่มี | Git Integration |
| Debugging | ไม่ได้ | ได้ (ต้องมี debugger) |
| OTA Upload | ไม่ง่าย | ง่าย |

---

### 11. คำสั่ง Command Palette

กด `Ctrl+Shift+P` แล้วพิมพ์:

```
> PlatformIO: Build
> PlatformIO: Upload
> PlatformIO: Upload and Monitor
> PlatformIO: Clean
> PlatformIO: Serial Monitor
> PlatformIO: Home
> PlatformIO: New Terminal
```

---

### 12. Tips & Tricks

#### 💡 Upload แล้วเปิด Serial Monitor ทันที
```
Command Palette → PlatformIO: Upload and Monitor
```

#### 💡 ใช้ Task Runner
กด `Ctrl+Shift+B` เพื่อเปิด Task Menu

#### 💡 ดู Memory Usage
หลัง Build จะแสดง:
```
RAM:   [====      ]  45.2% (used 148288 bytes)
Flash: [=====     ]  48.9% (used 639857 bytes)
```

#### 💡 OTA Update (Upload ผ่าน Wi-Fi)
1. เพิ่มใน `platformio.ini`:
```ini
upload_protocol = espota
upload_port = 192.168.1.50  ; IP ของ ESP32
```

2. เพิ่มโค้ดใน ESP32:
```cpp
#include <ArduinoOTA.h>

void setup() {
  // ... WiFi setup
  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();
  // ... your code
}
```

---

### 13. การใช้งานจริง

#### ขั้นตอนที่ 1: Build
```
1. กด Ctrl+Alt+B
2. รอจนขึ้น "SUCCESS"
3. ตรวจสอบ memory usage
```

#### ขั้นตอนที่ 2: Upload
```
1. เสียบ ESP32
2. กด Ctrl+Alt+U
3. (ถ้าจำเป็น) กดปุ่ม BOOT บน ESP32
4. รอจนขึ้น "SUCCESS"
```

#### ขั้นตอนที่ 3: Monitor
```
1. กด Ctrl+Alt+S
2. ดู Serial output
3. กด Ctrl+C เพื่อปิด
```

---

## 🎯 Quick Start

```bash
# 1. เปิดโฟลเดอร์
cd D:\9.15\B2SRGB\firmware
code .

# 2. ใน VS Code
Ctrl+Alt+B  # Build
Ctrl+Alt+U  # Upload
Ctrl+Alt+S  # Serial Monitor
```

---

## 📞 ต้องการความช่วยเหลือ?

- **PlatformIO Docs**: https://docs.platformio.org
- **Community**: https://community.platformio.org
- **GitHub Issues**: https://github.com/platformio/platformio-core/issues

---

**Happy Coding with PlatformIO! 🚀**
