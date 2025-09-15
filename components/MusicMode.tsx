import React from 'react';
import { useAudioAnalyzer } from '../hooks/useAudioAnalyzer';
import { connectionService } from '../services/connectionService';

const MusicMode: React.FC = () => {
  const handleBeat = () => {
    // On each beat, send a random vibrant color to the ESP32
    const r = Math.floor(Math.random() * 256);
    const g = Math.floor(Math.random() * 256);
    const b = Math.floor(Math.random() * 256);
    connectionService.triggerMusicBeat({r, g, b}, 100);
  };
    
  const { isListening, audioData, start, stop } = useAudioAnalyzer(handleBeat);

  return (
    <div className="w-full flex flex-col items-center space-y-6">
      <div className="w-full h-24 bg-gray-800 rounded-lg flex items-end justify-center gap-1 p-2 overflow-hidden">
        {audioData && Array.from({ length: 64 }).map((_, i) => {
           const barValue = audioData[Math.floor(i * (audioData.length / 64))] || 0;
           const height = (barValue / 255) * 100;
           return (
            <div
                key={i}
                className="w-full rounded-t-sm bg-gradient-to-t from-cyan-500 to-purple-500 transition-all duration-75"
                style={{ height: `${height}%` }}
            ></div>
           );
        })}
      </div>
      <button
        onClick={isListening ? stop : start}
        className={`px-8 py-4 text-lg font-bold rounded-full transition-all duration-300 ease-in-out focus:outline-none focus:ring-4 focus:ring-offset-2 focus:ring-offset-gray-900
        ${isListening 
          ? 'bg-red-500 text-white shadow-[0_0_15px_rgba(239,68,68,0.7)]' 
          : 'bg-green-500 text-white shadow-[0_0_15px_rgba(34,197,94,0.7)]'
        }`}
      >
        {isListening ? 'หยุดฟัง' : 'เริ่มซิงค์กับเพลง'}
      </button>
      <p className="text-xs text-gray-500 text-center px-4">
        แอพจะขออนุญาตใช้ไมโครโฟนเพื่อจับจังหวะเพลง ไฟจะกระพริบตามเสียงเบส
      </p>
    </div>
  );
};

export default MusicMode;
