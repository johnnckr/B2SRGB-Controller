# 📱 B2SRGB Web App - คู่มือใช้งาน

## 🚀 Progressive Web App สำหรับควบคุม LED RGB

Web App นี้เป็น **Progressive Web App (PWA)** ที่สามารถ:
- ✅ ใช้งานได้ทั้งเบราว์เซอร์และติดตั้งเป็น App
- ✅ เชื่อมต่อผ่าน **HTTP** หรือ **Bluetooth**
- ✅ ซิงค์กับเพลงผ่าน **Music Reactive Mode**
- ✅ ใช้งาน Offline ได้ (หลัง cache)

---

## 🌐 วิธีเปิดใช้งาน

### 1️⃣ เริ่ม Development Server

```bash
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
- ไปที่: `http://localhost:3000/`

**จากมือถือ** (ต้องอยู่ WiFi เดียวกัน):
- เปิดเบราว์เซอร์
- ไปที่: `http://192.168.100.6:3000/` (ใช้ Network URL)

---

## 🔌 การเชื่อมต่อกับ ESP32

### วิธีที่ 1: HTTP Connection (แนะนำ) 🌐

#### ✅ ข้อดี:
- เสถียร ไม่ขาดหาย
- เร็วกว่า Bluetooth
- ทำงานได้ทุกเบราว์เซอร์
- ไม่ต้อง Pair

#### ขั้นตอน:

1. **เช็ค IP Address ของ ESP32**
   ```bash
   # เปิด Serial Monitor ใน VS Code
   Ctrl+Alt+S
   
   # จะเห็นข้อความ:
   WiFi connected! IP: 192.168.1.50
   ```

2. **เชื่อมต่อจาก Web App**
   - คลิกปุ่ม **"Connect"** บนหน้า Web App
   - เลือก **"HTTP"**
   - ใส่ IP Address: `192.168.1.50`
   - กดปุ่ม **"Connect"**

3. **เสร็จแล้ว!** ✅
   - หัวข้อด้านบนจะแสดง "Connected via HTTP"
   - พร้อมควบคุม LED!

---

### วิธีที่ 2: Bluetooth Connection 🔵

#### ✅ ข้อดี:
- ไม่ต้องรู้ IP Address
- เชื่อมต่อตรงกับ ESP32
- ใช้งานได้แม้ไม่มี WiFi

#### ⚠️ ข้อจำกัด:
- รองรับเฉพาะ **Chrome** และ **Edge** (Desktop/Android)
- **ไม่รองรับ:** Firefox, Safari, iOS
- ระยะทำงานจำกัด (~10 เมตร)

#### ขั้นตอน:

1. **ตรวจสอบเบราว์เซอร์**
   ```
   ✅ Chrome (Desktop/Android)
   ✅ Edge (Desktop)
   ✅ Opera (Desktop/Android)
   ❌ Firefox (ไม่รองรับ)
   ❌ Safari (ไม่รองรับ)
   ```

2. **เปิด Bluetooth บนคอมพิวเตอร์/มือถือ**

3. **เชื่อมต่อจาก Web App**
   - คลิกปุ่ม **"Connect"**
   - เลือก **"Bluetooth"**
   - รอหน้าต่าง Bluetooth Pairing เปิด
   - เลือกอุปกรณ์ **"B2SRGB"**
   - กดปุ่ม **"Pair"**

4. **เสร็จแล้ว!** ✅

---

## 🎨 การควบคุม LED

### 🔘 Power Button
- **เปิด/ปิด** LED Strip
- สีเขียว = เปิด, สีแดง = ปิด

### 🎭 Mode Selector (4 โหมด)

| Mode | คำอธิบาย | การใช้งาน |
|------|----------|-----------|
| **Static** | สีเดียวคงที่ | เลือกสีจาก Color Picker |
| **Rainbow** | สีรุ้งหมุนวน | ปรับความเร็วได้ |
| **Breathing** | สว่าง/มืดสลับ | นุ่มนวล ผ่อนคลาย |
| **Music Reactive** | เด้งตามเพลง 🎵 | ต้องอนุญาตไมโครโฟน |

### 🌟 Pattern Selector (12 แบบ)

1. **Rainbow Cycle** - สีรุ้งวนรอบ
2. **Rainbow Chase** - สีรุ้งไล่วิ่ง
3. **Rainbow Twinkle** - สีรุ้งกระพริบ
4. **Fire Effect** 🔥 - เปลวไฟ
5. **Ocean Wave** 🌊 - คลื่นทะเล
6. **Forest Breeze** 🌲 - ป่าไม้
7. **Aurora Borealis** 🌌 - แสงเหนือ
8. **Lava Lamp** 🌋 - ลาวา
9. **Police Siren** 🚨 - ไซเรนตำรวจ
10. **Lightning Storm** ⚡ - ฟ้าแลบ
11. **Strobe** 💥 - กระพริบเร็ว
12. **Sparkle** ✨ - ประกายแสง

### 🎨 Color & Palette

**Color Picker:**
- เลือกสีที่ต้องการ (สำหรับ Static Mode)
- รองรับ 16 ล้านสี

**Color Palette:**
- เลือกชุดสีสำเร็จรูป 6 ชุด:
  - 🌈 Rainbow
  - 🔥 Fire
  - 🌊 Ocean
  - 🌲 Forest
  - 🌅 Sunset
  - ⚡ Neon

### ☀️ Brightness Slider
```
0 = ปิด
50 = มืด (ประหยัดไฟ)
80 = พอดี (แนะนำ)
100 = สว่างสุด
```

### ⚡ Speed Slider
```
0 = ช้าที่สุด
50 = ปานกลาง
100 = เร็วที่สุด
```

---

## 🎵 Music Reactive Mode

### วิธีใช้:

1. **เปลี่ยนโหมด**
   - คลิก Mode Selector
   - เลือก "Music Reactive"

2. **เริ่มซิงค์**
   - คลิกปุ่ม **"เริ่มซิงค์กับเพลง"**
   - เบราว์เซอร์จะขออนุญาตใช้ไมโครโฟน
   - กดปุ่ม **"Allow"** (อนุญาต)

3. **เล่นเพลง**
   - เปิดเพลงจากที่ต้องการ
   - LED จะเด้งตามจังหวะอัตโนมัติ! 🎶

### 🎛️ ปรับแต่ง:

- **Sensitivity** (ความไว): 1-10
  - 1 = ไวน้อย (จังหวะแรงๆ เท่านั้น)
  - 10 = ไวมาก (ทุกเสียง)

### 💡 Tips:
```
เพลงช้า: Sensitivity 3-5
เพลงเร็ว/EDM: Sensitivity 7-9
ถ้าไม่เด้ง: เพิ่ม Sensitivity หรือเปิดเพลงดังขึ้น
```

---

## 📱 ติดตั้งเป็น App (PWA)

### iOS (Safari):

1. เปิด Web App ใน Safari
2. แตะปุ่ม **Share** (กล่องมีลูกศรขึ้น)
3. เลื่อนลงหา **"Add to Home Screen"**
4. ตั้งชื่อ → กด **Add**
5. ไอคอนจะปรากฏบนหน้าจอหลัก

### Android (Chrome):

1. เปิด Web App ใน Chrome
2. แตะปุ่ม **Menu** (จุด 3 จุด)
3. เลือก **"Add to Home screen"**
4. ตั้งชื่อ → กด **Add**
5. ไอคอนจะปรากฏบนหน้าจอหลัก

### ✅ ข้อดี:
- เปิดเหมือน App จริง
- เต็มหน้าจอ (ไม่มีแถบเบราว์เซอร์)
- ใช้งานแบบ Offline ได้ (หลังจาก Cache)
- เข้าถึงเร็ว
- ไม่กินพื้นที่มาก

---

## 🔧 แก้ปัญหา (Troubleshooting)

### ❌ ปัญหา: เชื่อมต่อ HTTP ไม่ได้

**อาการ:**
- กด Connect แล้วไม่มีอะไรเกิดขึ้น
- แสดง "Connection failed"

**วิธีแก้:**

1. **เช็คว่า ESP32 เชื่อมต่อ WiFi แล้ว**
   ```bash
   # เปิด Serial Monitor
   Ctrl+Alt+S
   
   # ต้องเห็นข้อความ:
   WiFi connected! IP: 192.168.1.50
   ```

2. **เช็คว่าคอมกับ ESP32 อยู่ WiFi เดียวกัน**
   ```
   คอม: Settings → WiFi
   ESP32: ดูที่ Serial Monitor
   ```

3. **ลอง Ping ESP32**
   ```powershell
   ping 192.168.1.50
   
   # ถ้า Ping ผ่าน = OK
   # ถ้า Ping ไม่ผ่าน = ไม่ได้เชื่อมต่อ
   ```

4. **ตั้งค่า WiFi ใหม่**
   ```
   1. ถอดสาย USB จาก ESP32
   2. กดปุ่ม BOOT ค้างไว้
   3. เสียบสาย USB กลับ
   4. ปล่อยปุ่ม BOOT
   5. เชื่อมต่อ WiFi "B2SRGB_Setup"
   6. Captive Portal จะเปิด
   7. ตั้งค่า WiFi ใหม่
   ```

---

### ❌ ปัญหา: Bluetooth ไม่ทำงาน

**อาการ:**
- ไม่เห็นหน้าต่าง Bluetooth Pairing
- แสดง "Web Bluetooth API is not available"

**วิธีแก้:**

1. **เช็คเบราว์เซอร์**
   ```
   ✅ ใช้ Chrome หรือ Edge
   ❌ ห้ามใช้ Firefox หรือ Safari
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

### ❌ ปัญหา: Music Mode ไม่ทำงาน

**อาการ:**
- คลิก "เริ่มซิงค์" แล้วไม่มีอะไรเกิดขึ้น
- LED ไม่เด้งตามเพลง

**วิธีแก้:**

1. **อนุญาตการใช้ไมโครโฟน**
   ```
   เบราว์เซอร์จะขออนุญาต → กด "Allow"
   
   ถ้าไม่เห็นหน้าต่าง:
   Chrome Settings → Privacy and security
   → Site Settings → Microphone → Allow
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

**อาการ:**
- LED ไม่ติดเลย
- สีไม่ตรงกับที่เลือก
- กระพริบผิดปกติ

**วิธีแก้:**

1. **เช็คการเชื่อมต่อสาย**
   - ดูคู่มือ: `firmware/WIRING_GUIDE.md`
   - เช็ค Data pin, Power, Ground

2. **ลอง Test Pattern**
   ```
   เลือก Mode: Effects
   เลือก Pattern: Rainbow Cycle
   ถ้าเห็นสีรุ้ง → OK
   ```

3. **เช็คพารามิเตอร์ใน Firmware**
   ```cpp
   #define NUM_LEDS 1200     // ตรวจสอบจำนวน LED
   #define LED_PIN 5         // ตรวจสอบ GPIO pin
   #define LED_TYPE WS2812B  // ตรวจสอบประเภท LED
   ```

4. **ลดจำนวน LED**
   ```cpp
   // ลองเริ่มจากจำนวนน้อยก่อน
   #define NUM_LEDS 60  // แทน 1200
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

## 🔒 Security & Privacy

### 🔐 การเชื่อมต่อ:
- HTTP: ไม่เข้ารหัส (ใช้ใน LAN เท่านั้น)
- Bluetooth: เข้ารหัสด้วย BLE encryption

### 🎤 ไมโครโฟน:
- ใช้เฉพาะใน Music Mode
- ไม่มีการบันทึกหรือส่งข้อมูล
- ประมวลผลในเครื่องเท่านั้น

### 💾 ข้อมูล:
- ไม่มีการเก็บข้อมูลส่วนบุคคล
- ไม่มี Analytics หรือ Tracking
- ทำงานแบบ Offline ได้

---

## 📊 System Requirements

### เบราว์เซอร์:

| Feature | Chrome | Edge | Firefox | Safari |
|---------|--------|------|---------|--------|
| HTTP Connection | ✅ | ✅ | ✅ | ✅ |
| Bluetooth | ✅ | ✅ | ❌ | ❌ |
| Music Mode | ✅ | ✅ | ✅ | ✅ |
| PWA Install | ✅ | ✅ | ❌ | ✅ |
| Offline | ✅ | ✅ | ✅ | ✅ |

### อุปกรณ์:

- **Desktop:** Windows 10+, macOS 10.15+, Linux
- **Mobile:** Android 8.0+, iOS 13.0+
- **RAM:** 512 MB+
- **Network:** WiFi หรือ Bluetooth 4.0+

---

## 💡 Tips & Tricks

### 1. **ประหยัดแบตเตอรี่ (มือถือ)**
```
- ลด Brightness ลง 50%
- ใช้ Static Mode แทน Effects
- ปิด Music Mode เมื่อไม่ใช้
```

### 2. **การใช้งานในงานปาร์ตี้**
```
Mode: Music Reactive
Sensitivity: 8
Speed: 80
Pattern: Strobe / Rainbow Chase
→ LED จะเด้งตามดนตรีสนุกมาก!
```

### 3. **การใช้งานผ่อนคลาย**
```
Mode: Effects
Pattern: Ocean Wave / Aurora Borealis
Speed: 20
Brightness: 50
→ นุ่มนวล สบายตา
```

### 4. **ใช้จากหลายอุปกรณ์พร้อมกัน**
```
เปิด Web App จากมือถือและแท็บเล็ตได้พร้อมกัน
(ต้องเชื่อมต่อ WiFi เดียวกัน)
```

---

## 📚 เอกสารเพิ่มเติม

- **Firmware**: [firmware/README.md](firmware/README.md)
- **Effects Guide**: [firmware/EFFECTS_GUIDE.md](firmware/EFFECTS_GUIDE.md)
- **Wiring Guide**: [firmware/WIRING_GUIDE.md](firmware/WIRING_GUIDE.md)
- **Troubleshooting**: [firmware/TROUBLESHOOTING.md](firmware/TROUBLESHOOTING.md)

---

## 💬 ติดต่อและสนับสนุน

- 📧 Email: [your-email@example.com](mailto:your-email@example.com)
- 🐛 Issues: [GitHub Issues](https://github.com/johnnckr/B2SRGB-Controller/issues)
- 💬 Discussions: [GitHub Discussions](https://github.com/johnnckr/B2SRGB-Controller/discussions)
- 📞 โทร: [02097959398](tel:02097959398)
- 📘 Facebook: [NeeKPS](https://www.facebook.com/NeeKPS)

---

## ⭐ ชอบโปรเจ็กต์นี้?

- ⭐ Star บน GitHub
- 🍴 Fork ไปพัฒนาต่อ
- 📢 แชร์ให้เพื่อนๆ

**Happy Controlling! 🎨✨**
