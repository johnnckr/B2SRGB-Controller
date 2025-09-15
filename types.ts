export enum LightMode {
  Solid = 'สีเดียว',
  Fade = 'เปลี่ยนสี',
  Strobe = 'กระพริบ',
  Effects = 'เอฟเฟกต์',
  Music = 'ตามเพลง',
}

export enum Pattern {
  // Rainbows
  RainbowCycle = 'รุ้งวนลูป',
  RainbowChase = 'รุ้งวิ่งไล่',
  // Sparkles
  RainbowTwinkle = 'รุ้งระยิบ',
  Glitter = 'กากเพชร',
  Confetti = 'พลุกระดาษ',
  // Movers
  Comet = 'ดาวตก',
  Scanner = 'สแกนเนอร์',
  TheaterChase = 'ไฟโรงหนัง',
  // Rhythmic
  Breathe = 'ลมหายใจ',
  Pulse = 'ชีพจร',
  // Themed
  Fire = 'เปลวไฟ',
  Ocean = 'คลื่นทะเล',
}

export enum ConnectionType {
  None,
  WiFi,
  Bluetooth,
}

export interface ConnectionService {
  connectionType: ConnectionType;
  isConnected: boolean;
  connectWifi: (ip: string) => Promise<boolean>;
  connectBluetooth: () => Promise<boolean>;
  disconnect: () => void;
  setPower: (on: boolean) => void;
  setMode: (mode: LightMode) => void;
  setPattern: (pattern: Pattern) => void;
  setColor: (hexColor: string) => void;
  setPalette: (hexColors: string[]) => void;
  setBrightness: (brightness: number) => void;
  setSpeed: (speed: number) => void;
  triggerMusicBeat: (color: {r: number, g: number, b: number}, brightness: number) => void;
}