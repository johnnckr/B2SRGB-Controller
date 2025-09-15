import React from 'react';

interface HeaderProps {
  isConnected: boolean;
  onStatusClick: () => void;
}

const Header: React.FC<HeaderProps> = ({ isConnected, onStatusClick }) => {
  return (
    <header className="flex items-center justify-between p-4 bg-gray-800/50 backdrop-blur-sm rounded-b-2xl shadow-lg sticky top-0 z-10">
      <h1 className="text-2xl font-bold bg-clip-text text-transparent bg-gradient-to-r from-blue-400 via-cyan-300 to-orange-400">
        B2SRGB
      </h1>
      <button 
        onClick={onStatusClick}
        className="flex items-center space-x-2 p-2 rounded-lg transition-colors hover:bg-gray-700/50 focus:outline-none focus:ring-2 focus:ring-cyan-400"
        aria-label="Connection status"
      >
        <span className={`text-sm ${isConnected ? 'text-green-400' : 'text-red-400'}`}>
          {isConnected ? 'เชื่อมต่อแล้ว' : 'ไม่ได้เชื่อมต่อ'}
        </span>
        <div
          className={`w-3 h-3 rounded-full transition-colors ${isConnected ? 'bg-green-500 animate-pulse' : 'bg-red-500'}`}
        ></div>
      </button>
    </header>
  );
};

export default Header;