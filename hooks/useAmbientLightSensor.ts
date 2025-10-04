import { useEffect, useRef, useCallback } from 'react';

interface UseAmbientLightSensorProps {
  enabled: boolean;
  onReading: (illuminance: number) => void;
  onError?: (error: Error) => void;
}

// Type declaration for AmbientLightSensor, as it might not be in default TS lib
declare global {
  interface Window {
    AmbientLightSensor?: any;
  }
}

export const useAmbientLightSensor = ({ enabled, onReading, onError }: UseAmbientLightSensorProps) => {
  const sensorRef = useRef<any | null>(null);
  const simulationIntervalRef = useRef<ReturnType<typeof setInterval> | null>(null);

  const cleanup = useCallback(() => {
    if (sensorRef.current) {
      sensorRef.current.stop();
      sensorRef.current.onreading = null;
      sensorRef.current.onerror = null;
      sensorRef.current = null;
    }
    if (simulationIntervalRef.current) {
      clearInterval(simulationIntervalRef.current);
      simulationIntervalRef.current = null;
    }
  }, []);
  
  const startSimulation = useCallback(() => {
    simulationIntervalRef.current = setInterval(() => {
      // Simulate a fluctuating light level (e.g., between 50 and 300 lux)
      const base = 150;
      const fluctuation = 120 * Math.sin(Date.now() / 2000); // Slow sine wave
      onReading(base + fluctuation);
    }, 1000);
  }, [onReading]);

  useEffect(() => {
    if (!enabled) {
      cleanup();
      return;
    }

    if ('AmbientLightSensor' in window) {
      // The browser supports the Ambient Light Sensor API
      const initSensor = async () => {
        try {
          // The 'ambient-light-sensor' permission is not a standard one for manifest
          // but is requested this way by the API.
          const permissionStatus = await navigator.permissions.query({ name: 'ambient-light-sensor' as any });

          if (permissionStatus.state === 'granted' || permissionStatus.state === 'prompt') {
            const sensor = new window.AmbientLightSensor({ frequency: 1 }); // Read once per second

            sensor.onreading = () => {
              if (sensor.illuminance !== null) {
                onReading(sensor.illuminance);
              }
            };

            sensor.onerror = (event: any) => {
              console.error('AmbientLightSensor error:', event.error);
              if (onError) onError(event.error);
              // Fallback to simulation if the sensor fails
              cleanup();
              startSimulation();
            };
            
            sensor.start();
            sensorRef.current = sensor;
          } else {
            // Permission denied, fallback to simulation
            console.warn('Ambient light sensor permission denied. Starting simulation.');
            startSimulation();
          }
        } catch (error) {
          console.error('Failed to initialize ambient light sensor:', error);
          if (onError && error instanceof Error) onError(error);
          startSimulation();
        }
      };

      initSensor();

    } else {
      // The browser does NOT support the API, run simulation.
      console.warn('AmbientLightSensor API not supported. Starting simulation.');
      startSimulation();
    }

    return cleanup;
  }, [enabled, onReading, onError, cleanup, startSimulation]);
};