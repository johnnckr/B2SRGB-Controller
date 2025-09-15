import React, { useState } from 'react';
import ColorPicker from './ColorPicker';

interface ColorPalettePickerProps {
  palette: string[];
  onColorChange: (index: number, color: string) => void;
  onReset: () => void;
}

const ColorPalettePicker: React.FC<ColorPalettePickerProps> = ({ palette, onColorChange, onReset }) => {
  const [activeIndex, setActiveIndex] = useState(0);

  return (
    <div className="w-full flex flex-col items-center space-y-6">
      <div>
        <label className="text-sm font-medium text-gray-400 mb-3 block text-center">ชุดสี (Palette)</label>
        <div className="flex justify-center items-center gap-2 sm:gap-4">
          {palette.map((color, index) => (
            <button
              key={index}
              onClick={() => setActiveIndex(index)}
              className={`relative w-10 h-10 sm:w-12 sm:h-12 rounded-full transition-all duration-200 focus:outline-none
                         ${activeIndex === index ? 'ring-4 ring-cyan-400 ring-offset-2 ring-offset-gray-800' : 'ring-2 ring-gray-600'}`}
              style={{ backgroundColor: color }}
              aria-label={`Select color ${index + 1}`}
            />
          ))}
        </div>
      </div>
      
      <div className="w-full pt-4 border-t border-gray-700/50">
        <ColorPicker 
          color={palette[activeIndex]}
          onColorChange={(newColor) => onColorChange(activeIndex, newColor)}
        />
      </div>

      <button
        onClick={onReset}
        className="px-4 py-2 text-xs font-semibold bg-gray-700 text-gray-300 hover:bg-gray-600 rounded-lg transition-colors focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-offset-gray-800 focus:ring-cyan-400"
      >
        รีเซ็ตเป็นสีรุ้ง
      </button>
    </div>
  );
};

export default ColorPalettePicker;
