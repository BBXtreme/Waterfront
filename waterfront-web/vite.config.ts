// vitest.config.ts  (in project ROOT)
import { defineConfig } from 'vitest/config'
import react from '@vitejs/plugin-react'
import tsconfigPaths from 'vite-tsconfig-paths'  // this is the key fix

export default defineConfig({
  plugins: [
    react(),
    tsconfigPaths(),  // resolves tsconfig paths + helps vitest/config loading
  ],
  test: {
    globals: true,
    environment: 'jsdom',
    setupFiles: './setupTests.ts',  // must exist in root
    css: true,                      // optional but good for Tailwind
    exclude: ['node_modules', 'dist', '.next'],
  },
})