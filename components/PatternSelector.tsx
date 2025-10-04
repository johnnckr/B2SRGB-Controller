import React from 'react';
import { Pattern } from '../types';

interface PatternSelectorProps {
  currentPattern: Pattern;
  onPatternChange: (pattern: Pattern) => void;
  onApplyPreset: () => void;
  hasPreset: boolean;
}

const patterns = Object.values(Pattern);

const PatternSelector: React.FC<PatternSelectorProps> = ({ currentPattern, onPatternChange, onApplyPreset, hasPreset }) => {
  return (
    <div className="w-full flex flex-col items-center space-y-3">
        <div className="w-full flex justify-between items-center px-1">
            <label className="text-sm font-medium text-gray-400">เลือกเอฟเฟกต์</label>
            <button
                onClick={onApplyPreset}
                disabled={!hasPreset}
                className="px-3 py-1 text-xs font-semibold bg-cyan-600 text-white rounded-lg transition-all focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-offset-gray-800 focus:ring-cyan-400 hover:bg-cyan-500 disabled:bg-gray-600 disabled:opacity-50 disabled:cursor-not-allowed"
                aria-label="Apply preset palette for the current pattern"
            >
                ใช้สีแนะนำ
            </button>
        </div>
        <div className="w-full bg-gray-900/50 rounded-xl p-2">
            <div className="grid grid-cols-3 gap-2">
                {patterns.map((pattern) => (
                <button
                    key={pattern}
                    onClick={() => onPatternChange(pattern)}
                    className={`px-3 py-2 text-xs sm:text-sm font-semibold rounded-lg transition-all duration-200 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-offset-gray-800 focus:ring-cyan-400
                    ${currentPattern === pattern ? 'bg-cyan-500 text-white shadow-md' : 'bg-gray-700 text-gray-300 hover:bg-gray-600'}`}
                >
                    {pattern}
                </button>
                ))}
            </div>
        </div>
    </div>
  );
};

export default PatternSelector;