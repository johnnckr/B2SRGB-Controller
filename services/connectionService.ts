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
        // MOCK: In a real app, you'd establish a WebSocket or check an HTTP endpoint.
        // For this mock, we'll just simulate a successful connection after a delay.
        return new Promise(resolve => {
            setTimeout(() => {
                this.wifiIp = ip;
                this.setConnectionStatus(ConnectionType.WiFi, true);
                console.log('[WiFi] Mock connection successful.');
                resolve(true);
            }, 1000);
        });
    }

    async connectBluetooth(): Promise<boolean> {
        console.log('[BT] Starting Bluetooth scan...');
        try {
            // FIX: Property 'bluetooth' does not exist on type 'Navigator'. Cast to any to bypass.
            if (!(navigator as any).bluetooth) {
                alert('Web Bluetooth API is not available on this browser.');
                return false;
            }
            // FIX: Property 'bluetooth' does not exist on type 'Navigator'. Cast to any to bypass.
            const device = await (navigator as any).bluetooth.requestDevice({
                filters: [{ services: [BLE_SERVICE_UUID] }],
                optionalServices: [BLE_SERVICE_UUID]
            });

            if (!device || !device.gatt) return false;
            
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

    private sendCommand(command: object) {
        if (!this.isConnected) {
            console.warn('Attempted to send command while disconnected:', command);
            return;
        }

        const commandString = JSON.stringify(command);

        switch (this.connectionType) {
            case ConnectionType.WiFi:
                // MOCK: Send via WebSocket or HTTP POST
                console.log(`[WiFi SEND to ${this.wifiIp}]`, commandString);
                // fetch(`http://${this.wifiIp}/command`, { method: 'POST', body: commandString });
                break;
            
            case ConnectionType.Bluetooth:
                if (this.bleCharacteristic) {
                    console.log('[BT SEND]', commandString);
                    const encoder = new TextEncoder();
                    this.bleCharacteristic.writeValue(encoder.encode(commandString));
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