import type { Config } from 'tailwindcss'

export default {
  content: [
    './src/app/**/*.{js,ts,jsx,tsx,mdx}',
    './src/components/**/*.{js,ts,jsx,tsx,mdx}', // add this line even if folder empty
  ],
  theme: {
    extend: {},
  },
  plugins: [],
} satisfies Config