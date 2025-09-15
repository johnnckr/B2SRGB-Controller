
import React from 'react';

interface SliderProps {
  label: string;
  value: number;
  onChange: (value: number) => void;
  min?: number;
  max?: number;
  icon: React.ReactNode;
}

const Slider: React.FC<SliderProps> = ({ label, value, onChange, min = 0, max = 100, icon }) => {
  const percentage = ((value - min) / (max - min)) * 100;
  
  return (
    <div className="w-full space-y-2">
      <div className="flex justify-between items-center text-sm text-gray-400">
        <label className="flex items-center space-x-2">
          {icon}
          <span>{label}</span>
        </label>
        <span className="font-mono bg-gray-700 px-2 py-1 rounded-md text-white">{value}</span>
      </div>
      <div className="relative flex items-center">
        <input
          type="range"
          min={min}
          max={max}
          value={value}
          onChange={(e) => onChange(parseInt(e.target.value, 10))}
          className="w-full h-2 bg-transparent appearance-none cursor-pointer group"
          style={{ '--thumb-color': '#22d3ee' } as React.CSSProperties} // cyan-400
        />
        <div className="absolute w-full h-2 bg-gray-700 rounded-full pointer-events-none -z-10">
           <div 
            className="h-2 rounded-full bg-gradient-to-r from-cyan-500 to-purple-500" 
            style={{ width: `${percentage}%` }}
          ></div>
        </div>
      </div>
    </div>
  );
};

export default Slider;
