# 📱 คู่มือการใช้งาน B2SRGB บน iOS

## 🎯 ภาพรวม

iOS ไม่รองรับ Web Bluetooth API แต่ B2SRGB สามารถใช้งานได้เต็มประสิทธิภาพผ่าน **WiFi/HTTP Control** ครับ!

---

## 🚀 วิธีการเชื่อมต่อและใช้งาน

### ขั้นตอนที่ 1: ตั้งค่า ESP32 เชื่อมต่อ WiFi

1. **เปิดไฟ ESP32** (ครั้งแรกจะเข้า Provisioning Mode)
2. **เปิด WiFi Settings บน iPhone/iPad**
3. **เชื่อมต่อ WiFi** ชื่อ: `B2SRGB-XXXXXX-Setup` (XXXXXX = รหัสเครื่อง)
4. **หน้าต่างตั้งค่าจะเด้งขึ้นอัตโนมัติ** (Captive Portal)
5. **เลือก WiFi บ้านของคุณ** และใส่รหัสผ่าน
6. **กด Save** → ESP32 จะรีสตาร์ทและเชื่อมต่อ WiFi

### ขั้นตอนที่ 2: หา IP Address ของ ESP32

#### วิธีที่ 1: ดูจาก Serial Monitor (สำหรับคนที่มี Computer)
```
WiFi connected! IP: 192.168.1.123
```

#### วิธีที่ 2: ดูจาก Router Admin Panel
- เข้า Router Settings (เช่น 192.168.1.1)
- ดู Device List / DHCP Clients
- หาอุปกรณ์ชื่อ `B2SRGB-XXXXXX`

#### วิธีที่ 3: ใช้ Network Scanner App
- ติดตั้ง **Fing** หรือ **Network Analyzer** จาก App Store
- Scan เครือข่าย
- หาอุปกรณ์ที่เป็น ESP32

---

## 🎨 วิธีที่ 1: ใช้ Web App (แนะนำ!)

### เปิดใน Browser
1. **เปิด Safari หรือ Chrome บน iPhone**
2. **พิมพ์:** `http://localhost:3000` (ถ้า Web App รันบน Computer เดียวกัน)
   
   หรือ
   
3. **พิมพ์ Network IP:** `http://192.168.100.6:3000` (แก้เป็น IP ของ Computer ที่รัน Web App)

### เชื่อมต่อ ESP32
1. **กดปุ่ม "เชื่อมต่อ"**
2. **เลือก WiFi**
3. **ใส่ IP Address ของ ESP32** (เช่น 192.168.1.123)
4. **เริ่มควบคุม LED ได้เลย!** 🎉

### ติดตั้งเป็น PWA (เหมือนแอพจริง)
1. **เปิด Web App ใน Safari**
2. **กดปุ่ม Share** (ไอคอนลูกศร)
3. **เลือก "Add to Home Screen"**
4. **กด Add**
5. **ไอคอนจะปรากฏบน Home Screen** → เปิดใช้งานเหมือน Native App!

---

## 🌐 วิธีที่ 2: ใช้ Built-in Control Panel (ง่ายที่สุด!)

### เปิด Control Panel ในตัว ESP32
1. **เปิด Safari บน iPhone/iPad**
2. **พิมพ์ IP Address ของ ESP32** (เช่น `http://192.168.1.123`)
3. **Control Panel จะเปิดขึ้นทันที!** 🎮

### ฟีเจอร์ที่มี:
- ⚡ **เปิด/ปิดไฟ** - ปุ่มสลับใหญ่
- 🎭 **โหมด** - สีเดียว, เปลี่ยนสี, กระพริบ, เอฟเฟกต์, ตามเพลง
- ✨ **เอฟเฟกต์** - รุ้งวนลูป, รุ้งวิ่งไล่, เปลวไฟ, คลื่นทะเล, ประกาย
- 🎨 **Color Picker** - เลือกสีด้วย Native iOS Color Picker
- 💡 **Brightness Slider** - ปรับความสว่าง 0-100%
- ⚡ **Speed Slider** - ปรับความเร็วเอฟเฟกต์
- 📊 **Status Display** - แสดงสถานะปัจจุบัน (อัปเดตทุก 3 วินาที)

### บุ๊กมาร์คเพื่อเข้าใช้งานง่าย:
1. เปิด Control Panel ใน Safari
2. กดปุ่ม Share
3. เลือก "Add Bookmark"
4. บันทึก → เข้าใช้งานได้ทันทีจาก Bookmarks!

---

## 🔥 ฟีเจอร์เต็มรูปแบบใน Web App

### 🎨 **4 โหมดหลัก**
1. **สีเดียว** - เลือกสีที่ต้องการ
2. **เปลี่ยนสี** - ไล่เปลี่ยนสีตาม Palette
3. **กระพริบ** - กระพริบตามจังหวะ
4. **เอฟเฟกต์** - เลือกจาก 12 เอฟเฟกต์สุดเจ๋ง
5. **ตามเพลง** - LED เปลี่ยนสีตามจังหวะเพลง

### ✨ **12 เอฟเฟกต์**
1. 🌈 **รุ้งวนลูป** - Rainbow Cycle
2. 🏃 **รุ้งวิ่งไล่** - Rainbow Chase
3. ✨ **รุ้งระยิบ** - Rainbow Twinkle
4. 🔥 **เปลวไฟ** - Fire Effect
5. 🌊 **คลื่นทะเล** - Ocean Wave
6. 🌲 **ป่าไม้** - Forest Breeze
7. 🌅 **พระอาทิตย์ตก** - Sunset
8. 🌌 **แสงเหนือ** - Aurora Borealis
9. 🍭 **ลูกกวาด** - Candy
10. 🌋 **ลาวา** - Lava Lamp
11. ☄️ **ดาวตก** - Meteor
12. ⚡ **ประกาย** - Sparkle

### 🎛️ **ตัวควบคุม**
- **Brightness** - ปรับความสว่าง 0-100%
- **Speed** - ควบคุมความเร็วเอฟเฟกต์
- **Color Picker** - เลือกสีได้ไม่จำกัด
- **Palette Editor** - สร้างชุดสีเอง

### 🎵 **Music Reactive Mode**
- เปิดไมโครโฟนบนมือถือ
- LED เปลี่ยนสีตามจังหวะเพลง
- ปรับความไวได้

---

## 🆚 เปรียบเทียบวิธีการใช้งาน

| คุณสมบัติ | Built-in Panel | Web App (Full) |
|-----------|----------------|----------------|
| ติดตั้งแอพ | ❌ ไม่ต้อง | ❌ ไม่ต้อง |
| เปิดใน Browser | ✅ Safari | ✅ Safari/Chrome |
| จำนวนเอฟเฟกต์ | 5 เอฟเฟกต์ | 12 เอฟเฟกต์ |
| Color Picker | ✅ Native iOS | ✅ Advanced |
| Music Mode | ❌ | ✅ |
| Palette Editor | ❌ | ✅ |
| Auto Brightness | ❌ | ✅ |
| UI Design | 🟢 Good | 🔥 Excellent |
| ความเร็ว | ⚡ เร็วมาก | ⚡ เร็ว |

**แนะนำ:**
- 🎯 **ใช้งานเบื้องต้น**: Built-in Panel (รวดเร็ว)
- 🚀 **ใช้งานเต็มรูปแบบ**: Web App (ครบฟีเจอร์)

---

## 🛠️ Troubleshooting

### ❌ ปัญหา: เชื่อมต่อ ESP32 ไม่ได้

**สาเหตุที่เป็นไปได้:**

1. **ESP32 ไม่ได้เชื่อมต่อ WiFi**
   - ✅ ตรวจสอบ: ดู Serial Monitor มีข้อความ "WiFi connected!" หรือไม่
   - ✅ แก้ไข: ตั้งค่า WiFi ใหม่ผ่าน Captive Portal

2. **IP Address ผิด**
   - ✅ ตรวจสอบ: ดู IP Address จาก Serial Monitor หรือ Router
   - ✅ แก้ไข: ใช้ IP Address ที่ถูกต้อง

3. **Computer กับ ESP32 อยู่คนละ Network**
   - ✅ ตรวจสอบ: ทั้งสองต้องเชื่อมต่อ WiFi เดียวกัน
   - ✅ แก้ไข: เชื่อมต่อให้อยู่ WiFi เดียวกัน

4. **Firewall บล็อก**
   - ✅ ตรวจสอบ: ปิด Firewall ชั่วคราว
   - ✅ แก้ไข: เพิ่ม Exception สำหรับ port 80

### ❌ ปัญหา: Control Panel ไม่แสดงผล

**แก้ไข:**
1. Clear Browser Cache (Safari → Settings → Clear History and Website Data)
2. ลองเปิดใน Private Browsing Mode
3. Refresh หน้าเว็บ (ดึงลงแล้วปล่อย)

### ❌ ปัญหา: คำสั่งไม่ทำงาน

**แก้ไข:**
1. ตรวจสอบว่า ESP32 ยังเชื่อมต่อ WiFi อยู่
2. ลอง Disconnect แล้ว Connect ใหม่
3. Restart ESP32

---

## 📚 HTTP API Reference (สำหรับนักพัฒนา)

### Base URL
```
http://[ESP32_IP]
```

### Endpoints

#### 1. Get Status
```bash
GET /status

Response:
{
  "power": true,
  "mode": "สีเดียว",
  "pattern": "รุ้งวนลูป",
  "brightness": 50,
  "speed": 50,
  "deviceName": "B2SRGB-AABBCC"
}
```

#### 2. Power Control
```bash
POST /power
Body: state=on / state=off

Response:
{
  "status": "ok",
  "power": true
}
```

#### 3. Mode Control
```bash
POST /mode
Body: mode=สีเดียว

Response:
{
  "status": "ok",
  "mode": "สีเดียว"
}
```

#### 4. Pattern Control
```bash
POST /pattern
Body: pattern=รุ้งวนลูป

Response:
{
  "status": "ok",
  "pattern": "รุ้งวนลูป"
}
```

#### 5. Color Control
```bash
POST /color
Body: r=255&g=0&b=0

Response:
{
  "status": "ok",
  "color": {
    "r": 255,
    "g": 0,
    "b": 0
  }
}
```

#### 6. Brightness Control
```bash
POST /brightness
Body: value=50

Response:
{
  "status": "ok",
  "brightness": 50
}
```

#### 7. Speed Control
```bash
POST /speed
Body: value=75

Response:
{
  "status": "ok",
  "speed": 75
}
```

### ตัวอย่าง cURL Commands

```bash
# เปิดไฟ
curl -X POST http://192.168.1.123/power -d "state=on"

# ตั้งสีแดง
curl -X POST http://192.168.1.123/color -d "r=255&g=0&b=0"

# เปลี่ยนเป็นเอฟเฟกต์รุ้ง
curl -X POST http://192.168.1.123/mode -d "mode=เอฟเฟกต์"
curl -X POST http://192.168.1.123/pattern -d "pattern=รุ้งวนลูป"

# ปรับความสว่าง 75%
curl -X POST http://192.168.1.123/brightness -d "value=75"

# ดูสถานะ
curl http://192.168.1.123/status
```

---

## 💡 เคล็ดลับ

### 1. เพิ่มไอคอนบน Home Screen
- Control Panel สามารถเพิ่มเป็นไอคอนบน Home Screen ได้
- เปิดใช้งานเร็วเหมือนแอพจริง

### 2. ใช้ Siri Shortcuts (Advanced)
- สร้าง Shortcut เรียก HTTP API
- สั่งด้วยเสียง "Hey Siri, เปิดไฟ LED"

### 3. ใช้ Static IP
- ตั้งค่า Static IP ใน Router เพื่อไม่ต้องหา IP ใหม่ทุกครั้ง
- จำ IP ได้ง่าย เช่น 192.168.1.100

### 4. แชร์การควบคุมให้คนอื่น
- แชร์ลิงก์ `http://[ESP32_IP]` ให้เพื่อนในบ้าน
- ทุกคนสามารถควบคุม LED ได้พร้อมกัน

---

## 🎉 สรุป

✅ iOS ใช้งาน B2SRGB ได้เต็มประสิทธิภาพผ่าน WiFi/HTTP  
✅ ไม่ต้องติดตั้งแอพ - ใช้ Safari ได้เลย  
✅ 2 วิธี: Built-in Panel (ง่าย) และ Web App (ครบฟีเจอร์)  
✅ รองรับทุกฟีเจอร์ - เอฟเฟกต์, สี, ความสว่าง, ความเร็ว  
✅ ติดตั้งเป็น PWA ได้ - เหมือน Native App  

---

## 📞 ต้องการความช่วยเหลือ?

- 📖 อ่าน: [APP_GUIDE.md](./APP_GUIDE.md)
- 🔧 อ่าน: [TROUBLESHOOTING.md](./firmware/TROUBLESHOOTING.md)
- 🌐 อ่าน: [MULTI_DEVICE_GUIDE.md](./MULTI_DEVICE_GUIDE.md)

**Happy Lighting! 🎨✨**
