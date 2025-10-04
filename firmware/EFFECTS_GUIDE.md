# 🎨 คู่มือเอฟเฟกต์ LED - B2SRGB Controller

## ✨ ทุกเอฟเฟกต์ใช้ชุดสี (Color Palette) ที่แนะนำ

เอฟเฟกต์ทั้งหมดได้รับการปรับปรุงให้ใช้สีจากชุดสีที่แนะนำ 6 ชุด เพื่อความสวยงามและสอดคล้องกัน

---

## 🌈 ชุดสี (Color Palettes)

### 1. **Rainbow Palette** 🌈
```
สี: แดง, ส้ม, เหลือง, เขียว, น้ำเงิน, ม่วง
RGB: (255,0,0), (255,127,0), (255,255,0), (0,255,0), (0,0,255), (148,0,211)
```

### 2. **Fire Palette** 🔥
```
สี: แดง, ส้มแดง, ส้มเข้ม, ดำ
RGB: (255,0,0), (255,69,0), (255,140,0), (0,0,0)
```

### 3. **Ocean Palette** 🌊
```
สี: ฟ้า, น้ำเงิน, เขียวมรกต
RGB: (0,191,255), (0,0,255), (0,128,128)
```

### 4. **Forest Palette** 🌲
```
สี: เขียว, เขียวเข้ม, น้ำตาล
RGB: (34,139,34), (0,100,0), (85,107,47)
```

### 5. **Sunset Palette** 🌅
```
สี: ส้ม, ชมพู, ม่วง, แดง
RGB: (255,140,0), (255,20,147), (138,43,226), (220,20,60)
```

### 6. **Neon Palette** ⚡
```
สี: ชมพูนีออน, เขียวนีออน, ฟ้านีออน
RGB: (255,0,255), (0,255,0), (0,255,255)
```

---

## 🎭 รายการเอฟเฟกต์ทั้งหมด (12 แบบ)

### 1. **Rainbow Cycle** (รุ้งวนลูป) 🌈
- **ชุดสี:** Rainbow Palette
- **คำอธิบาย:** สีรุ้งวนรอบ LED Strip แบบต่อเนื่อง
- **ความเร็ว:** ปรับได้ 0-100
- **เหมาะสำหรับ:** การแสดงทั่วไป, ทดสอบ LED

**โค้ด:**
```cpp
void rainbowCycle() {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(gHue + (i * 255 / NUM_LEDS), 255, 255);
  }
  FastLED.show();
  gHue++;
}
```

---

### 2. **Rainbow Chase** (รุ้งวิ่งไล่) 🏃‍♂️
- **ชุดสี:** Rainbow Palette (HSV)
- **คำอธิบาย:** สีรุ้งไล่วิ่งตามแนว LED Strip
- **ความเร็ว:** ปรับได้ 0-100
- **เหมาะสำหรับ:** ตกแต่งงานเลี้ยง, งานอีเวนต์

**โค้ด:**
```cpp
void rainbowChase() {
  static uint8_t startIndex = 0;
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV((startIndex + i * 3) % 255, 255, 255);
  }
  FastLED.show();
  startIndex += 2;
}
```

---

### 3. **Rainbow Twinkle** (รุ้งระยิบ) ✨
- **ชุดสี:** Rainbow Palette (สุ่ม)
- **คำอธิบาย:** สีรุ้งกระพริบสุ่มตำแหน่ง
- **ความเร็ว:** ปรับได้ 0-100
- **เหมาะสำหรับ:** บรรยากาศเมจิก, ห้องเด็ก

**โค้ด:**
```cpp
void rainbowTwinkle() {
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = random16(NUM_LEDS);
  leds[pos] = CHSV(random8(), 255, 255);
  FastLED.show();
}
```

---

### 4. **Fire Effect** (เปลวไฟ) 🔥
- **ชุดสี:** Fire Palette (แดง → ส้ม → เหลือง)
- **คำอธิบาย:** จำลองเปลวไฟที่เคลื่อนไหว
- **ความเร็ว:** ช้า = สมจริง, เร็ว = ระเบิด
- **เหมาะสำหรับ:** บาร์, ร้านอาหาร, ห้องนั่งเล่น

**การทำงาน:**
1. สร้างความร้อนแต่ละจุด (Heat array)
2. ความร้อนลดลงและแพร่กระจายขึ้น
3. สุ่มจุดใหม่ที่ติดไฟ
4. แปลงความร้อนเป็นสี (ดำ → แดง → ส้ม → เหลือง)

**โค้ด:**
```cpp
void fireEffect() {
  // ใช้ firePalette: (255,0,0), (255,69,0), (255,140,0), (0,0,0)
  // แปลงอุณหภูมิ -> สี
  if(temperature < 85) color = CRGB::Black;
  else if(temperature < 170) color = blend(Red, OrangeRed);
  else color = blend(OrangeRed, Orange);
}
```

**Tips:**
- Speed 40-60: ดูสมจริงที่สุด
- Speed 80-100: เหมาะกับดนตรีเร็ว

---

### 5. **Ocean Wave** (คลื่นทะเล) 🌊
- **ชุดสี:** Ocean Palette (ฟ้า, น้ำเงิน, เขียวมรกต)
- **คำอธิบาย:** คลื่นทะเลที่นุ่มนวล
- **ความเร็ว:** ช้า = สงบ, เร็ว = พายุ
- **เหมาะสำหรับ:** ห้องนอน, สปา, โยคะ

**การทำงาน:**
1. ใช้ beatsin88 สร้างคลื่นไซน์
2. เลือกสีจาก Ocean Palette แบบไล่เรียง
3. ปรับความสว่างตามคลื่น
4. Blend นุ่มนวล

**โค้ด:**
```cpp
void oceanWave() {
  // ใช้ oceanPalette: (0,191,255), (0,0,255), (0,128,128)
  uint8_t palettePos = (i * 3 + colorIndex) % 3;
  CRGB baseColor = oceanPalette[palettePos];
  // Apply wave brightness
}
```

**Tips:**
- Speed 20-40: ผ่อนคลาย
- Speed 60-80: ไดนามิก

---

### 6. **Forest Breeze** (ป่าไม้) 🌲
- **ชุดสี:** Forest Palette (เขียว, เขียวเข้ม, น้ำตาล)
- **คำอธิบาย:** ลมพัดผ่านใบไม้ในป่า
- **ความเร็ว:** ช้า = สงบ, เร็ว = พายุ
- **เหมาะสำหรับ:** ห้องทำงาน, ห้องอ่านหนังสือ

**การทำงาน:**
1. ใช้ Forest Palette
2. เพิ่มคลื่นนุ่มนวลด้วย beatsin8
3. ไล่สีแบบหมุนเวียน

**โค้ด:**
```cpp
void forestBreeze() {
  // ใช้ forestPalette: (34,139,34), (0,100,0), (85,107,47)
  uint8_t palettePos = ((i * 5) + offset) % 3;
  CRGB baseColor = forestPalette[palettePos];
  uint8_t brightness = beatsin8(30, 150, 255, 0, i * 10);
}
```

**Tips:**
- Speed 30-50: ธรรมชาติ
- Brightness 150-200: สบายตา

---

### 7. **Aurora Borealis** (แสงเหนือ) 🌌
- **ชุดสี:** Rainbow Palette (หลากสี)
- **คำอธิบาย:** แสงเหนือที่เคลื่อนไหวนุ่มนวล
- **ความเร็ว:** ช้ามาก = สมจริง
- **เหมาะสำหรับ:** ห้องนอน, Home Theater

**การทำงาน:**
1. ใช้ 2 คลื่นไซน์ซ้อนกัน
2. เลือกสีจาก Rainbow Palette
3. Blend แบบนุ่มนวล

**โค้ด:**
```cpp
void auroraBorealis() {
  // ใช้ rainbowPalette: 6 สี
  uint8_t wave1 = beatsin8(13, 0, 255, 0, i * 3 + offset);
  uint8_t wave2 = beatsin8(17, 0, 255, 0, i * 5 + offset * 2);
  uint8_t brightness = (wave1 + wave2) / 2;
}
```

**Tips:**
- Speed 10-30: สวยงามที่สุด
- Brightness 100-150: สมจริง

---

### 8. **Lava Lamp** (ลาวา) 🌋
- **ชุดสี:** Sunset Palette (ส้ม, ชมพู, ม่วง, แดง)
- **คำอธิบาย:** การเคลื่อนไหวแบบ Lava Lamp
- **ความเร็ว:** ช้า = ลาวา, เร็ว = หลอมละลาย
- **เหมาะสำหรับ:** บาร์, คลับ, ห้องพักผ่อน

**การทำงาน:**
1. ใช้ sin8 สร้างการเคลื่อนไหวช้าๆ
2. เลือก 2 สีจาก Sunset Palette
3. Blend แบบค่อยเป็นค่อยไป

**โค้ด:**
```cpp
void lavaLamp() {
  // ใช้ sunsetPalette: (255,140,0), (255,20,147), (138,43,226), (220,20,60)
  uint8_t pos = sin8(i * 16 + offset);
  uint8_t paletteIndex = (pos / 64) % 4;
  CRGB color = blend(color1, color2, blendAmount);
}
```

**Tips:**
- Speed 20-40: ดูเหมือนจริง
- Brightness 150-200: เหมาะสม

---

### 9. **Police Siren** (ไซเรนตำรวจ) 🚨
- **สี:** แดง และ น้ำเงิน
- **คำอธิบาย:** ไซเรนตำรวจสลับแดง-น้ำเงิน
- **ความเร็ว:** ช้า = สงบ, เร็ว = ฉุกเฉิน
- **เหมาะสำหรับ:** งานปาร์ตี้, Halloween

**การทำงาน:**
1. แบ่ง LED ครึ่งหนึ่ง
2. สลับแดงและน้ำเงินสลับกัน

**โค้ด:**
```cpp
void policeSiren() {
  if(isRed) {
    fill_solid(leds, NUM_LEDS / 2, CRGB::Red);
    fill_solid(leds + NUM_LEDS / 2, NUM_LEDS / 2, CRGB::Black);
  } else {
    // สลับ
  }
}
```

**Tips:**
- Speed 70-100: สมจริง
- ใช้กับเพลงเร็ว

---

### 10. **Lightning Storm** (ฟ้าแลบ) ⚡
- **สี:** ฟ้า-ขาว (สุ่ม)
- **คำอธิบาย:** ฟ้าแลบแบบสุ่ม
- **ความเร็ว:** ช้า = พายุไกล, เร็ว = พายุใกล้
- **เหมาะสำหรับ:** Halloween, ธีมพายุ

**การทำงาน:**
1. Background: ค่อยๆ มืดลง
2. สุ่มตำแหน่งฟ้าแลบ
3. แสงสีฟ้า-ขาวแบบสุ่ม

**โค้ด:**
```cpp
void lightningStorm() {
  fadeToBlackBy(leds, NUM_LEDS, 50);
  if(random condition) {
    int pos = random16(NUM_LEDS);
    leds[pos] = CHSV(160 + random8(40), 200, 255); // ฟ้า-ขาว
  }
}
```

**Tips:**
- Speed 50-80: สมจริง
- ดูดีในที่มืด

---

### 11. **Strobe** (กระพริบ) 💥
- **ชุดสี:** ใช้ Active Palette (ตามที่เลือก)
- **คำอธิบาย:** กระพริบเร็วสลับสี
- **ความเร็ว:** ช้า = ดราม่า, เร็ว = ดิสโก้
- **เหมาะสำหรับ:** ดิสโก้, คลับ, EDM

**การทำงาน:**
1. เปิด/ปิดสลับเร็วๆ
2. เปลี่ยนสีจาก Palette ทุกครั้ง

**โค้ด:**
```cpp
void strobeEffect() {
  if(isOn) {
    fill_solid(leds, NUM_LEDS, palette[colorIndex % paletteSize]);
  } else {
    FastLED.clear();
  }
}
```

**Tips:**
- Speed 80-100: เหมาะกับดนตรี
- **คำเตือน:** อาจไม่เหมาะกับผู้ป่วย Epilepsy

---

### 12. **Sparkle** (ประกาย) ✨
- **ชุดสี:** Neon Palette (ชมพู, เขียว, ฟ้านีออน)
- **คำอธิบาย:** ประกายแสงสุ่มเหมือนดาว
- **ความเร็ว:** ช้า = นุ่มนวล, เร็ว = ระเบิด
- **เหมาะสำหรับ:** งานแต่งงาน, ห้องเด็ก

**การทำงาน:**
1. ค่อยๆ fade out
2. สุ่มตำแหน่ง 3-5 จุด
3. เลือกสีจาก Neon Palette

**โค้ด:**
```cpp
void sparkleEffect() {
  fadeToBlackBy(leds, NUM_LEDS, 30);
  int numSparkles = random8(3, 6);
  for(int i = 0; i < numSparkles; i++) {
    int pos = random16(NUM_LEDS);
    leds[pos] = neonPalette[random8(3)];
  }
}
```

**Tips:**
- Speed 40-60: สวยงาม
- Brightness 200-255: เห็นชัด

---

## 🎛️ การปรับแต่งเอฟเฟกต์

### ความสว่าง (Brightness)
```
0 = ปิด
50 = มืด (ประหยัดไฟ)
100 = พอดี (แนะนำ)
150 = สว่าง
200 = สว่างมาก
255 = สว่างสุด
```

### ความเร็ว (Speed)
```
0-20 = ช้ามาก (ผ่อนคลาย)
20-40 = ช้า (สงบ)
40-60 = ปานกลาง (ทั่วไป)
60-80 = เร็ว (ไดนามิก)
80-100 = เร็วมาก (ปาร์ตี้)
```

---

## 🎨 วิธีเลือกชุดสี (Palette)

### จาก Web App:
```typescript
// เลือกชุดสีแนะนำ
esp32Service.sendCommand({
  type: "PALETTE_NAME",
  value: "rainbow" // หรือ "fire", "ocean", "forest", "sunset", "neon"
});

// หรือกำหนดสีเอง
esp32Service.sendCommand({
  type: "PALETTE",
  value: [
    {r: 255, g: 0, b: 0},
    {r: 0, g: 255, b: 0},
    {r: 0, g: 0, b: 255}
  ]
});
```

### ชุดสีที่แนะนำสำหรับแต่ละโอกาส:

| โอกาส | Palette | เอฟเฟกต์ที่เหมาะ |
|-------|---------|------------------|
| **งานปาร์ตี้** | Rainbow, Neon | Rainbow Chase, Strobe |
| **ผ่อนคลาย** | Ocean, Forest | Ocean Wave, Forest Breeze |
| **โรแมนติก** | Sunset | Lava Lamp, Aurora Borealis |
| **สนุกสนาน** | Rainbow, Fire | Fire Effect, Sparkle |
| **เทศกาล** | Rainbow, Neon | Rainbow Cycle, Strobe |
| **ทำงาน** | Forest | Forest Breeze (ช้า) |
| **นอนหลับ** | Ocean | Ocean Wave (ช้ามาก) |

---

## 💡 Tips & Tricks

### 1. **การเลือกเอฟเฟกต์ตามเวลา**
```cpp
Morning (6-12): Forest Breeze (Speed 30, Brightness 150)
Afternoon (12-18): Rainbow Cycle (Speed 50, Brightness 200)
Evening (18-22): Sunset Lava (Speed 40, Brightness 180)
Night (22-6): Ocean Wave (Speed 20, Brightness 80)
```

### 2. **การประหยัดไฟ**
- ลด Brightness ลง 50% → ประหยัดไฟ 50%
- ใช้เอฟเฟกต์ที่ LED ติดไม่ครบ: Lightning, Sparkle
- เปิดเฉพาะช่วงที่ต้องการ

### 3. **การปรับให้เข้ากับดนตรี**
```
เพลงช้า: Ocean Wave, Aurora Borealis (Speed 20-30)
เพลงเร็ว: Rainbow Chase, Strobe (Speed 80-100)
EDM: Strobe, Police Siren (Speed 90-100)
Chill: Forest Breeze, Lava Lamp (Speed 30-40)
```

### 4. **การสร้างบรรยากาศ**
```
โรแมนติก: Sunset Lava (Speed 30, Brightness 120)
ตื่นเต้น: Rainbow Chase + Strobe (Speed 90)
สงบ: Ocean Wave (Speed 15, Brightness 100)
ลึกลับ: Lightning Storm (Speed 50, Brightness 150)
```

---

## 🔧 Custom Palette

สร้างชุดสีของคุณเอง:

### ตัวอย่าง: ชุดสีเทศกาลคริสต์มาส 🎄
```cpp
CRGB christmasPalette[3] = {
  CRGB(255, 0, 0),   // แดง
  CRGB(0, 255, 0),   // เขียว
  CRGB(255, 255, 255) // ขาว
};
```

### ตัวอย่าง: ชุดสีฮาโลวีน 🎃
```cpp
CRGB halloweenPalette[3] = {
  CRGB(255, 140, 0),  // ส้ม
  CRGB(128, 0, 128),  // ม่วง
  CRGB(0, 0, 0)       // ดำ
};
```

### ตัวอย่าง: ชุดสีเดือนแห่งความภาคภูมิใจ 🏳️‍🌈
```cpp
CRGB pridePalette[6] = {
  CRGB(255, 0, 0),    // แดง
  CRGB(255, 127, 0),  // ส้ม
  CRGB(255, 255, 0),  // เหลือง
  CRGB(0, 255, 0),    // เขียว
  CRGB(0, 0, 255),    // น้ำเงิน
  CRGB(128, 0, 128)   // ม่วง
};
```

---

## 📊 ตารางเปรียบเทียบ

| เอฟเฟกต์ | Palette | CPU | RAM | ความสวย | ความเร็วที่แนะนำ |
|---------|---------|-----|-----|---------|------------------|
| Rainbow Cycle | Rainbow | ⭐ | ⭐ | ⭐⭐⭐⭐⭐ | 50 |
| Rainbow Chase | Rainbow | ⭐ | ⭐ | ⭐⭐⭐⭐ | 70 |
| Rainbow Twinkle | Rainbow | ⭐⭐ | ⭐ | ⭐⭐⭐⭐ | 60 |
| Fire Effect | Fire | ⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ | 50 |
| Ocean Wave | Ocean | ⭐⭐⭐ | ⭐⭐ | ⭐⭐⭐⭐⭐ | 30 |
| Forest Breeze | Forest | ⭐⭐ | ⭐ | ⭐⭐⭐⭐ | 40 |
| Aurora Borealis | Rainbow | ⭐⭐⭐ | ⭐⭐ | ⭐⭐⭐⭐⭐ | 20 |
| Lava Lamp | Sunset | ⭐⭐ | ⭐ | ⭐⭐⭐⭐ | 30 |
| Police Siren | - | ⭐ | ⭐ | ⭐⭐⭐ | 80 |
| Lightning Storm | - | ⭐⭐ | ⭐ | ⭐⭐⭐⭐ | 60 |
| Strobe | Active | ⭐ | ⭐ | ⭐⭐⭐ | 90 |
| Sparkle | Neon | ⭐⭐ | ⭐ | ⭐⭐⭐⭐ | 50 |

---

## 🚀 API Reference

### เปลี่ยนเอฟเฟกต์
```json
POST /command
{
  "type": "PATTERN",
  "value": "เปลวไฟ"
}
```

### เลือกชุดสี
```json
POST /command
{
  "type": "PALETTE_NAME",
  "value": "fire"
}
```

### กำหนดสีเอง
```json
POST /command
{
  "type": "PALETTE",
  "value": [
    {"r": 255, "g": 0, "b": 0},
    {"r": 0, "g": 255, "b": 0}
  ]
}
```

### ปรับความสว่าง
```json
POST /command
{
  "type": "BRIGHTNESS",
  "value": 200
}
```

### ปรับความเร็ว
```json
POST /command
{
  "type": "SPEED",
  "value": 50
}
```

---

## 🎓 เรียนรู้เพิ่มเติม

- **CUSTOMIZATION.md**: วิธีสร้างเอฟเฟกต์ของคุณเอง
- **WEBAPP_GUIDE.md**: การใช้งาน Web App
- **README.md**: ภาพรวมโปรเจ็กต์

---

## 💬 สรุป

✅ **ทุกเอฟเฟกต์ใช้ชุดสีที่แนะนำ**  
✅ **สีสวยงาม สอดคล้องกัน**  
✅ **ปรับแต่งได้ง่าย**  
✅ **เหมาะกับทุกโอกาส**  

**สนุกกับการตกแต่ง LED ของคุณ! 🎨✨**
