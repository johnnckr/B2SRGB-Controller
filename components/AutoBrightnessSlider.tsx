import React from 'react';

interface AutoBrightnessSliderProps {
  label: string;
  value: number;
  onChange: (value: number) => void;
  min?: number;
  max?: number;
  icon: React.ReactNode;
  isAuto: boolean;
  onAutoToggle: (enabled: boolean) => void;
}

const AutoBrightnessSlider: React.FC<AutoBrightnessSliderProps> = ({ 
    label, 
    value, 
    onChange, 
    min = 0, 
    max = 100, 
    icon, 
    isAuto, 
    onAutoToggle 
}) => {
  const percentage = ((value - min) / (max - min)) * 100;
  
  return (
    <div className="w-full space-y-2">
      <div className="flex justify-between items-center text-sm text-gray-400">
        <label className="flex items-center space-x-2">
          {icon}
          <span>{label}</span>
        </label>
        <div className="flex items-center space-x-3">
            <span className="text-xs">Auto</span>
            <button
                role="switch"
                aria-checked={isAuto}
                onClick={() => onAutoToggle(!isAuto)}
                className={`relative inline-flex items-center h-6 w-11 flex-shrink-0 cursor-pointer rounded-full border-2 border-transparent transition-colors duration-200 ease-in-out focus:outline-none focus:ring-2 focus:ring-cyan-500 focus:ring-offset-2 focus:ring-offset-gray-800 ${
                isAuto ? 'bg-cyan-600' : 'bg-gray-600'
                }`}
            >
                <span
                aria-hidden="true"
                className={`inline-block h-5 w-5 transform rounded-full bg-white shadow ring-0 transition duration-200 ease-in-out ${
                    isAuto ? 'translate-x-5' : 'translate-x-0'
                }`}
                />
            </button>
        </div>
      </div>
      <div className={`relative flex items-center transition-opacity ${isAuto ? 'opacity-50' : 'opacity-100'}`}>
        <input
          type="range"
          min={min}
          max={max}
          value={value}
          onChange={(e) => onChange(parseInt(e.target.value, 10))}
          className="w-full h-2 bg-transparent appearance-none cursor-pointer group"
          disabled={isAuto}
        />
        <div className="absolute w-full h-2 bg-gray-700 rounded-full pointer-events-none -z-10">
           <div 
            className="h-2 rounded-full bg-gradient-to-r from-cyan-500 to-purple-500" 
            style={{ width: `${percentage}%` }}
          ></div>
        </div>
        <div className="absolute right-0 top-1/2 -translate-y-1/2 -mr-4 font-mono bg-gray-700 px-2 py-1 rounded-md text-white text-sm">
            {value}
        </div>
      </div>
    </div>
  );
};

export default AutoBrightnessSlider;