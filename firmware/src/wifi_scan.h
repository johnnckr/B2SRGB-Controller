// WiFi Scan Handler
// Add this at the top of B2SRGB_Controller.ino: #include "wifi_scan.h"

void handleWifiScan() {
  Serial.println("Scanning WiFi networks...");
  int n = WiFi.scanNetworks();
  
  String json = "[";
  for (int i = 0; i < n; i++) {
    if (i > 0) json += ",";
    json += "{";
    json += "\"ssid\":\"" + WiFi.SSID(i) + "\",";
    json += "\"rssi\":" + String(WiFi.RSSI(i)) + ",";
    json += "\"encryption\":" + String(WiFi.encryptionType(i));
    json += "}";
  }
  json += "]";
  
  server.send(200, "application/json", json);
  Serial.println("WiFi scan complete. Found " + String(n) + " networks");
}

// Enhanced HTML with WiFi Scanner
const char WIFI_SETUP_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
<title>B2SRGB Wi-Fi Setup</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
  body{font-family: Arial, sans-serif; background-color: #121212; color: #E0E0E0; display: flex; justify-content: center; align-items: center; min-height: 100vh; margin: 0; padding: 10px;}
  .container{background-color: #1E1E1E; padding: 25px; border-radius: 10px; box-shadow: 0 0 15px rgba(0,0,0,0.5); width: 100%; max-width: 400px;}
  h2{color: #03DAC6; text-align: center; margin-top: 0;}
  input{width: 100%; padding: 12px; margin-bottom: 15px; border-radius: 5px; border: 1px solid #333; background-color: #2C2C2C; color: #E0E0E0; box-sizing: border-box;}
  button{width: 100%; padding: 12px; border-radius: 5px; border: none; background-color: #03DAC6; color: #121212; font-weight: bold; font-size: 16px; cursor: pointer; margin-bottom: 10px;}
  button:hover{background-color: #02B8A8;}
  .scan-btn{background-color: #6200EE; color: #E0E0E0;}
  .scan-btn:hover{background-color: #7c2aef;}
  #wifiList{margin-bottom: 15px; max-height: 300px; overflow-y: auto;}
  .wifi-item{background-color: #2C2C2C; padding: 12px; margin-bottom: 8px; border-radius: 5px; cursor: pointer; display: flex; justify-content: space-between; align-items: center; border: 2px solid transparent;}
  .wifi-item:hover{border-color: #03DAC6; background-color: #353535;}
  .wifi-item.selected{border-color: #03DAC6; background-color: #353535;}
  .wifi-name{font-weight: bold; color: #03DAC6;}
  .wifi-signal{font-size: 12px; color: #888;}
  .loading{text-align: center; color: #03DAC6; padding: 20px;}
</style>
<script>
function scanWifi() {
  document.getElementById('wifiList').innerHTML = '<div class="loading">🔍 กำลังค้นหา WiFi...</div>';
  fetch('/scan-wifi')
    .then(response => response.json())
    .then(data => {
      let html = '';
      if(data.length === 0) {
        html = '<div class="loading">ไม่พบเครือข่าย WiFi</div>';
      } else {
        data.forEach(network => {
          let signal = '';
          let rssi = network.rssi;
          if(rssi > -50) signal = '📶 ดีมาก';
          else if(rssi > -60) signal = '📶 ดี';
          else if(rssi > -70) signal = '📶 ปานกลาง';
          else signal = '📶 อ่อน';
          
          let lock = network.encryption !== 0 ? '🔒' : '🔓';
          html += '<div class="wifi-item" onclick="selectWifi(\'' + network.ssid + '\')">';
          html += '<span class="wifi-name">' + lock + ' ' + network.ssid + '</span>';
          html += '<span class="wifi-signal">' + signal + '</span>';
          html += '</div>';
        });
      }
      document.getElementById('wifiList').innerHTML = html;
    })
    .catch(err => {
      document.getElementById('wifiList').innerHTML = '<div class="loading">❌ เกิดข้อผิดพลาดในการสแกน</div>';
    });
}

function selectWifi(ssid) {
  document.getElementById('ssid').value = ssid;
  document.querySelectorAll('.wifi-item').forEach(item => item.classList.remove('selected'));
  event.target.closest('.wifi-item').classList.add('selected');
  document.getElementById('pass').focus();
}

window.onload = function() {
  scanWifi();
};
</script>
</head><body>
<div class="container">
  <h2>🔧 ตั้งค่า Wi-Fi สำหรับ B2SRGB</h2>
  <button class="scan-btn" onclick="scanWifi()">🔍 ค้นหา Wi-Fi ใหม่</button>
  <div id="wifiList">
    <div class="loading">🔍 กำลังค้นหา WiFi...</div>
  </div>
  <form action="/save-wifi" method="POST">
    <input type="text" id="ssid" name="ssid" placeholder="ชื่อ Wi-Fi (SSID)" required><br>
    <input type="password" id="pass" name="pass" placeholder="รหัสผ่าน"><br>
    <button type="submit">💾 บันทึกและเชื่อมต่อ</button>
  </form>
</div>
</body></html>
)rawliteral";

void handleRootWithScan() {
  server.send_P(200, "text/html", WIFI_SETUP_HTML);
}
