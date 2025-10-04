/**
 * Connection Service
 * 
 * This service centralizes all communication with the ESP32 device.
 * It abstracts the underlying connection method (WiFi or Bluetooth)
 * from the rest of the application.
 */
import { LightMode, Pattern, ConnectionType, ConnectionService as IConnectionService } from '../types';

// UUIDs for the Bluetooth service and characteristic.
// These MUST match the values defined in your ESP32's Bluetooth code.
const BLE_SERVICE_UUID = '4fafc201-1fb5-459e-8fcc-c5c9c331914b';
const BLE_CHARACTERISTIC_UUID = 'beb5483e-36e1-4688-b7f5-ea07361b26a8';

// Default IP for the ESP32 when in Access Point (AP) mode for provisioning.
const PROVISIONING_IP = '192.168.4.1';

class ConnectionServiceManager implements IConnectionService {
    public connectionType: ConnectionType = ConnectionType.None;
    public isConnected: boolean = false;
    // FIX: Use 'any' for BluetoothRemoteGATTCharacteristic since the type is not available in default TS lib.
    private bleCharacteristic: any | null = null;
    private wifiIp: string | null = null;

    private setConnectionStatus(type: ConnectionType, status: boolean) {
        this.connectionType = type;
        this.isConnected = status;
        // Optional: Dispatch a global event for connection status changes
        // window.dispatchEvent(new CustomEvent('connectionchange', { detail: { isConnected: status } }));
    }

    async provisionWifi(ssid: string, pass: string): Promise<boolean> {
        console.log(`[Provisioning] Sending credentials for SSID: ${ssid} to ${PROVISIONING_IP}`);
        try {
            const response = await fetch(`http://${PROVISIONING_IP}/save-wifi`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ ssid, pass }),
            });
            if (response.ok) {
                console.log('[Provisioning] Credentials sent successfully.');
                return true;
            }
            console.error('[Provisioning] Failed to send credentials, server responded with:', response.status);
            return false;
        } catch (error) {
            console.error('[Provisioning] Error sending credentials:', error);
            return false;
        }
    }

    async connectWifi(ip: string): Promise<boolean> {
        console.log(`[WiFi] Attempting to connect to ${ip}...`);
        try {
            // ทดสอบการเชื่อมต่อโดยส่ง GET request ไปที่ ESP32
            const response = await fetch(`http://${ip}/`, {
                method: 'GET',
                mode: 'no-cors', // ป้องกัน CORS error
                cache: 'no-cache',
            });

            this.wifiIp = ip;
            this.setConnectionStatus(ConnectionType.WiFi, true);
            console.log('[WiFi] Connection successful to ' + ip);
            return true;

        } catch (error) {
            console.error('[WiFi] Connection failed:', error);
            alert(`Cannot connect to ESP32 at ${ip}\n\nPlease check:\n1. ESP32 is connected to WiFi\n2. Computer and ESP32 are on the same network\n3. IP address is correct`);
            return false;
        }
    }

    async connectBluetooth(): Promise<boolean> {
        console.log('[BT] Starting Bluetooth scan...');
        
        // ตรวจสอบว่าเบราว์เซอร์รองรับ Web Bluetooth API หรือไม่
        if (!('bluetooth' in navigator)) {
            const errorMsg = 'Web Bluetooth API is not available on this browser.\n\n' +
                           'Please use:\n' +
                           '✅ Chrome (Desktop/Android)\n' +
                           '✅ Edge (Desktop)\n' +
                           '✅ Opera (Desktop/Android)\n\n' +
                           'Or use HTTP connection instead.';
            alert(errorMsg);
            console.error('[BT] Web Bluetooth API not supported');
            return false;
        }

        try {
            // FIX: Property 'bluetooth' does not exist on type 'Navigator'. Cast to any to bypass.
            const device = await (navigator as any).bluetooth.requestDevice({
                filters: [{ services: [BLE_SERVICE_UUID] }],
                optionalServices: [BLE_SERVICE_UUID]
            });

            if (!device || !device.gatt) {
                console.error('[BT] Device or GATT not available');
                return false;
            }
            
            console.log('[BT] Device found, connecting to GATT server...');
            const server = await device.gatt.connect();

            console.log('[BT] Getting service...');
            const service = await server.getPrimaryService(BLE_SERVICE_UUID);

            console.log('[BT] Getting characteristic...');
            this.bleCharacteristic = await service.getCharacteristic(BLE_CHARACTERISTIC_UUID);

            this.setConnectionStatus(ConnectionType.Bluetooth, true);
            console.log('[BT] Bluetooth connection successful.');
            return true;

        } catch (error) {
            console.error('[BT] Connection failed:', error);
            if ((error as Error).message.includes('User cancelled')) {
                console.log('[BT] User cancelled the pairing request');
            }
            this.disconnect();
            return false;
        }
    }

    disconnect() {
        // In a real app, you would properly close connections here.
        this.bleCharacteristic = null;
        this.wifiIp = null;
        this.setConnectionStatus(ConnectionType.None, false);
        console.log('[Connection] Disconnected.');
    }

    private async sendCommand(command: object) {
        if (!this.isConnected) {
            console.warn('Attempted to send command while disconnected:', command);
            return;
        }

        const commandString = JSON.stringify(command);

        switch (this.connectionType) {
            case ConnectionType.WiFi:
                try {
                    console.log(`[WiFi SEND to ${this.wifiIp}]`, commandString);
                    const response = await fetch(`http://${this.wifiIp}/command`, {
                        method: 'POST',
                        headers: {
                            'Content-Type': 'application/json',
                        },
                        body: commandString,
                        mode: 'no-cors', // ป้องกัน CORS error
                    });
                    console.log('[WiFi] Command sent successfully');
                } catch (error) {
                    console.error('[WiFi] Failed to send command:', error);
                }
                break;
            
            case ConnectionType.Bluetooth:
                if (this.bleCharacteristic) {
                    try {
                        console.log('[BT SEND]', commandString);
                        const encoder = new TextEncoder();
                        await this.bleCharacteristic.writeValue(encoder.encode(commandString));
                        console.log('[BT] Command sent successfully');
                    } catch (error) {
                        console.error('[BT] Failed to send command:', error);
                    }
                }
                break;
            
            default:
                console.error('Unknown connection type.');
                break;
        }
    }

    // --- Public API for sending commands ---

    setPower = (on: boolean) => this.sendCommand({ type: 'POWER', value: on });
    setMode = (mode: LightMode) => this.sendCommand({ type: 'MODE', value: mode });
    setPattern = (pattern: Pattern) => this.sendCommand({ type: 'PATTERN', value: pattern });

    setColor = (hexColor: string) => {
        const r = parseInt(hexColor.slice(1, 3), 16);
        const g = parseInt(hexColor.slice(3, 5), 16);
        const b = parseInt(hexColor.slice(5, 7), 16);
        this.sendCommand({ type: 'COLOR', value: { r, g, b } });
    }

    setPalette = (hexColors: string[]) => {
        const rgbColors = hexColors.map(hex => {
            const r = parseInt(hex.slice(1, 3), 16);
            const g = parseInt(hex.slice(3, 5), 16);
            const b = parseInt(hex.slice(5, 7), 16);
            return { r, g, b };
        });
        this.sendCommand({ type: 'PALETTE', value: rgbColors });
    }

    setBrightness = (brightness: number) => {
        const value = Math.round((brightness / 100) * 255);
        this.sendCommand({ type: 'BRIGHTNESS', value });
    }

    setSpeed = (speed: number) => {
        const value = 100 - speed;
        this.sendCommand({ type: 'SPEED', value });
    }

    triggerMusicBeat = (color: {r: number, g: number, b: number}, brightness: number) => {
        const scaledBrightness = Math.round((brightness / 100) * 255);
        this.sendCommand({ type: 'MUSIC_BEAT', color, brightness: scaledBrightness });
    }
}

// Export a singleton instance of the service
export const connectionService = new ConnectionServiceManager();