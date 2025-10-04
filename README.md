# 🎨 B2SRGB Controller

<div align="center">

![B2SRGB Controller](https://img.shields.io/badge/ESP32-RGB%20LED%20Controller-green?style=for-the-badge&logo=espressif)
![React](https://img.shields.io/badge/React-19.1.1-61DAFB?style=for-the-badge&logo=react)
![TypeScript](https://img.shields.io/badge/TypeScript-5.8.2-3178C6?style=for-the-badge&logo=typescript)
![PWA](https://img.shields.io/badge/PWA-Ready-5A0FC8?style=for-the-badge&logo=pwa)

**Progressive Web App สำหรับควบคุม RGB LED Strip ผ่าน ESP32**

[📱 Web App](#-web-application) • [🔌 Firmware](#-esp32-firmware) • [📖 Documentation](#-documentation) • [🚀 Quick Start](#-quick-start)

</div>

---

## ✨ คุณสมบัติ

### 📱 Web Application (PWA)
- ⚡ **Progressive Web App** - ติดตั้งได้บนมือถือ ทำงานแบบออฟไลน์
- 🎨 **UI สวยงาม** - ใช้ Tailwind CSS รองรับภาษาไทย
- 🌈 **โหมดไฟ 5 แบบ**:
  - **สีเดียว** - Color Picker แบบ Advanced พร้อม HSL Wheel
  - **เปลี่ยนสี** - Fade ผ่าน Color Palette
  - **กระพริบ** - Strobe Effect
  - **เอฟเฟกต์** - Patterns พร้อม Presets (Fire, Ocean, Rainbow)
  - **ตามเพลง** - Music Reactive ใช้ไมโครโฟน
- 🔌 **การเชื่อมต่อหลากหลาย**:
  - Wi-Fi Provisioning (ตั้งค่าอุปกรณ์ใหม่)
  - Wi-Fi Direct (เชื่อมต่อผ่าน IP)
  - Web Bluetooth (BLE)
- 💡 **Auto-Brightness** - ปรับความสว่างอัตโนมัติตาม Ambient Light Sensor
- 🎵 **Music Visualizer** - แสดงผลคลื่นเสียงแบบ Real-time

### 🔌 ESP32 Firmware
- 📡 **Captive Portal** - ตั้งค่า Wi-Fi แบบง่ายดาย (หน้าต่างเด้งอัตโนมัติ)
- 💾 **NVS Storage** - บันทึกการตั้งค่า Wi-Fi ถาวร
- 🌐 **HTTP API** - รับคำสั่งผ่าน REST API
- 📶 **BLE Server** - ควบคุมผ่าน Bluetooth Low Energy
- 💡 **FastLED** - รองรับ WS2812B และ LED Strip ชนิดอื่นๆ
- 🔄 **Auto-Reconnect** - เชื่อมต่อ Wi-Fi อัตโนมัติ

---

## 🚀 Quick Start

### 📱 Web Application

**Prerequisites:** Node.js 18+

```bash
# 1. Clone Repository
git clone https://github.com/johnnckr/B2SRGB-Controller.git
cd B2SRGB-Controller

# 2. ติดตั้ง Dependencies
npm install

# 3. รันโปรเจ็กต์
npm run dev

# 4. เปิดเบราว์เซอร์ที่
# http://localhost:3000
```

**Build สำหรับ Production:**
```bash
npm run build
npm run preview
```

### 🔌 ESP32 Firmware

**Prerequisites:** 
- Arduino IDE 2.x
- ESP32 Board Support
- ไลบรารี: ArduinoJson, FastLED

```bash
# 1. เปิดไฟล์
firmware/B2SRGB_Controller.ino

# 2. ปรับค่าคอนฟิก (ถ้าต้องการ)
#define LED_PIN     5
#define NUM_LEDS    1200

# 3. เลือก Board
Tools -> Board -> ESP32 Dev Module

# 4. Upload
กดปุ่ม Upload
```

📖 **เอกสารเต็มรูปแบบ**: [firmware/README.md](firmware/README.md)

---

## 📂 โครงสร้างโปรเจ็กต์

```
B2SRGB-Controller/
├── 📱 Web Application (PWA)
│   ├── src/
│   │   ├── components/        # React Components
│   │   │   ├── App.tsx       # Main App Component
│   │   │   ├── ConnectionModal.tsx
│   │   │   ├── ColorPicker.tsx
│   │   │   ├── MusicMode.tsx
│   │   │   └── ...
│   │   ├── hooks/            # Custom React Hooks
│   │   │   ├── useAudioAnalyzer.ts
│   │   │   └── useAmbientLightSensor.ts
│   │   ├── services/         # Service Layer
│   │   │   ├── connectionService.ts
│   │   │   └── esp32Service.ts
│   │   ├── types.ts          # TypeScript Types
│   │   └── index.tsx         # Entry Point
│   ├── public/
│   │   ├── icon.svg          # App Icon
│   │   ├── manifest.json     # PWA Manifest
│   │   └── service-worker.js # Service Worker
│   ├── index.html
│   ├── package.json
│   └── vite.config.ts
│
└── 🔌 ESP32 Firmware
    └── firmware/
        ├── B2SRGB_Controller.ino  # Main Firmware
        └── README.md              # Firmware Documentation
```

---

## 🎯 การใช้งาน

### 1️⃣ ตั้งค่า ESP32 ครั้งแรก

1. อัปโหลด firmware ลง ESP32
2. ESP32 จะสร้าง Wi-Fi AP ชื่อ **"B2SRGB_Setup"**
3. เชื่อมต่อมือถือกับ Wi-Fi นี้
4. หน้าต่างตั้งค่าจะเด้งขึ้นอัตโนมัติ (Captive Portal)
5. กรอก SSID และรหัสผ่าน Wi-Fi บ้านคุณ
6. ESP32 จะรีสตาร์ทและเชื่อมต่อกับ Wi-Fi ของคุณ

### 2️⃣ เปิด Web App

1. เปิดเบราว์เซอร์ไปที่ http://localhost:3000 (หรือ deploy บน server)
2. กดปุ่ม **"เชื่อมต่ออุปกรณ์"**
3. เลือกวิธีการเชื่อมต่อ:
   - **Wi-Fi** - ใส่ IP Address ของ ESP32
   - **Bluetooth** - สแกนและเชื่อมต่อผ่าน BLE

### 3️⃣ ควบคุม LED

- เปิด/ปิด ไฟด้วยปุ่มกลาง
- เลือกโหมดที่ต้องการ (สีเดียว, เปลี่ยนสี, กระพริบ, เอฟเฟกต์, ตามเพลง)
- ปรับความสว่าง, สี, และความเร็ว
- ใช้ Auto-Brightness สำหรับปรับอัตโนมัติ

---

## 🛠️ เทคโนโลยีที่ใช้

### Frontend
- **React 19.1.1** - UI Library
- **TypeScript 5.8.2** - Type Safety
- **Tailwind CSS** - Styling (via CDN)
- **Vite 6.2** - Build Tool
- **Web APIs**:
  - Web Audio API (Music Mode)
  - Ambient Light Sensor API (Auto-Brightness)
  - Web Bluetooth API (BLE Connection)
  - Service Worker (PWA)

### Backend (ESP32)
- **ESP32** - Microcontroller
- **Arduino Framework**
- **FastLED** - LED Control
- **ArduinoJson** - JSON Parsing
- **BLE** - Bluetooth Communication
- **DNSServer** - Captive Portal
- **NVS** - Non-Volatile Storage

---

## 📡 API Documentation

### HTTP API

**Base URL**: `http://<ESP32_IP>/command`  
**Method**: `POST`  
**Content-Type**: `application/json`

**ตัวอย่างคำสั่ง**:

```json
// เปิด/ปิด
{"type": "POWER", "value": true}

// เปลี่ยนสี
{"type": "COLOR", "value": {"r": 255, "g": 0, "b": 0}}

// ปรับความสว่าง (0-255)
{"type": "BRIGHTNESS", "value": 200}

// เปลี่ยนโหมด
{"type": "MODE", "value": "สีเดียว"}

// เปลี่ยน Pattern
{"type": "PATTERN", "value": "รุ้งวนลูป"}

// ตั้งค่า Palette
{"type": "PALETTE", "value": [
  {"r": 255, "g": 0, "b": 0},
  {"r": 0, "g": 255, "b": 0}
]}

// ปรับความเร็ว (0-100)
{"type": "SPEED", "value": 50}
```

### BLE API

**Service UUID**: `4fafc201-1fb5-459e-8fcc-c5c9c331914b`  
**Characteristic UUID**: `beb5483e-36e1-4688-b7f5-ea07361b26a8`

เขียนคำสั่ง JSON (เหมือนกับ HTTP API) ลงใน Characteristic

---

## 🌟 Features Roadmap

- [x] PWA Support
- [x] Wi-Fi Provisioning (Captive Portal)
- [x] Web Bluetooth Connection
- [x] Music Reactive Mode
- [x] Auto-Brightness
- [x] Color Presets
- [ ] Custom Pattern Editor
- [ ] Schedule/Timer
- [ ] Voice Control
- [ ] Multi-Device Support
- [ ] Cloud Sync

---

## 🐛 Troubleshooting

### Web App ไม่เชื่อมต่อ
- ✅ ตรวจสอบว่า ESP32 เชื่อมต่อ Wi-Fi แล้ว
- ✅ ตรวจสอบว่าอยู่ในเครือข่ายเดียวกัน
- ✅ ดู IP Address จาก Serial Monitor

### Bluetooth ไม่เจอ
- ✅ เปิด Bluetooth บนมือถือ
- ✅ ใช้ HTTPS หรือ localhost (Web Bluetooth ต้องการ Secure Context)
- ✅ ค้นหาชื่อ "B2SRGB"

### LED ไม่ติด
- ✅ ตรวจสอบการเชื่อมต่อสายไฟ
- ✅ ตรวจสอบ LED_PIN ในโค้ด
- ✅ ตรวจสอบ Power Supply
- ✅ ลองปรับ COLOR_ORDER (GRB, RGB, BRG)

---

## 📖 Documentation

- [📱 Web App Guide](docs/webapp-guide.md) _(Coming Soon)_
- [🔌 Firmware Guide](firmware/README.md) ✅
- [🔧 API Reference](docs/api-reference.md) _(Coming Soon)_
- [🎨 Custom Patterns](docs/custom-patterns.md) _(Coming Soon)_

---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## 📄 License

This project is open source and available under the [MIT License](LICENSE).

---

## 📞 Contact

- **GitHub**: [@johnnckr](https://github.com/johnnckr)
- **Project Link**: [https://github.com/johnnckr/B2SRGB-Controller](https://github.com/johnnckr/B2SRGB-Controller)
- **Issues**: [Report a bug](https://github.com/johnnckr/B2SRGB-Controller/issues)

---

<div align="center">

**สร้างด้วย ❤️ สำหรับผู้ที่รัก RGB LED**

⭐ ถ้าชอบโปรเจ็กต์นี้ อย่าลืมกด Star นะครับ! ⭐

</div>
