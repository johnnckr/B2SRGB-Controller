import React, { useState, useEffect } from 'react';
import { connectionService } from '../services/connectionService';

interface ConnectionModalProps {
  isOpen: boolean;
  onClose: () => void;
  onConnectionChange: (isConnected: boolean) => void;
}

type View = 'initial' | 'connect' | 'provision' | 'bluetooth' | 'success' | 'provision_success';

// --- SVG Icons for internal use ---
const WifiIcon: React.FC<{className?: string}> = ({ className }) => (
    <svg xmlns="http://www.w3.org/2000/svg" className={className} fill="none" viewBox="0 0 24 24" stroke="currentColor">
        <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M8.111 16.555a5.5 5.5 0 017.778 0M12 20.5v.01m-6.364-6.364a9 9 0 0112.728 0M1.636 9.636a12.5 12.5 0 0117.678 0" />
    </svg>
);
const BluetoothIcon: React.FC<{className?: string}> = ({ className }) => (
    <svg xmlns="http://www.w3.org/2000/svg" className={className} fill="none" viewBox="0 0 24 24" stroke="currentColor">
        <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M6.623 8.877l7.754 7.754-3.877 3.877v-15.5l3.877 3.877-7.754 7.754" />
    </svg>
);
const BackArrowIcon: React.FC<{className?: string}> = ({ className }) => (
    <svg xmlns="http://www.w3.org/2000/svg" className={className} fill="none" viewBox="0 0 24 24" stroke="currentColor">
        <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M10 19l-7-7m0 0l7-7m-7 7h18" />
    </svg>
);
const SpinnerIcon: React.FC<{className?: string}> = ({ className }) => (
    <svg className={`animate-spin ${className}`} xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24">
        <circle className="opacity-25" cx="12" cy="12" r="10" stroke="currentColor" strokeWidth="4"></circle>
        <path className="opacity-75" fill="currentColor" d="M4 12a8 8 0 018-8V0C5.373 0 0 5.373 0 12h4zm2 5.291A7.962 7.962 0 014 12H0c0 3.042 1.135 5.824 3 7.938l3-2.647z"></path>
    </svg>
);
const CheckIcon: React.FC<{className?: string}> = ({ className }) => (
    <svg xmlns="http://www.w3.org/2000/svg" className={className} fill="none" viewBox="0 0 24 24" stroke="currentColor">
      <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={3} d="M5 13l4 4L19 7" />
    </svg>
);


const ConnectionModal: React.FC<ConnectionModalProps> = ({ isOpen, onClose, onConnectionChange }) => {
  const [view, setView] = useState<View>('initial');
  const [status, setStatus] = useState<'idle' | 'connecting' | 'error'>('idle');
  const [ipAddress, setIpAddress] = useState('192.168.1.100');
  const [ssid, setSsid] = useState('');
  const [password, setPassword] = useState('');
  const [errorMessage, setErrorMessage] = useState('');
  const [wifiList, setWifiList] = useState<Array<{ssid: string, rssi: number, encryption: string}>>([]);
  const [isScanning, setIsScanning] = useState(false);

  useEffect(() => {
    // Reset state when modal is closed
    if (!isOpen) {
      setTimeout(() => {
        setView('initial');
        setStatus('idle');
        setErrorMessage('');
      }, 300); // Wait for closing animation
    }
  }, [isOpen]);

  useEffect(() => {
    let timer: ReturnType<typeof setTimeout>;
    if (view === 'success') {
      timer = setTimeout(() => {
        onConnectionChange(true);
      }, 1500);
    } else if (view === 'provision_success') {
      timer = setTimeout(() => {
        onClose(); // Close modal, user needs to reconnect wifi
      }, 4000);
    }
    return () => clearTimeout(timer);
  }, [view, onConnectionChange, onClose]);


  const handleClose = () => {
    if (status === 'connecting') return;
    onClose();
  };

  const handleWifiConnect = async () => {
    setStatus('connecting');
    setErrorMessage('');
    const success = await connectionService.connectWifi(ipAddress);
    if (success) {
      setView('success');
    } else {
      setStatus('error');
      setErrorMessage('ไม่สามารถเชื่อมต่อผ่าน Wi-Fi ได้');
    }
  };
  
  const handleScanWifi = async () => {
    setIsScanning(true);
    setErrorMessage('');
    try {
      // เชื่อมต่อไปยัง ESP32 ที่ IP 192.168.4.1 (Provisioning Mode)
      const response = await fetch('http://192.168.4.1/scan-wifi');
      if (response.ok) {
        const data = await response.json();
        setWifiList(data.networks || []);
      } else {
        setErrorMessage('ไม่สามารถสแกน Wi-Fi ได้ กรุณาตรวจสอบว่าเชื่อมต่อกับ ESP32 แล้ว');
      }
    } catch (error) {
      setErrorMessage('ไม่สามารถเชื่อมต่อกับอุปกรณ์ได้ (ต้องเชื่อมต่อ Wi-Fi: B2SRGB-XXXXXX-Setup)');
    } finally {
      setIsScanning(false);
    }
  };

  const handleProvision = async () => {
    if (!ssid) {
        setErrorMessage('กรุณาใส่ชื่อ Wi-Fi');
        return;
    }
    setStatus('connecting');
    setErrorMessage('');
    const success = await connectionService.provisionWifi(ssid, password);
    if (success) {
        setView('provision_success');
    } else {
        setStatus('error');
        setErrorMessage('ตั้งค่าไม่สำเร็จ กรุณาตรวจสอบว่าเชื่อมต่อกับ Wi-Fi ของอุปกรณ์แล้ว');
    }
  };

  const handleBluetoothConnect = async () => {
    setStatus('connecting');
    setErrorMessage('');
    const success = await connectionService.connectBluetooth();
    if (success) {
      setView('success');
    } else {
      setStatus('error');
      setErrorMessage('การเชื่อมต่อล้มเหลวหรือถูกยกเลิก');
      setTimeout(() => setStatus('idle'), 2500); 
    }
  };
  
  const getTitle = () => {
    switch(view) {
        case 'initial': return 'เลือกวิธีการเชื่อมต่อ';
        case 'connect': return 'เชื่อมต่ออุปกรณ์';
        case 'provision': return 'ตั้งค่าอุปกรณ์ใหม่';
        case 'bluetooth': return 'เชื่อมต่อผ่าน Bluetooth';
        case 'success': return 'เชื่อมต่อสำเร็จ!';
        case 'provision_success': return 'ตั้งค่าสำเร็จ!';
        default: return 'เชื่อมต่อ';
    }
  };

  const renderContent = () => {
    switch (view) {
      case 'success':
        return (
            <div className="flex flex-col items-center justify-center space-y-4 text-center h-48">
                <div className="w-16 h-16 bg-green-500/20 rounded-full flex items-center justify-center">
                    <CheckIcon className="w-10 h-10 text-green-400" />
                </div>
            </div>
        );
      case 'provision_success':
        return (
            <div className="flex flex-col items-center justify-center space-y-4 text-center h-48">
                <div className="w-16 h-16 bg-green-500/20 rounded-full flex items-center justify-center">
                    <CheckIcon className="w-10 h-10 text-green-400" />
                </div>
                <p className="text-gray-300">อุปกรณ์จะรีสตาร์ทและเชื่อมต่อ Wi-Fi ของคุณ กรุณาเชื่อมต่อมือถือกับ Wi-Fi บ้านของคุณเพื่อใช้งานต่อ</p>
            </div>
        );
      case 'connect':
        return (
          <div className="flex flex-col space-y-4">
            <p className="text-sm text-gray-400 text-center">กรอก IP Address ของ ESP32 ของคุณ</p>
            <input
              type="text"
              value={ipAddress}
              onChange={(e) => setIpAddress(e.target.value)}
              className="w-full bg-gray-900 border border-gray-600 rounded-lg px-4 py-3 text-center text-white focus:outline-none focus:ring-2 focus:ring-cyan-500 transition-all"
              placeholder="e.g., 192.168.1.100"
              disabled={status === 'connecting'}
            />
            <button
              onClick={handleWifiConnect}
              disabled={status === 'connecting'}
              className="w-full h-12 flex items-center justify-center bg-cyan-600 text-white font-bold py-2 px-4 rounded-lg hover:bg-cyan-500 disabled:bg-gray-600 transition-colors"
            >
              {status === 'connecting' ? <SpinnerIcon className="w-6 h-6"/> : 'เชื่อมต่อ'}
            </button>
          </div>
        );
      
      case 'provision':
        return (
            <div className="flex flex-col space-y-4">
                <p className="text-sm text-gray-400 text-center">เชื่อมต่อมือถือกับ Wi-Fi ของอุปกรณ์ (ชื่อ B2SRGB-XXXXXX-Setup) แล้วค้นหา Wi-Fi</p>
                
                {/* ปุ่มค้นหา WiFi */}
                <button
                    onClick={handleScanWifi}
                    disabled={isScanning || status === 'connecting'}
                    className="w-full h-12 flex items-center justify-center bg-blue-600 text-white font-bold py-2 px-4 rounded-lg hover:bg-blue-500 disabled:bg-gray-600 transition-colors"
                >
                    {isScanning ? (
                        <>
                            <SpinnerIcon className="w-5 h-5 mr-2"/>
                            <span>กำลังค้นหา WiFi...</span>
                        </>
                    ) : (
                        <>
                            <WifiIcon className="w-5 h-5 mr-2"/>
                            <span>🔍 ค้นหา WiFi</span>
                        </>
                    )}
                </button>

                {/* รายการ WiFi ที่สแกนได้ */}
                {wifiList.length > 0 && (
                    <div className="max-h-48 overflow-y-auto space-y-2 border border-gray-700 rounded-lg p-2">
                        {wifiList.map((network, index) => (
                            <button
                                key={index}
                                onClick={() => {
                                    setSsid(network.ssid);
                                    setWifiList([]);
                                }}
                                className="w-full flex items-center justify-between p-3 bg-gray-800 hover:bg-gray-700 rounded-lg transition-colors text-left"
                            >
                                <div className="flex items-center space-x-3">
                                    <WifiIcon className="w-5 h-5 text-cyan-400"/>
                                    <div>
                                        <div className="font-medium text-white">{network.ssid}</div>
                                        <div className="text-xs text-gray-400">
                                            {network.encryption} • Signal: {network.rssi} dBm
                                        </div>
                                    </div>
                                </div>
                                <div className={`w-3 h-3 rounded-full ${network.rssi > -60 ? 'bg-green-500' : network.rssi > -70 ? 'bg-yellow-500' : 'bg-red-500'}`}></div>
                            </button>
                        ))}
                    </div>
                )}

                {/* Manual Input */}
                <div className="pt-2 border-t border-gray-700">
                    <p className="text-xs text-gray-500 mb-2 text-center">หรือกรอกข้อมูล Wi-Fi เอง</p>
                    <input
                        type="text"
                        value={ssid}
                        onChange={(e) => setSsid(e.target.value)}
                        className="w-full bg-gray-900 border border-gray-600 rounded-lg px-4 py-3 text-white focus:outline-none focus:ring-2 focus:ring-cyan-500 transition-all mb-3"
                        placeholder="ชื่อ Wi-Fi (SSID)"
                        disabled={status === 'connecting'}
                    />
                    <input
                        type="password"
                        value={password}
                        onChange={(e) => setPassword(e.target.value)}
                        className="w-full bg-gray-900 border border-gray-600 rounded-lg px-4 py-3 text-white focus:outline-none focus:ring-2 focus:ring-cyan-500 transition-all"
                        placeholder="รหัสผ่าน (ถ้ามี)"
                        disabled={status === 'connecting'}
                    />
                </div>

                <button
                    onClick={handleProvision}
                    disabled={status === 'connecting' || !ssid}
                    className="w-full h-12 flex items-center justify-center bg-cyan-600 text-white font-bold py-2 px-4 rounded-lg hover:bg-cyan-500 disabled:bg-gray-600 transition-colors"
                >
                    {status === 'connecting' ? <SpinnerIcon className="w-6 h-6"/> : 'บันทึกและเชื่อมต่อ'}
                </button>
            </div>
        );

      case 'bluetooth':
        return (
          <div className="flex flex-col items-center space-y-4">
            <p className="text-sm text-gray-400 text-center px-4">
              เบราว์เซอร์จะแสดงหน้าต่าง ให้เลือกอุปกรณ์ชื่อ <strong className="text-cyan-400">'B2SRGB'</strong>
            </p>
            <button
              onClick={handleBluetoothConnect}
              disabled={status === 'connecting'}
              className="w-full h-12 flex items-center justify-center bg-blue-600 text-white font-bold py-3 px-4 rounded-lg hover:bg-blue-500 disabled:bg-gray-600 transition-colors"
            >
              {status === 'connecting' ? <SpinnerIcon className="w-6 h-6"/> : 'สแกนหาอุปกรณ์'}
            </button>
          </div>
        );

      case 'initial':
      default:
        return (
            <div className="flex flex-col space-y-6">
                <div className="space-y-3 text-center">
                    <h3 className="font-semibold text-white">สำหรับครั้งแรก หรือเปลี่ยน Wi-Fi</h3>
                    <button onClick={() => setView('provision')} className="w-full p-4 bg-cyan-700 rounded-lg text-white font-bold hover:bg-cyan-600 transition-colors">
                        ตั้งค่าอุปกรณ์ใหม่
                    </button>
                </div>
                <div className="relative flex items-center">
                    <div className="flex-grow border-t border-gray-600"></div>
                    <span className="flex-shrink mx-4 text-gray-400 text-sm">หรือ</span>
                    <div className="flex-grow border-t border-gray-600"></div>
                </div>
                <div className="space-y-3 text-center">
                     <h3 className="font-semibold text-white">สำหรับใช้งานปกติ</h3>
                    <div className="grid grid-cols-2 gap-4">
                        <button onClick={() => setView('connect')} className="group p-4 bg-gray-700/50 rounded-lg border border-gray-600 hover:border-cyan-500 hover:bg-gray-700 transition-all space-y-2 text-center focus:outline-none focus:ring-2 focus:ring-cyan-400">
                            <WifiIcon className="w-8 h-8 mx-auto text-cyan-400" />
                            <span className="text-sm text-white">Wi-Fi</span>
                        </button>
                        <button onClick={() => setView('bluetooth')} className="group p-4 bg-gray-700/50 rounded-lg border border-gray-600 hover:border-blue-500 hover:bg-gray-700 transition-all space-y-2 text-center focus:outline-none focus:ring-2 focus:ring-blue-400">
                            <BluetoothIcon className="w-8 h-8 mx-auto text-blue-400" />
                            <span className="text-sm text-white">Bluetooth</span>
                        </button>
                    </div>
                </div>
            </div>
        );
    }
  };

  return (
    <div 
        className={`fixed inset-0 bg-black/70 backdrop-blur-sm flex items-center justify-center z-50 p-4 transition-opacity duration-300
                    ${isOpen ? 'opacity-100' : 'opacity-0 pointer-events-none'}`} 
        onClick={handleClose}
    >
      <div 
        className={`w-full max-w-md bg-gray-800/80 bg-gradient-to-br from-gray-800 to-gray-900 rounded-2xl shadow-2xl p-6 border border-gray-700 transition-all duration-300 ease-in-out
                    ${isOpen ? 'scale-100 opacity-100' : 'scale-95 opacity-0'}`}
        onClick={(e) => e.stopPropagation()}
      >
        <div className="flex justify-between items-center mb-6">
          {view !== 'initial' && view !== 'success' && view !== 'provision_success' ? (
            <button onClick={() => { setView('initial'); setStatus('idle'); setErrorMessage(''); }} className="text-gray-400 hover:text-white p-2 rounded-full -ml-2">
                <BackArrowIcon className="w-6 h-6" />
            </button>
          ) : <div className="w-10 h-10"></div>}
          
          <h2 className="text-xl font-bold text-white text-center absolute left-1/2 -translate-x-1/2">
            {getTitle()}
          </h2>

          <button onClick={handleClose} className="text-3xl text-gray-500 hover:text-white leading-none p-2 rounded-full -mr-2">&times;</button>
        </div>

        {errorMessage && <p className="text-red-400 text-center text-sm mb-4 animate-pulse">{errorMessage}</p>}
        
        {renderContent()}
      </div>
    </div>
  );
};

export default ConnectionModal;