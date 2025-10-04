import React, { useState } from 'react';

interface Props {
  onClose: () => void;
}

const HelpModal: React.FC<Props> = ({ onClose }) => {
  const [currentStep, setCurrentStep] = useState(0);

  const steps = [
    {
      title: '🔌 เชื่อมต่อ ESP32',
      content: (
        <div className="space-y-4">
          <p className="text-gray-300">เลือกวิธีการเชื่อมต่อ 2 แบบ:</p>
          
          <div className="bg-gray-800 p-4 rounded-lg">
            <h4 className="font-bold text-cyan-400 mb-2">🌐 HTTP (แนะนำ)</h4>
            <ol className="list-decimal list-inside space-y-2 text-sm text-gray-300">
              <li>เปิด Serial Monitor (Ctrl+Alt+S)</li>
              <li>ดู IP Address ของ ESP32</li>
              <li>คลิกปุ่ม "Connect"</li>
              <li>เลือก HTTP → ใส่ IP</li>
            </ol>
            <p className="text-xs text-green-400 mt-2">✅ เสถียร, เร็ว, ทุกเบราว์เซอร์</p>
          </div>

          <div className="bg-gray-800 p-4 rounded-lg">
            <h4 className="font-bold text-blue-400 mb-2">🔵 Bluetooth</h4>
            <ol className="list-decimal list-inside space-y-2 text-sm text-gray-300">
              <li>คลิกปุ่ม "Connect"</li>
              <li>เลือก Bluetooth</li>
              <li>เลือกอุปกรณ์ "B2SRGB"</li>
            </ol>
            <p className="text-xs text-yellow-400 mt-2">⚠️ รองรับเฉพาะ Chrome/Edge</p>
          </div>
        </div>
      )
    },
    {
      title: '🎨 ควบคุม LED',
      content: (
        <div className="space-y-3">
          <div className="bg-gray-800 p-3 rounded-lg">
            <h4 className="font-bold text-cyan-400">🔘 Power</h4>
            <p className="text-sm text-gray-300">เปิด/ปิด LED Strip</p>
          </div>

          <div className="bg-gray-800 p-3 rounded-lg">
            <h4 className="font-bold text-cyan-400">🎭 Mode</h4>
            <ul className="text-sm text-gray-300 list-disc list-inside">
              <li>Static - สีเดียวคงที่</li>
              <li>Rainbow - สีรุ้ง</li>
              <li>Breathing - สว่าง/มืดสลับ</li>
              <li>Music Reactive - ตามเพลง 🎵</li>
            </ul>
          </div>

          <div className="bg-gray-800 p-3 rounded-lg">
            <h4 className="font-bold text-cyan-400">🌟 Pattern (12 แบบ)</h4>
            <p className="text-sm text-gray-300">Rainbow, Fire, Ocean, Forest, Aurora, Lava, และอื่นๆ</p>
          </div>

          <div className="bg-gray-800 p-3 rounded-lg">
            <h4 className="font-bold text-cyan-400">⚙️ ปรับแต่ง</h4>
            <ul className="text-sm text-gray-300 list-disc list-inside">
              <li>☀️ Brightness (ความสว่าง)</li>
              <li>⚡ Speed (ความเร็ว)</li>
              <li>🎨 Color/Palette (สี)</li>
            </ul>
          </div>
        </div>
      )
    },
    {
      title: '🎵 Music Mode',
      content: (
        <div className="space-y-4">
          <p className="text-gray-300">LED เด้งตามจังหวะเพลง!</p>
          
          <div className="bg-gray-800 p-4 rounded-lg space-y-3">
            <h4 className="font-bold text-purple-400">วิธีใช้:</h4>
            <ol className="list-decimal list-inside space-y-2 text-sm text-gray-300">
              <li>เปลี่ยนโหมดเป็น "Music Reactive"</li>
              <li>คลิก "เริ่มซิงค์กับเพลง"</li>
              <li>อนุญาตการใช้ไมโครโฟน</li>
              <li>เปิดเพลง → LED จะเด้งตามจังหวะ!</li>
            </ol>
          </div>

          <div className="bg-yellow-900/30 border border-yellow-600 p-3 rounded-lg">
            <p className="text-sm text-yellow-300">
              <strong>💡 Tips:</strong> เพิ่ม Sensitivity ถ้าเพลงเบาเกินไป
            </p>
          </div>
        </div>
      )
    },
    {
      title: '📱 ติดตั้งเป็น App',
      content: (
        <div className="space-y-4">
          <p className="text-gray-300">ใช้งานเหมือน App จริง!</p>
          
          <div className="bg-gray-800 p-4 rounded-lg">
            <h4 className="font-bold text-green-400 mb-2">📱 iOS (Safari)</h4>
            <ol className="list-decimal list-inside space-y-2 text-sm text-gray-300">
              <li>แตะปุ่ม <strong>Share</strong> (ลูกศรขึ้น)</li>
              <li>เลื่อนหา "Add to Home Screen"</li>
              <li>ตั้งชื่อ → กด Add</li>
            </ol>
          </div>

          <div className="bg-gray-800 p-4 rounded-lg">
            <h4 className="font-bold text-green-400 mb-2">🤖 Android (Chrome)</h4>
            <ol className="list-decimal list-inside space-y-2 text-sm text-gray-300">
              <li>แตะปุ่ม <strong>Menu</strong> (จุด 3 จุด)</li>
              <li>เลือก "Add to Home screen"</li>
              <li>ตั้งชื่อ → กด Add</li>
            </ol>
          </div>

          <div className="bg-green-900/30 border border-green-600 p-3 rounded-lg">
            <p className="text-sm text-green-300">
              <strong>✅ ข้อดี:</strong> เปิดเต็มหน้าจอ, ใช้งาน Offline, รวดเร็ว
            </p>
          </div>
        </div>
      )
    },
    {
      title: '🔧 แก้ปัญหา',
      content: (
        <div className="space-y-3">
          <div className="bg-red-900/30 border border-red-600 p-3 rounded-lg">
            <h4 className="font-bold text-red-400">❌ เชื่อมต่อไม่ได้ (HTTP)</h4>
            <ul className="text-sm text-gray-300 list-disc list-inside mt-2">
              <li>ESP32 เชื่อมต่อ WiFi แล้วหรือยัง?</li>
              <li>อยู่ WiFi เดียวกันหรือไม่?</li>
              <li>IP Address ถูกต้องหรือไม่?</li>
              <li>ลอง ping ESP32</li>
            </ul>
          </div>

          <div className="bg-red-900/30 border border-red-600 p-3 rounded-lg">
            <h4 className="font-bold text-red-400">❌ Bluetooth ไม่ทำงาน</h4>
            <ul className="text-sm text-gray-300 list-disc list-inside mt-2">
              <li>ใช้ Chrome หรือ Edge หรือไม่?</li>
              <li>เปิด Bluetooth บนเครื่องแล้วหรือยัง?</li>
              <li>ลองรีสตาร์ท ESP32</li>
            </ul>
          </div>

          <div className="bg-red-900/30 border border-red-600 p-3 rounded-lg">
            <h4 className="font-bold text-red-400">❌ Music Mode ไม่ทำงาน</h4>
            <ul className="text-sm text-gray-300 list-disc list-inside mt-2">
              <li>อนุญาตการใช้ไมโครโฟนหรือยัง?</li>
              <li>เพิ่ม Sensitivity (ความไว)</li>
              <li>เปิดเพลงให้ดังขึ้น</li>
            </ul>
          </div>
        </div>
      )
    }
  ];

  const nextStep = () => {
    if (currentStep < steps.length - 1) {
      setCurrentStep(currentStep + 1);
    } else {
      onClose();
    }
  };

  const prevStep = () => {
    if (currentStep > 0) {
      setCurrentStep(currentStep - 1);
    }
  };

  return (
    <div className="fixed inset-0 bg-black bg-opacity-75 flex items-center justify-center z-50 p-4">
      <div className="bg-gray-900 rounded-xl max-w-2xl w-full max-h-[90vh] overflow-hidden flex flex-col shadow-2xl border border-cyan-500/30">
        {/* Header */}
        <div className="bg-gradient-to-r from-cyan-600 to-purple-600 p-4 flex justify-between items-center">
          <h2 className="text-xl font-bold text-white">{steps[currentStep].title}</h2>
          <button
            onClick={onClose}
            className="text-white hover:text-gray-300 text-2xl"
            aria-label="Close"
          >
            ×
          </button>
        </div>

        {/* Progress Bar */}
        <div className="bg-gray-800 px-4 py-2">
          <div className="flex gap-2">
            {steps.map((_, index) => (
              <div
                key={index}
                className={`h-2 flex-1 rounded-full transition-all ${
                  index <= currentStep ? 'bg-cyan-500' : 'bg-gray-700'
                }`}
              />
            ))}
          </div>
          <p className="text-xs text-gray-400 mt-2 text-center">
            {currentStep + 1} / {steps.length}
          </p>
        </div>

        {/* Content */}
        <div className="flex-1 overflow-y-auto p-6">
          {steps[currentStep].content}
        </div>

        {/* Footer */}
        <div className="bg-gray-800 p-4 flex justify-between items-center border-t border-gray-700">
          <button
            onClick={prevStep}
            disabled={currentStep === 0}
            className={`px-6 py-2 rounded-lg font-medium transition-all ${
              currentStep === 0
                ? 'bg-gray-700 text-gray-500 cursor-not-allowed'
                : 'bg-gray-700 text-white hover:bg-gray-600'
            }`}
          >
            ← ย้อนกลับ
          </button>
          <button
            onClick={nextStep}
            className="px-6 py-2 bg-gradient-to-r from-cyan-600 to-purple-600 text-white rounded-lg font-medium hover:from-cyan-500 hover:to-purple-500 transition-all"
          >
            {currentStep === steps.length - 1 ? 'เริ่มใช้งาน! 🚀' : 'ถัดไป →'}
          </button>
        </div>
      </div>
    </div>
  );
};

export default HelpModal;
