
import React from 'react';

interface PowerButtonProps {
  isOn: boolean;
  onToggle: () => void;
  isConnected: boolean;
}

const PowerIcon: React.FC<{className?: string}> = ({ className }) => (
    <svg xmlns="http://www.w3.org/2000/svg" className={className} fill="none" viewBox="0 0 24 24" stroke="currentColor">
        <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M5.636 5.636a9 9 0 1012.728 0M12 3v9" />
    </svg>
);

const PowerButton: React.FC<PowerButtonProps> = ({ isOn, onToggle, isConnected }) => {
  return (
    <button
      onClick={onToggle}
      disabled={!isConnected}
      className={`relative w-24 h-24 rounded-full flex items-center justify-center transition-all duration-300 ease-in-out focus:outline-none focus:ring-4 focus:ring-offset-2 focus:ring-offset-gray-900
      ${isOn ? 'bg-cyan-500 text-white shadow-[0_0_15px_rgba(56,189,248,0.8)]' : 'bg-gray-700 text-gray-400 hover:bg-gray-600'}
      disabled:bg-gray-800 disabled:text-gray-600 disabled:cursor-not-allowed disabled:shadow-none`}
      aria-label={isConnected ? "Toggle power" : "Power disabled, please connect first"}
    >
      <PowerIcon className="w-10 h-10" />
    </button>
  );
};

export default PowerButton;