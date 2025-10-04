# 🚀 วิธีใช้ PlatformIO ใน VS Code (ฉบับย่อ)

## ✅ Prerequisites

- ✅ VS Code ติดตั้งแล้ว
- ✅ PlatformIO IDE Extension ติดตั้งแล้ว
- ✅ ESP32 เชื่อมต่อผ่าน USB

---

## 📋 Quick Start (5 ขั้นตอน)

### 1️⃣ เปิดโฟลเดอร์ Firmware

```bash
# วิธีที่ 1: ใน VS Code
File → Open Folder → เลือก firmware/

# วิธีที่ 2: Command Line
cd d:\9.15\B2SRGB\firmware
code .
```

---

### 2️⃣ รอ PlatformIO เตรียมโปรเจ็กต์

ครั้งแรกจะใช้เวลาสักครู่:
- ดาวน์โหลด ESP32 Platform
- ติดตั้งไลบรารี (ArduinoJson, FastLED, NimBLE)
- ตั้งค่าเครื่องมือทั้งหมด

ดูความคืบหน้าที่แถบด้านล่างขวา

---

### 3️⃣ Build (คอมไพล์)

**กด:** `Ctrl+Alt+B`

หรือคลิกปุ่ม **✓** ในแถบล่าง

**ผลลัพธ์:**
```
✓ SUCCESS
RAM:   [====      ] 45.2% (used 148288 bytes)
Flash: [=====     ] 48.9% (used 639857 bytes)
```

---

### 4️⃣ Upload (อัปโหลดลง ESP32)

**กด:** `Ctrl+Alt+U`

หรือคลิกปุ่ม **→** ในแถบล่าง

**ถ้า upload ไม่ได้:**
1. กดปุ่ม **BOOT** บน ESP32 ค้างไว้
2. รอจนเริ่ม upload (เห็น %)
3. ปล่อยปุ่ม BOOT

---

### 5️⃣ Serial Monitor (ดูผลลัพธ์)

**กด:** `Ctrl+Alt+S`

หรือคลิกปุ่ม **🔌** ในแถบล่าง

**จะเห็นข้อความ:**
```
Found saved Wi-Fi. Connecting in Normal Mode...
WiFi connected! IP: 192.168.1.50
BLE server started. Device name: B2SRGB
```

**ปิด Monitor:** กด `Ctrl+C`

---

## 🎯 คำสั่งหลัก

| ปุ่ม | Shortcut | คำสั่ง |
|------|----------|--------|
| **✓** | `Ctrl+Alt+B` | Build |
| **→** | `Ctrl+Alt+U` | Upload |
| **🔌** | `Ctrl+Alt+S` | Serial Monitor |
| **🧹** | - | Clean |

---

## 🔧 การตั้งค่า (ถ้าต้องการ)

### เปลี่ยน COM Port
แก้ในไฟล์ `platformio.ini`:
```ini
upload_port = COM3  ; เปลี่ยนเป็น port ของคุณ
```

### ลดความเร็ว Upload (ถ้า upload ไม่ได้)
```ini
upload_speed = 115200
```

### เปลี่ยน Board
```ini
board = esp32doit-devkit-v1  ; DevKit v1
board = nodemcu-32s          ; NodeMCU
```

---

## 🆚 เปรียบเทียบกับ Arduino IDE

| | Arduino IDE | **PlatformIO** |
|-|-------------|----------------|
| ติดตั้งไลบรารี | ❌ Manual | ✅ อัตโนมัติ |
| ความเร็ว Build | 🐢 ช้า | ⚡ เร็วกว่า 3 เท่า |
| Serial Monitor | 📊 พื้นฐาน | 🎨 Advanced + Color |
| Auto-complete | ❌ ไม่มี | ✅ มี (IntelliSense) |
| Git Integration | ❌ ไม่มี | ✅ มี |

---

## 💡 Tips

### Upload พร้อม Monitor
```
Ctrl+Shift+P → "PlatformIO: Upload and Monitor"
```

### ดู Tasks ทั้งหมด
```
Ctrl+Shift+B → เลือก Task
```

### เปิด PlatformIO Home
คลิกปุ่ม **🏠** ในแถบล่าง

---

## ❓ แก้ปัญหาเบื้องต้น

### ❌ ไม่เห็น COM Port
```
1. ติดตั้ง Driver (CH340/CP2102)
2. เสียบสาย USB ใหม่
3. เช็คใน Device Manager
```

### ❌ Upload Failed
```
1. กดปุ่ม BOOT บน ESP32
2. ลด upload_speed ใน platformio.ini
3. ลองใช้ USB Port อื่น
```

### ❌ Library Error
```
1. ลบโฟลเดอร์ .pio
2. Build ใหม่ (จะดาวน์โหลดใหม่อัตโนมัติ)
```

---

## 📚 อ่านเพิ่มเติม

- **คู่มือเต็ม**: [PLATFORMIO_GUIDE.md](PLATFORMIO_GUIDE.md)
- **Troubleshooting**: [TROUBLESHOOTING.md](TROUBLESHOOTING.md)
- **PlatformIO Docs**: https://docs.platformio.org

---

## 🎉 เริ่มใช้งานได้เลย!

```bash
# เปิดโฟลเดอร์
cd d:\9.15\B2SRGB\firmware
code .

# จากนั้น
Ctrl+Alt+B  # Build
Ctrl+Alt+U  # Upload
Ctrl+Alt+S  # Monitor
```

**ง่ายกว่า Arduino IDE มาก! 🚀**
