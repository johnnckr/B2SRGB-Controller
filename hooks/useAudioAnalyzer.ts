import { useState, useRef, useCallback } from 'react';

const FFT_SIZE = 256;

export const useAudioAnalyzer = (onBeat: () => void) => {
  const [isListening, setIsListening] = useState(false);
  const [audioData, setAudioData] = useState<Uint8Array>(new Uint8Array(0));
  const audioContextRef = useRef<AudioContext | null>(null);
  const analyserRef = useRef<AnalyserNode | null>(null);
  const sourceRef = useRef<MediaStreamAudioSourceNode | null>(null);
  const streamRef = useRef<MediaStream | null>(null);
  const animationFrameRef = useRef<number>(0);
  // FIX: Replaced NodeJS.Timeout with ReturnType<typeof setTimeout> for browser compatibility.
  const beatDetectionTimeoutRef = useRef<ReturnType<typeof setTimeout> | null>(null);

  const processAudio = useCallback(() => {
    if (analyserRef.current) {
      const dataArray = new Uint8Array(analyserRef.current.frequencyBinCount);
      analyserRef.current.getByteFrequencyData(dataArray);
      setAudioData(dataArray);

      // Simple beat detection: check energy in bass frequencies
      const bassEnergy = dataArray.slice(0, 10).reduce((sum, value) => sum + value, 0) / 10;
      if (bassEnergy > 160) { // Threshold for beat detection
        if (!beatDetectionTimeoutRef.current) {
          onBeat();
          beatDetectionTimeoutRef.current = setTimeout(() => {
            beatDetectionTimeoutRef.current = null;
          }, 120); // Cooldown to avoid rapid firing
        }
      }
    }
    animationFrameRef.current = requestAnimationFrame(processAudio);
  }, [onBeat]);

  const start = useCallback(async () => {
    try {
      if (navigator.mediaDevices && navigator.mediaDevices.getUserMedia) {
        const stream = await navigator.mediaDevices.getUserMedia({ audio: true });
        streamRef.current = stream;
        const context = new (window.AudioContext || (window as any).webkitAudioContext)();
        audioContextRef.current = context;
        
        const analyser = context.createAnalyser();
        analyser.fftSize = FFT_SIZE;
        analyserRef.current = analyser;
        
        const source = context.createMediaStreamSource(stream);
        source.connect(analyser);
        sourceRef.current = source;
        
        setAudioData(new Uint8Array(analyser.frequencyBinCount));
        setIsListening(true);
        animationFrameRef.current = requestAnimationFrame(processAudio);
      } else {
        alert('Your browser does not support audio input.');
      }
    } catch (err) {
      console.error('Error accessing microphone:', err);
      alert('Could not access the microphone. Please grant permission.');
    }
  }, [processAudio]);

  const stop = useCallback(() => {
    if (animationFrameRef.current) {
      cancelAnimationFrame(animationFrameRef.current);
    }
    if (streamRef.current) {
      streamRef.current.getTracks().forEach(track => track.stop());
    }
    if (sourceRef.current) {
      sourceRef.current.disconnect();
    }
    if (audioContextRef.current && audioContextRef.current.state !== 'closed') {
      audioContextRef.current.close();
    }
    if (beatDetectionTimeoutRef.current) {
        clearTimeout(beatDetectionTimeoutRef.current);
        beatDetectionTimeoutRef.current = null;
    }
    setIsListening(false);
  }, []);

  return { isListening, audioData, start, stop };
};
