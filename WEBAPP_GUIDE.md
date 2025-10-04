# 📱 คู่มือการใช้งาน Web App - B2SRGB Controller

## 🚀 วิธีเปิดใช้งาน Web App

### 1️⃣ เริ่มต้น Development Server

```bash
# เปิด Terminal ใน VS Code
cd d:\9.15\B2SRGB
npm run dev
```

**ผลลัพธ์:**
```
VITE v6.3.6  ready in 2720 ms

➜  Local:   http://localhost:3000/
➜  Network: http://192.168.100.6:3000/
```

### 2️⃣ เปิดในเบราว์เซอร์

**จากคอมพิวเตอร์:**
- เปิด Chrome/Edge/Firefox
- ไปที่: http://localhost:3000/

**จากมือถือ** (ต้องอยู่ WiFi เดียวกัน):
- เปิดเบราว์เซอร์
- ไปที่: http://192.168.100.6:3000/ (ใช้ Network URL)

---

## 🔌 การเชื่อมต่อกับ ESP32

### วิธีที่ 1: HTTP Connection (แนะนำ) 🌐

#### ขั้นตอน:

1. **เปิด Serial Monitor ใน VS Code**
   ```
   Ctrl+Alt+S
   ```

2. **ดู IP Address ของ ESP32**
   ```
   WiFi connected! IP: 192.168.1.50  ← คัดลอก IP นี้
   BLE server started. Device name: B2SRGB
   ```

3. **เชื่อมต่อจาก Web App**
   - คลิกปุ่ม **"Connect"** บนหน้า Web App
   - เลือก **"HTTP"**
   - ใส่ IP Address ที่เห็นใน Serial Monitor
   - กดปุ่ม **"Connect"**

#### ✅ ข้อดี:
- เสถียร ไม่ขาดหาย
- ใช้งานได้ทั้งคอมและมือถือ
- ไม่ต้อง Pair
- ตอบสนองเร็ว

---

### วิธีที่ 2: Bluetooth Connection 🔵

#### ขั้นตอน:

1. **เปิด Bluetooth บนคอมพิวเตอร์/มือถือ**

2. **เชื่อมต่อจาก Web App**
   - คลิกปุ่ม **"Connect"**
   - เลือก **"Bluetooth"**
   - รอหน้าต่าง Bluetooth Pairing เปิด
   - เลือกอุปกรณ์ **"B2SRGB"**
   - กดปุ่ม **"Pair"**

#### ✅ ข้อดี:
- ไม่ต้องรู้ IP Address
- ใช้งานได้แม้ไม่มี WiFi
- เชื่อมต่อตรงกับ ESP32

#### ⚠️ ข้อจำกัด:
- รองรับเฉพาะเบราว์เซอร์ Chrome, Edge (ที่มี Web Bluetooth API)
- ระยะทำงานจำกัด (ประมาณ 10 เมตร)
- บางครั้งอาจขาดการเชื่อมต่อ

---

## 🎮 การควบคุม LED Strip

### 🔘 Power Button
```
เปิด/ปิด LED Strip
```

### 🎨 Mode Selector

| Mode | คำอธิบาย |
|------|----------|
| **Static** | สีเดียวคงที่ (เลือกสีได้) |
| **Rainbow** | สีรุ้งหมุนวน |
| **Breathing** | สว่าง/มืดสลับนุ่มนวล |
| **Music Reactive** | เด้งตามจังหวะเพลง 🎵 |

---

### 🌈 Pattern Selector (12 แบบ)

| # | Pattern | คำอธิบาย |
|---|---------|----------|
| 1 | **Rainbow Cycle** | สีรุ้งวนรอบ LED Strip |
| 2 | **Rainbow Chase** | สีรุ้งไล่วิ่ง |
| 3 | **Rainbow Twinkle** | สีรุ้งกระพริบสุ่ม |
| 4 | **Fire Effect** | เปลวไฟสีส้ม-แดง |
| 5 | **Ocean Wave** | คลื่นสีฟ้า-เขียว |
| 6 | **Forest Breeze** | สีเขียวธรรมชาติ |
| 7 | **Aurora Borealis** | แสงเหนือ หลากสี |
| 8 | **Lava Lamp** | ลาวาสีส้ม-ม่วง |
| 9 | **Police Siren** | ไซเรนสีแดง-น้ำเงิน |
| 10 | **Lightning Storm** | ฟ้าแลบสีฟ้า-ขาว |
| 11 | **Strobe** | กระพริบเร็ว |
| 12 | **Sparkle** | ประกายแสงสุ่ม |

---

### 🎨 Color Picker
```
เลือกสีที่ต้องการ (ใช้ได้กับ Static Mode)
- คลิกที่วงสี
- ปรับความสว่าง/ความอิ่มตัว
- สีจะเปลี่ยนทันที
```

---

### 🎨 Color Palette (ชุดสีสำเร็จรูป)

| Palette | สี |
|---------|-----|
| **Rainbow** | 🌈 แดง, ส้ม, เหลือง, เขียว, น้ำเงิน, ม่วง |
| **Fire** | 🔥 แดง, ส้ม, เหลือง, ดำ |
| **Ocean** | 🌊 ฟ้า, น้ำเงิน, เขียวมรกต |
| **Forest** | 🌲 เขียว, เขียวเข้ม, น้ำตาล |
| **Sunset** | 🌅 ส้ม, ชมพู, ม่วง, แดง |
| **Neon** | ⚡ ชมพูนีออน, เขียวนีออน, ฟ้านีออน |

---

### ☀️ Brightness Slider
```
ปรับความสว่าง: 0 - 255
- 0 = ปิด
- 255 = สว่างสุด
```

---

### ⚡ Speed Slider
```
ปรับความเร็วของ Animation: 1 - 100
- 1 = ช้าที่สุด
- 100 = เร็วที่สุด
```

---

### 🔊 Music Reactive Mode

#### วิธีใช้:

1. **เปลี่ยนโหมดเป็น Music Reactive**
   - เลือก Mode → Music Reactive

2. **อนุญาตการใช้ไมโครโฟน**
   - เบราว์เซอร์จะขออนุญาต
   - กดปุ่ม "Allow"

3. **เล่นเพลง**
   - เปิดเพลงจากที่ต้องการ
   - LED จะเด้งตามจังหวะอัตโนมัติ!

#### 🎛️ ปรับแต่ง:
```
Sensitivity: ความไวในการจับจังหวะ (1-10)
- 1 = ไวน้อย (จังหวะแรงๆ เท่านั้น)
- 10 = ไวมาก (ทุกเสียง)

Mode: แบบการแสดงผล
- Bars: แท่งกราฟ
- Wave: คลื่น
- Pulse: เต้นตามจังหวะ
```

---

## 🔧 Troubleshooting

### ❌ ปัญหา: เชื่อมต่อ ESP32 ไม่ได้ (HTTP)

**วิธีแก้:**

1. **เช็คว่า ESP32 เชื่อมต่อ WiFi แล้ว**
   ```
   Ctrl+Alt+S  → เปิด Serial Monitor
   ดูว่ามีข้อความ "WiFi connected! IP: ..."
   ```

2. **เช็คว่าคอมพิวเตอร์กับ ESP32 อยู่ WiFi เดียวกัน**
   ```
   คอม: เช็คที่ WiFi Settings
   ESP32: เช็คที่ Serial Monitor
   ```

3. **ลอง Ping ESP32**
   ```powershell
   ping 192.168.1.50  # ใส่ IP ของ ESP32
   ```
   ถ้า Ping ไม่ผ่าน → ESP32 ไม่ได้เชื่อมต่อ WiFi

4. **ตั้งค่า WiFi ใหม่**
   ```
   1. ถอดสาย USB จาก ESP32
   2. กดปุ่ม BOOT ค้างไว้
   3. เสียบสาย USB กลับ
   4. ปล่อยปุ่ม BOOT
   5. เชื่อมต่อ WiFi "B2SRGB_Setup"
   6. Captive Portal จะเปิด → ตั้งค่า WiFi ใหม่
   ```

---

### ❌ ปัญหา: เชื่อมต่อ Bluetooth ไม่ได้

**วิธีแก้:**

1. **เช็คว่าเบราว์เซอร์รองรับ Web Bluetooth**
   ```
   ✅ Chrome (Desktop/Android)
   ✅ Edge (Desktop)
   ❌ Firefox (ไม่รองรับ)
   ❌ Safari (ไม่รองรับ)
   ```

2. **เปิด Bluetooth บนคอมพิวเตอร์**
   ```
   Windows: Settings → Bluetooth & devices → เปิด
   ```

3. **อนุญาต Chrome ใช้ Bluetooth**
   ```
   Chrome Settings → Privacy and security
   → Site Settings → Bluetooth → Allow
   ```

4. **รีสตาร์ท ESP32**
   ```
   กดปุ่ม RESET หรือ ถอด-เสียบสาย USB
   ```

---

### ❌ ปัญหา: Music Reactive Mode ไม่ทำงาน

**วิธีแก้:**

1. **อนุญาตการใช้ไมโครโฟน**
   ```
   เบราว์เซอร์จะขออนุญาต → กด "Allow"
   ```

2. **เช็คว่าไมโครโฟนทำงาน**
   ```
   Windows: Settings → Privacy → Microphone
   → Allow apps to access microphone
   ```

3. **เพิ่มความไว (Sensitivity)**
   ```
   ปรับ Sensitivity Slider ขึ้นเป็น 7-10
   ```

4. **เปิดเพลงให้ดังขึ้น**
   ```
   ถ้าเสียงเบาเกินไป LED จะไม่เต้น
   ```

---

### ❌ ปัญหา: LED ไม่ติด หรือ สีผิด

**วิธีแก้:**

1. **เช็คการเชื่อมต่อสาย**
   ```
   ดู WIRING_GUIDE.md
   ```

2. **ลอง Test Pattern**
   ```
   เลือก Pattern: Rainbow Cycle
   ถ้าเห็นสีรุ้ง → OK
   ```

3. **เช็คพารามิเตอร์ใน Firmware**
   ```cpp
   #define NUM_LEDS 60      // ตรวจสอบจำนวน LED
   #define LED_PIN 13       // ตรวจสอบ GPIO pin
   #define LED_TYPE WS2812B // ตรวจสอบประเภท LED
   ```

---

## 📱 สร้าง PWA (Progressive Web App)

### ติดตั้งบนมือถือ:

#### iOS (Safari):
1. เปิด Web App ใน Safari
2. แตะปุ่ม **Share** (กล่องมีลูกศรขึ้น)
3. เลื่อนลงหา **"Add to Home Screen"**
4. ตั้งชื่อ → กด **Add**
5. ไอคอนจะปรากฏบนหน้าจอหลัก

#### Android (Chrome):
1. เปิด Web App ใน Chrome
2. แตะปุ่ม **Menu** (จุด 3 จุด)
3. เลือก **"Add to Home screen"**
4. ตั้งชื่อ → กด **Add**
5. ไอคอนจะปรากฏบนหน้าจอหลัก

#### ✅ ข้อดี:
- เปิดเหมือน App จริง
- เต็มหน้าจอ (ไม่มีแถบเบราว์เซอร์)
- ใช้งานแบบ Offline ได้ (หลังจาก Cache)
- รวดเร็ว

---

## 🎯 Tips & Tricks

### 💡 เคล็ดลับการใช้งาน

1. **สร้าง Preset สี**
   ```
   บันทึกชุดสีที่ชอบ → ใช้ Color Palette
   ```

2. **ปรับความเร็วให้เหมาะสม**
   ```
   Fire Effect → Speed 40-60 (ช้า ดูเหมือนจริง)
   Rainbow Chase → Speed 70-90 (เร็ว ดูสนุก)
   ```

3. **ประหยัดไฟ**
   ```
   ลด Brightness ลง 50% → ประหยัดไฟ 50%
   ```

4. **ใช้ Music Mode กับปาร์ตี้**
   ```
   Sensitivity: 8
   Mode: Bars
   Speed: 80
   → LED จะเด้งตามดนตรีสนุกมาก!
   ```

---

## 🔐 Security

### การรักษาความปลอดภัย:

1. **เปลี่ยนชื่อ WiFi AP** (ใน Firmware)
   ```cpp
   const char* apSSID = "MyLED_Setup";  // เปลี่ยนชื่อ
   const char* apPassword = "12345678"; // ตั้งรหัสผ่าน
   ```

2. **ตั้งรหัสผ่าน Captive Portal**
   ```cpp
   // เพิ่มระบบ Login ใน Captive Portal
   ```

3. **ปิด Bluetooth** (ถ้าไม่ใช้)
   ```cpp
   // Comment บรรทัดนี้
   // setupBLE();
   ```

---

## 📊 API Reference

### HTTP API Endpoints:

```
POST /api/power
Body: {"power": true/false}

POST /api/mode
Body: {"mode": "static|rainbow|breathing|music"}

POST /api/pattern
Body: {"pattern": 1-12}

POST /api/color
Body: {"r": 0-255, "g": 0-255, "b": 0-255}

POST /api/brightness
Body: {"brightness": 0-255}

POST /api/speed
Body: {"speed": 1-100}

GET /api/status
Response: {
  "power": true,
  "mode": "rainbow",
  "pattern": 1,
  "color": {"r": 255, "g": 0, "b": 0},
  "brightness": 200,
  "speed": 50
}
```

---

## 🚀 Build สำหรับ Production

### Build Web App:

```bash
# Build
npm run build

# Preview
npm run preview
```

**Output:**
- ไฟล์จะอยู่ใน `dist/`
- นำไปใส่บน Web Server (Nginx, Apache, Netlify, Vercel)

### Deploy:

#### Netlify:
```bash
netlify deploy --prod --dir=dist
```

#### Vercel:
```bash
vercel --prod
```

#### GitHub Pages:
```bash
npm run build
git add dist -f
git commit -m "Deploy to GitHub Pages"
git subtree push --prefix dist origin gh-pages
```

---

## 📚 เอกสารเพิ่มเติม

- **Firmware**: [firmware/README.md](firmware/README.md)
- **Wiring Guide**: [firmware/WIRING_GUIDE.md](firmware/WIRING_GUIDE.md)
- **Upload Guide**: [firmware/UPLOAD_GUIDE.md](firmware/UPLOAD_GUIDE.md)
- **Troubleshooting**: [firmware/TROUBLESHOOTING.md](firmware/TROUBLESHOOTING.md)
- **Customization**: [firmware/CUSTOMIZATION.md](firmware/CUSTOMIZATION.md)

---

## 💬 Support

มีปัญหาหรือข้อสงสัย?
- 📧 Email: your-email@example.com
- 🐛 Issues: https://github.com/johnnckr/B2SRGB-Controller/issues
- 💬 Discussions: https://github.com/johnnckr/B2SRGB-Controller/discussions

---

## ⭐ ขอบคุณที่ใช้งาน!

ถ้าชอบโปรเจ็กต์นี้ อย่าลืม:
- ⭐ Star บน GitHub
- 🍴 Fork ไปพัฒนาต่อ
- 📢 แชร์ให้เพื่อนๆ

**Happy Coding! 🎨✨**
