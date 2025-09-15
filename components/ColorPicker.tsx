import React, { useRef, useCallback, useEffect, useState } from 'react';
import Slider from './Slider';

// This sub-component is a 2D picker for Hue and Saturation.
const HueSaturationWheel: React.FC<{
    size: number;
    h: number;
    s: number;
    l: number;
    onHsChange: (h: number, s: number) => void;
}> = ({ size, h, s, l, onHsChange }) => {
    const wheelRef = useRef<HTMLDivElement>(null);
    const [isDragging, setIsDragging] = useState(false);

    const handleInteraction = useCallback((e: MouseEvent | TouchEvent) => {
        if (!wheelRef.current) return;
        const rect = wheelRef.current.getBoundingClientRect();
        const centerX = rect.left + rect.width / 2;
        const centerY = rect.top + rect.height / 2;

        let clientX, clientY;
        if (window.TouchEvent && e instanceof TouchEvent) {
            clientX = e.touches[0].clientX;
            clientY = e.touches[0].clientY;
        } else {
            clientX = (e as MouseEvent).clientX;
            clientY = (e as MouseEvent).clientY;
        }
        
        const dx = clientX - centerX;
        const dy = clientY - centerY;
        const radius = size / 2;
        
        // Calculate angle for hue
        let angleDeg = Math.atan2(dy, dx) * (180 / Math.PI);
        if (angleDeg < 0) angleDeg += 360;
        
        // Calculate distance for saturation and clamp it to the circle
        const distance = Math.min(Math.sqrt(dx * dx + dy * dy), radius);
        const saturation = (distance / radius) * 100;
        
        onHsChange(angleDeg, saturation);
    }, [size, onHsChange]);

    const stopDragging = useCallback(() => setIsDragging(false), []);

    const startDragging = (e: React.MouseEvent | React.TouchEvent) => {
        e.preventDefault();
        setIsDragging(true);
        handleInteraction(e.nativeEvent);
    };

    useEffect(() => {
        const handleMove = (e: MouseEvent | TouchEvent) => {
            if (isDragging) {
                e.preventDefault();
                handleInteraction(e);
            }
        };
        if (isDragging) {
            window.addEventListener('mousemove', handleMove);
            window.addEventListener('touchmove', handleMove, { passive: false });
            window.addEventListener('mouseup', stopDragging);
            window.addEventListener('touchend', stopDragging);
        }
        return () => {
            window.removeEventListener('mousemove', handleMove);
            window.removeEventListener('touchmove', handleMove);
            window.removeEventListener('mouseup', stopDragging);
            window.removeEventListener('touchend', stopDragging);
        };
    }, [isDragging, handleInteraction, stopDragging]);

    const thumbSize = size * 0.15;
    const thumbDistance = (s / 100) * (size / 2);
    const thumbAngleRad = h * Math.PI / 180;
    
    // Position thumb relative to the center of the wheel
    const thumbX = Math.cos(thumbAngleRad) * thumbDistance;
    const thumbY = Math.sin(thumbAngleRad) * thumbDistance;

    return (
        <div
            ref={wheelRef}
            className="relative rounded-full cursor-pointer select-none shadow-xl"
            style={{ 
                width: size, 
                height: size,
                background: `conic-gradient(from 90deg, hsl(360, 100%, 50%), hsl(315, 100%, 50%), hsl(270, 100%, 50%), hsl(225, 100%, 50%), hsl(180, 100%, 50%), hsl(135, 100%, 50%), hsl(90, 100%, 50%), hsl(45, 100%, 50%), hsl(0, 100%, 50%))`,
                touchAction: 'none'
            }}
            onMouseDown={startDragging}
            onTouchStart={startDragging}
        >
            {/* Saturation overlay (white to transparent from center) */}
            <div className="absolute inset-0 rounded-full" style={{background: 'radial-gradient(circle, white, rgba(255,255,255,0))'}}></div>
            {/* Thumb */}
            <div
                className="absolute rounded-full border-4 border-white shadow-lg pointer-events-none"
                style={{
                    width: thumbSize,
                    height: thumbSize,
                    // Position the thumb's center at the wheel's center, then transform
                    top: `calc(50% - ${thumbSize / 2}px)`,
                    left: `calc(50% - ${thumbSize / 2}px)`,
                    transform: `translate(${thumbX}px, ${thumbY}px)`,
                    backgroundColor: `hsl(${h}, ${s}%, ${l}%)`
                }}
            ></div>
        </div>
    );
};

// Helper functions for color conversion
function hexToHsl(hex: string): { h: number, s: number, l: number } {
    if (!hex || hex.length < 4) hex = '#000000';
    let r = 0, g = 0, b = 0;
    if (hex.length === 4) {
        r = parseInt(hex[1] + hex[1], 16);
        g = parseInt(hex[2] + hex[2], 16);
        b = parseInt(hex[3] + hex[3], 16);
    } else if (hex.length === 7) {
        r = parseInt(hex.substring(1, 3), 16);
        g = parseInt(hex.substring(3, 5), 16);
        b = parseInt(hex.substring(5, 7), 16);
    }
    r /= 255; g /= 255; b /= 255;
    const max = Math.max(r, g, b), min = Math.min(r, g, b);
    let h = 0, s = 0, l = (max + min) / 2;

    if (max !== min) {
        const d = max - min;
        s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
        switch (max) {
            case r: h = (g - b) / d + (g < b ? 6 : 0); break;
            case g: h = (b - r) / d + 2; break;
            case b: h = (r - g) / d + 4; break;
        }
        h /= 6;
    }
    return { h: h * 360, s: s * 100, l: l * 100 };
}

function hslToHex(h: number, s: number, l: number): string {
    h /= 360; s /= 100; l /= 100;
    let r, g, b;
    if (s === 0) {
        r = g = b = l;
    } else {
        const hue2rgb = (p: number, q: number, t: number) => {
            if (t < 0) t += 1;
            if (t > 1) t -= 1;
            if (t < 1/6) return p + (q - p) * 6 * t;
            if (t < 1/2) return q;
            if (t < 2/3) return p + (q - p) * (2/3 - t) * 6;
            return p;
        }
        const q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        const p = 2 * l - q;
        r = hue2rgb(p, q, h + 1/3);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1/3);
    }
    const toHex = (x: number) => {
        const hex = Math.round(x * 255).toString(16);
        return hex.length === 1 ? '0' + hex : hex;
    };
    return `#${toHex(r)}${toHex(g)}${toHex(b)}`;
}

const BrightnessIcon: React.FC<{className?: string}> = ({ className }) => (
    <svg xmlns="http://www.w3.org/2000/svg" className={className} fill="none" viewBox="0 0 24 24" stroke="currentColor">
        <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M12 3v1m0 16v1m9-9h-1M4 12H3m15.364 6.364l-.707-.707M6.343 6.343l-.707-.707m12.728 0l-.707.707M6.343 17.657l-.707.707M16 12a4 4 0 11-8 0 4 4 0 018 0z" />
    </svg>
);

interface ColorPickerProps {
  color: string;
  onColorChange: (color: string) => void;
}

const ColorPicker: React.FC<ColorPickerProps> = ({ color, onColorChange }) => {
  const { h, s, l } = hexToHsl(color);

  const handleHsChange = (newHue: number, newSaturation: number) => {
    const newHex = hslToHex(newHue, newSaturation, l);
    onColorChange(newHex);
  };
  
  const handleLightnessChange = (newLightness: number) => {
    const newHex = hslToHex(h, s, newLightness);
    onColorChange(newHex);
  };

  return (
    <div className="w-full flex flex-col items-center space-y-6">
        <HueSaturationWheel 
            size={180} 
            h={h} 
            s={s} 
            l={l} 
            onHsChange={handleHsChange} 
        />
        <div className="w-full max-w-xs px-4">
            <Slider 
                label="ความสว่าง"
                value={Math.round(l)}
                onChange={handleLightnessChange}
                icon={<BrightnessIcon className="w-5 h-5"/>}
            />
        </div>
    </div>
  );
};

export default ColorPicker;
