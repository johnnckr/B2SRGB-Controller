const CACHE_NAME = 'b2srgb-controller-cache-v1';
// These are the core files for the app shell.
// Other resources will be cached dynamically upon first request.
const assetsToCache = [
  '/',
  '/index.html',
  '/manifest.json',
  '/icon.svg',
];

// Install event: open cache and add core assets.
self.addEventListener('install', (event) => {
  event.waitUntil(
    caches.open(CACHE_NAME)
      .then((cache) => {
        console.log('Service Worker: Caching app shell');
        return cache.addAll(assetsToCache);
      })
      .catch(error => {
        console.error('Failed to cache app shell:', error);
      })
  );
});

// Activate event: clean up old caches.
self.addEventListener('activate', (event) => {
  const cacheWhitelist = [CACHE_NAME];
  event.waitUntil(
    caches.keys().then((cacheNames) => {
      return Promise.all(
        cacheNames.map((cacheName) => {
          if (cacheWhitelist.indexOf(cacheName) === -1) {
            console.log('Service Worker: Deleting old cache', cacheName);
            return caches.delete(cacheName);
          }
        })
      );
    }).then(() => self.clients.claim()) // Take control of open clients immediately
  );
});

// Fetch event: serve from cache first, fall back to network, and cache new resources.
self.addEventListener('fetch', (event) => {
  // Skip caching for external requests (ESP32, Bluetooth, etc.)
  const url = new URL(event.request.url);
  if (url.hostname !== self.location.hostname) {
    return; // ไม่ cache request ไป ESP32
  }

  // Only handle GET requests
  if (event.request.method !== 'GET') {
    return;
  }

  event.respondWith(
    caches.open(CACHE_NAME).then(async (cache) => {
      // Try to find the response in the cache.
      const cachedResponse = await cache.match(event.request);
      if (cachedResponse) {
        // If found, return it.
        return cachedResponse;
      }

      // If not in cache, fetch from the network.
      try {
        const networkResponse = await fetch(event.request);
        // Check for a valid response to cache.
        // We only cache successful GET requests. We don't cache non-http/https protocols.
        if (networkResponse.ok && event.request.url.startsWith('http')) {
          await cache.put(event.request, networkResponse.clone());
        }
        return networkResponse;
      } catch (error) {
        console.error('Service Worker: Network request failed.', error);
        // This is where you could return a fallback page if you had one.
        throw error;
      }
    })
  );
});
