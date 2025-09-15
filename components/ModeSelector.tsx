
import React from 'react';
import { LightMode } from '../types';

interface ModeSelectorProps {
  currentMode: LightMode;
  onModeChange: (mode: LightMode) => void;
}

const modes = Object.values(LightMode);

const ModeSelector: React.FC<ModeSelectorProps> = ({ currentMode, onModeChange }) => {
  return (
    <div className="w-full bg-gray-800 rounded-xl p-2">
      <div className="grid grid-cols-3 sm:grid-cols-5 gap-2">
        {modes.map((mode) => (
          <button
            key={mode}
            onClick={() => onModeChange(mode)}
            className={`px-3 py-3 text-sm font-semibold rounded-lg transition-all duration-200 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-offset-gray-800 focus:ring-cyan-400
            ${currentMode === mode ? 'bg-cyan-500 text-white shadow-md' : 'bg-gray-700 text-gray-300 hover:bg-gray-600'}`}
          >
            {mode}
          </button>
        ))}
      </div>
    </div>
  );
};

export default ModeSelector;
