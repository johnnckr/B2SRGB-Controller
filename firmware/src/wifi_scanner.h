#ifndef WIFI_SCANNER_H
#define WIFI_SCANNER_H

#include <WiFi.h>
#include <WebServer.h>

// ฟังก์ชันสำหรับสแกนและส่งรายการ Wi-Fi กลับไปในรูป JSON
void handleWifiScan(WebServer &server) {
  Serial.println("Scanning WiFi networks...");
  int n = WiFi.scanNetworks();
  String json = "[";
  for (int i = 0; i < n; i++) {
    if (i > 0) json += ",";
    json += "{\"ssid\":\"" + WiFi.SSID(i) + "\",";
    json += "\"rssi\":" + String(WiFi.RSSI(i)) + ",";
    json += "\"enc\":" + String(WiFi.encryptionType(i)) + "}";
  }
  json += "]";
  server.send(200, "application/json", json);
  Serial.println("Found " + String(n) + " networks");
}

// HTML Template สำหรับหน้าตั้งค่าที่มีฟีเจอร์สแกน WiFi
const char WIFI_SETUP_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
<title>B2SRGB Wi-Fi Setup</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
  body{font-family: Arial, sans-serif; background-color: #121212; color: #E0E0E0; display: flex; justify-content: center; align-items: center; min-height: 100vh; margin: 0; padding: 15px;}
  .container{background-color: #1E1E1E; padding: 25px; border-radius: 10px; box-shadow: 0 0 15px rgba(0,0,0,0.5); width: 100%; max-width: 400px;}
  h2{color: #03DAC6; text-align: center; margin-bottom: 20px;}
  input{width: 100%; padding: 12px; margin-bottom: 15px; border-radius: 5px; border: 1px solid #333; background-color: #2C2C2C; color: #E0E0E0; box-sizing: border-box;}
  button{width: 100%; padding: 12px; border-radius: 5px; border: none; background-color: #03DAC6; color: #121212; font-weight: bold; font-size: 16px; cursor: pointer; margin-bottom: 10px;}
  button:hover{background-color: #00B8A3;}
  .scan-btn{background-color: #BB86FC;}
  .scan-btn:hover{background-color: #9A67EA;}
  .wifi-list{margin: 15px 0; max-height: 200px; overflow-y: auto; display: none;}
  .wifi-item{padding: 10px; margin-bottom: 8px; background-color: #2C2C2C; border-radius: 5px; cursor: pointer; border: 1px solid #333;}
  .wifi-item:hover{background-color: #3C3C3C; border-color: #03DAC6;}
  .wifi-name{font-weight: bold; color: #03DAC6;}
  .wifi-signal{float: right; color: #888;}
  .loading{text-align: center; color: #888; display: none; margin: 10px 0;}
</style>
</head><body>
<div class="container">
  <h2>ตั้งค่า Wi-Fi สำหรับ B2SRGB</h2>
  <button type="button" class="scan-btn" onclick="scanWifi()">🔍 ค้นหาเครือข่าย Wi-Fi</button>
  <div class="loading" id="loading">กำลังค้นหา...</div>
  <div class="wifi-list" id="wifiList"></div>
  <form action="/save-wifi" method="POST">
    <input type="text" id="ssid" name="ssid" placeholder="ชื่อ Wi-Fi (SSID)" required><br>
    <input type="password" name="pass" placeholder="รหัสผ่าน"><br>
    <button type="submit">บันทึกและเชื่อมต่อ</button>
  </form>
</div>
<script>
function scanWifi() {
  document.getElementById('loading').style.display = 'block';
  document.getElementById('wifiList').style.display = 'none';
  fetch('/scan-wifi')
    .then(response => response.json())
    .then(data => {
      document.getElementById('loading').style.display = 'none';
      let list = document.getElementById('wifiList');
      list.innerHTML = '';
      if(data.length === 0) {
        list.innerHTML = '<div style="text-align:center;color:#888;">ไม่พบเครือข่าย</div>';
      } else {
        data.forEach(network => {
          let signal = network.rssi > -60 ? '📶' : (network.rssi > -75 ? '📶' : '📶');
          let lock = network.enc > 0 ? '🔒' : '🔓';
          let item = document.createElement('div');
          item.className = 'wifi-item';
          item.innerHTML = '<span class="wifi-name">' + network.ssid + '</span> <span class="wifi-signal">' + lock + ' ' + signal + ' (' + network.rssi + ' dBm)</span>';
          item.onclick = function() { document.getElementById('ssid').value = network.ssid; };
          list.appendChild(item);
        });
      }
      list.style.display = 'block';
    })
    .catch(err => {
      document.getElementById('loading').style.display = 'none';
      alert('เกิดข้อผิดพลาด: ' + err);
    });
}
</script>
</body></html>
)rawliteral";

#endif
