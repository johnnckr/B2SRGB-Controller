

import React, { useState, useEffect, useCallback } from 'react';
import { LightMode, Pattern } from './types';
import Header from './components/Header';
import PowerButton from './components/PowerButton';
import ModeSelector from './components/ModeSelector';
import PatternSelector from './components/PatternSelector';
import ColorPalettePicker from './components/ColorPalettePicker';
import Slider from './components/Slider';
import ColorPicker from './components/ColorPicker';
import MusicMode from './components/MusicMode';
import ConnectionModal from './components/ConnectionModal';
import HelpModal from './components/HelpModal';
import { connectionService } from './services/connectionService';
import AutoBrightnessSlider from './components/AutoBrightnessSlider';
import { useAmbientLightSensor } from './hooks/useAmbientLightSensor';

const SunIcon: React.FC<{className?: string}> = ({ className }) => (
    <svg xmlns="http://www.w3.org/2000/svg" className={className} fill="none" viewBox="0 0 24 24" stroke="currentColor">
        <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M12 3v1m0 16v1m9-9h-1M4 12H3m15.364 6.364l-.707-.707M6.343 6.343l-.707-.707m12.728 0l-.707.707M6.343 17.657l-.707.707M16 12a4 4 0 11-8 0 4 4 0 018 0z" />
    </svg>
);

const FastForwardIcon: React.FC<{className?: string}> = ({ className }) => (
    <svg xmlns="http://www.w3.org/2000/svg" className={className} fill="none" viewBox="0 0 24 24" stroke="currentColor">
        <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M13 5l7 7-7 7M5 5l7 7-7 7" />
    </svg>
);

const defaultPalette = ['#ff0000', '#ff8000', '#ffff00', '#00ff00', '#0000ff'];

const patternPresets: Record<string, string[]> = {
  [Pattern.Fire]: ['#FF4500', '#FFA500', '#FFD700', '#FF6347', '#DC143C'], // Oranges, Reds, Yellows
  [Pattern.Ocean]: ['#00008B', '#00BFFF', '#40E0D0', '#E0FFFF', '#1E90FF'], // Blues, Turquoises, Whites
  [Pattern.RainbowCycle]: defaultPalette,
  [Pattern.RainbowChase]: defaultPalette,
  [Pattern.RainbowTwinkle]: defaultPalette,
};

const App: React.FC = () => {
  const [isConnected, setIsConnected] = useState(connectionService.isConnected);
  const [isModalOpen, setIsModalOpen] = useState(false);
  const [isHelpOpen, setIsHelpOpen] = useState(false);
  const [powerOn, setPowerOn] = useState(true);
  const [mode, setMode] = useState<LightMode>(LightMode.Effects);
  const [pattern, setPattern] = useState<Pattern>(Pattern.RainbowCycle);
  const [color, setColor] = useState('#ff00ff');
  const [palette, setPalette] = useState<string[]>(defaultPalette);
  const [brightness, setBrightness] = useState(80);
  const [isAutoBrightness, setIsAutoBrightness] = useState(false);
  const [speed, setSpeed] = useState(50);

  const handleConnectionChange = (status: boolean) => {
    setIsConnected(status);
    if (status) {
      setIsModalOpen(false); // Close modal on successful connection
    }
  };
  
  const handleBrightnessChange = useCallback((newBrightness: number) => {
    setBrightness(newBrightness);
    if(powerOn) connectionService.setBrightness(newBrightness);
  }, [powerOn]);

  const handleAutoBrightnessUpdate = useCallback((illuminance: number) => {
    // Map illuminance (lux) to brightness (0-100).
    // This is a simple linear mapping, clamped to a reasonable range.
    // e.g., map lux 0-500 to brightness 10-100.
    const minLux = 0, maxLux = 500;
    const minBrightness = 10, maxBrightness = 100;

    const clampedLux = Math.min(Math.max(illuminance, minLux), maxLux);
    const newBrightness = minBrightness + ((clampedLux - minLux) / (maxLux - minLux)) * (maxBrightness - minBrightness);
    
    handleBrightnessChange(Math.round(newBrightness));
  }, [handleBrightnessChange]);

  useAmbientLightSensor({ 
    enabled: isAutoBrightness, 
    onReading: handleAutoBrightnessUpdate 
  });


  const handlePowerToggle = useCallback(() => {
    if (!isConnected) return;
    const newPowerState = !powerOn;
    setPowerOn(newPowerState);
    connectionService.setPower(newPowerState);
  }, [powerOn, isConnected]);

  const handleModeChange = useCallback((newMode: LightMode) => {
    setMode(newMode);
    if(powerOn) connectionService.setMode(newMode);
  }, [powerOn]);

  const handlePatternChange = useCallback((newPattern: Pattern) => {
    setPattern(newPattern);
    if (powerOn) connectionService.setPattern(newPattern);
  }, [powerOn]);
  
  const handlePaletteChange = useCallback((index: number, newColor: string) => {
    const newPalette = [...palette];
    newPalette[index] = newColor;
    setPalette(newPalette);
    if (powerOn) connectionService.setPalette(newPalette);
  }, [palette, powerOn]);
  
  const handleResetPalette = useCallback(() => {
    setPalette(defaultPalette);
    if (powerOn) connectionService.setPalette(defaultPalette);
  }, [powerOn]);

  const handleApplyPreset = useCallback(() => {
    const presetPalette = patternPresets[pattern];
    if (presetPalette) {
      setPalette(presetPalette);
      if (powerOn) connectionService.setPalette(presetPalette);
    }
  }, [pattern, powerOn]);

  const handleColorChange = useCallback((newColor: string) => {
    setColor(newColor);
    if(powerOn) connectionService.setColor(newColor);
  }, [powerOn]);

  const handleSpeedChange = useCallback((newSpeed: number) => {
    setSpeed(newSpeed);
    if(powerOn) connectionService.setSpeed(newSpeed);
  }, [powerOn]);
  
  const renderControls = () => {
    if (!powerOn && isConnected) {
      return <div className="text-center text-gray-500 py-10">ปิดไฟอยู่</div>;
    }

    return (
      <div className="w-full flex flex-col items-center space-y-6">
        <ModeSelector currentMode={mode} onModeChange={handleModeChange} />

        <div className="w-full p-6 bg-gray-800 rounded-2xl flex flex-col items-center space-y-6">
          {mode === LightMode.Solid && <ColorPicker color={color} onColorChange={handleColorChange} />}
          
          {mode === LightMode.Effects && (
            <>
              <PatternSelector 
                currentPattern={pattern} 
                onPatternChange={handlePatternChange}
                onApplyPreset={handleApplyPreset}
                hasPreset={!!patternPresets[pattern]}
              />
              <ColorPalettePicker 
                palette={palette} 
                onColorChange={handlePaletteChange}
                onReset={handleResetPalette}
              />
            </>
          )}

          {(mode === LightMode.Fade || mode === LightMode.Strobe || mode === LightMode.Effects) && (
            <Slider label="ความเร็ว" value={speed} onChange={handleSpeedChange} icon={<FastForwardIcon className="w-5 h-5" />} />
          )}

          {mode === LightMode.Music && <MusicMode />}

          {mode !== LightMode.Music && <AutoBrightnessSlider 
              label="ความสว่าง" 
              value={brightness} 
              onChange={handleBrightnessChange} 
              isAuto={isAutoBrightness}
              onAutoToggle={setIsAutoBrightness}
              icon={<SunIcon className="w-5 h-5" />} 
          />}
        </div>
      </div>
    );
  };

  return (
    <div className="h-full w-full max-w-md mx-auto flex flex-col bg-gray-900">
      <Header isConnected={isConnected} onStatusClick={() => setIsModalOpen(true)} />
      <main className="flex-grow overflow-y-auto p-4 space-y-6">
        <div className="flex justify-center pt-2">
            <PowerButton isOn={powerOn} onToggle={handlePowerToggle} isConnected={isConnected} />
        </div>
        {renderControls()}
      </main>
      <footer className="text-center text-xs text-gray-500 p-4 border-t border-gray-800 space-y-2">
        <p>B2SRGB Controller</p>
        <div className="flex justify-center items-center space-x-4">
          <button
            onClick={() => setIsHelpOpen(true)}
            className="flex items-center space-x-1.5 hover:text-cyan-400 transition-colors"
          >
            <svg xmlns="http://www.w3.org/2000/svg" className="h-4 w-4" fill="none" viewBox="0 0 24 24" stroke="currentColor">
              <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M8.228 9c.549-1.165 2.03-2 3.772-2 2.21 0 4 1.343 4 3 0 1.4-1.278 2.575-3.006 2.907-.542.104-.994.54-.994 1.093m0 3h.01M21 12a9 9 0 11-18 0 9 9 0 0118 0z" />
            </svg>
            <span>วิธีใช้</span>
          </button>
          <a href="tel:02097959398" className="flex items-center space-x-1.5 hover:text-cyan-400 transition-colors">
            <svg xmlns="http://www.w3.org/2000/svg" className="h-4 w-4" fill="none" viewBox="0 0 24 24" stroke="currentColor">
              <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M3 5a2 2 0 012-2h3.28a1 1 0 01.948.684l1.498 4.493a1 1 0 01-.502 1.21l-2.257 1.13a11.042 11.042 0 005.516 5.516l1.13-2.257a1 1 0 011.21-.502l4.493 1.498a1 1 0 01.684.949V19a2 2 0 01-2 2h-1C9.716 21 3 14.284 3 6V5z" />
            </svg>
            <span>โทร</span>
          </a>
          <a 
            href="https://www.facebook.com/NeeKPS" 
            target="_blank" 
            rel="noopener noreferrer" 
            className="flex items-center space-x-1.5 hover:text-cyan-400 transition-colors"
          >
            <svg xmlns="http://www.w3.org/2000/svg" className="h-4 w-4" fill="currentColor" viewBox="0 0 24 24">
              <path d="M19 0h-14c-2.761 0-5 2.239-5 5v14c0 2.761 2.239 5 5 5h14c2.762 0 5-2.239 5-5v-14c0-2.761-2.238-5-5-5zm-3 7h-1.924c-.615 0-1.076.252-1.076.888v1.112h3l-.238 2h-2.762v8h-3v-8h-2v-2h2v-1.157c0-1.281.65-2.843 2.839-2.843h2.161v3z"/>
            </svg>
            <span>Facebook</span>
          </a>
        </div>
      </footer>
      <ConnectionModal 
        isOpen={isModalOpen}
        onClose={() => setIsModalOpen(false)}
        onConnectionChange={handleConnectionChange}
      />
      {isHelpOpen && <HelpModal onClose={() => setIsHelpOpen(false)} />}
    </div>
  );
};

export default App;