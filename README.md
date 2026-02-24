# Waterfront  
**Kayak / SUP Rental Booking App + ESP32 MQTT Controller**

Self-service, unmanned rental system with Next.js PWA, Supabase backend, BTCPay/Stripe payments, and ESP32-based smart locker control via MQTT.

Live demo (Vercel): https://waterfront-[your-project-slug].vercel.app  

[![Deployed with Vercel](https://therealsujitk-vercel-badge.vercel.app/api/BBXtreme/Waterfront?branch=main)](https://vercel.com/bbxtreme/waterfront-[your-project-slug])  

<!-- Replace with your real Vercel project slug / badge link after connecting the repo -->

**Current status**: Early development – auth & local setup working, calendar & payments next

## Features (Current & Planned)

- Guest booking (no login required for basic flow)
- Calendar + time slot selection
- Location / equipment selection
- Real-time availability (Supabase Realtime)
- Payments: Stripe (fiat) + BTCPay Server (Lightning / Liquid BTC)
- Instant PIN + QR code confirmation (via email or on-screen)
- ESP32 MQTT integration for locker unlock / sensor feedback (take/return events)
- Admin dashboard (telemetry, bookings, payments, logs)
- Offline-tolerant PWA (cached QR/PIN for poor connectivity)

## Tech Stack

- **Frontend / PWA** — Next.js 16+, TypeScript, Tailwind CSS, shadcn/ui  
- **Backend / Database** — Supabase (PostgreSQL + Auth + Realtime + Storage)  
- **Payments** — Stripe + BTCPay Server (self-hosted)  
- **IoT / Hardware** — ESP32 (Arduino framework + PlatformIO), PubSubClient (MQTT), TinyGSM (LTE fallback)  
- **Infrastructure** — Docker + Mosquitto MQTT broker  
- **Development** — pnpm, Aider (AI coding), GitHub  
- **Deployment** — Vercel (frontend only)

## Project Structure (Monorepo)

Waterfront/ 

├── docs/                 # Specifications, screenshots, wireframes, FSD/TSD 

├── supabase-local/       # Local Supabase dev environment (CLI) 

├── waterfront-web/       # Next.js PWA → deployed to Vercel 

├── waterfront-infra/     # Docker Compose (MQTT broker, future BTCPay, etc.) 

├── waterfront-esp32/     # PlatformIO project (ESP32 firmware) 

├── waterfront-backend/   # Optional Node.js/Edge Functions (if needed) 

└── README.md

**Note**: Only `waterfront-web` is deployed to Vercel. The rest (ESP32, infra, local Supabase) remain local/dev tools.

## Quick Start – Development Setup

### Prerequisites

- Node.js 20+ (via nvm recommended)
- pnpm (`npm install -g pnpm`)
- Docker Desktop
- Supabase CLI (`brew install supabase/tap/supabase` or `npm install -g supabase`)
- PlatformIO (VS Code extension)
- GitHub account

### 1. Clone & Install

```bash
git clone https://github.com/BBXtreme/Waterfront.git
cd Waterfront

cd waterfront-web
pnpm install
```

### 2. Run the PWA locally

Bash

```
cd waterfront-web
pnpm dev
```

→ [http://localhost:3000](http://localhost:3000/)

### 3. Local Supabase (Database + Auth + Realtime)

Bash

```
cd ../supabase-local
supabase init    # only first time
supabase start
```

→ Studio: [http://127.0.0.1:54323](http://127.0.0.1:54323/) Default login: postgres / postgres → Create tables / run migrations (see docs/ for schema)

Add to waterfront-web/.env.local:

env

```
NEXT_PUBLIC_SUPABASE_URL=http://127.0.0.1:54321 NEXT_PUBLIC_SUPABASE_ANON_KEY=eyJh...
```

Restart pnpm dev.

### 4. Local MQTT Broker (for ESP32 testing)

Bash

```
cd ../waterfront-infra
docker compose up -d
```

→ Mosquitto at mqtt://localhost:1883 (anonymous)

### 5. ESP32 Firmware (PlatformIO)

Open waterfront-esp32 folder in VS Code → use PlatformIO toolbar to build/upload.

## Deployment – Vercel (waterfront-web only)

1. Go to [https://vercel.com](https://vercel.com/) → New Project → Import Git Repository
2. Select this repo (BBXtreme/Waterfront)
3. **Important**: Set **Root Directory** = waterfront-web
4. Framework Preset: Next.js (auto-detected)
5. Add environment variables in Vercel dashboard (see below)
6. Deploy

→ Every push to main auto-deploys. Preview branches work automatically.

**Common monorepo fixes** (add to waterfront-web/next.config.mjs if build fails):

JavaScript

```
/** @type {import('next').NextConfig} */
const nextConfig = {
  // If you have shared/internal packages later
  transpilePackages: [],

  // Helps with monorepo tracing sometimes
  experimental: {
    outputFileTracingRoot: new URL('../../', import.meta.url).pathname,
  },
};

export default nextConfig;
```

## Environment Variables

### Local – waterfront-web/.env.local

env

```
# Supabase local 
NEXT_PUBLIC_SUPABASE_URL=http://127.0.0.1:54321 NEXT_PUBLIC_SUPABASE_ANON_KEY=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZS1kZW1vIiwicm9sZSI6ImFub24iLCJleHAiOjE5ODM4MTI5OTZ9.CRXP1A7WOeoJeXxjNni43kdQwgnWNReilDMblYTn_I0 

# MQTT (local dev) 
MQTT_BROKER_URL=mqtt://localhost:1883 

# Payments (add when ready) 
NEXT_PUBLIC_STRIPE_PUBLISHABLE_KEY=pk_test_... 
STRIPE_SECRET_KEY=sk_test_... 
BTCPAY_URL=https://your-btcpay-server.com BTCPAY_API_KEY=...
```

### Production – Vercel dashboard

Add these keys (no .env file in git):

env

```
NEXT_PUBLIC_SUPABASE_URL=https://your-project.supabase.co NEXT_PUBLIC_SUPABASE_ANON_KEY=eyJh... NEXT_PUBLIC_STRIPE_PUBLISHABLE_KEY=pk_live_... STRIPE_SECRET_KEY=sk_live_... BTCPAY_URL=https://your-btcpay-server.com BTCPAY_API_KEY=... # Optional – for admin/debug MQTT_BROKER_URL=wss://your-mosquitto-domain:8084/mqtt
```

**Rule of thumb**: NEXT_PUBLIC_ → client-side (browser), others → server/edge only.

## Useful Commands

| Task                 | Command                                     |
| -------------------- | ------------------------------------------- |
| Run PWA              | cd waterfront-web && pnpm dev               |
| Start local Supabase | cd supabase-local && supabase start         |
| Start MQTT broker    | cd waterfront-infra && docker compose up -d |
| Stop MQTT            | cd waterfront-infra && docker compose down  |
| Open whole project   | code . (from root)                          |

## Development Guidelines

- Use Aider: aider --model xai/grok-code-fast-1
- Commit often, small & clear messages
- Keep waterfront-web as priority until PWA flow is complete
- Document decisions & schemas in docs/
- Feature branches: git checkout -b feat/calendar-availability

## License

MIT License (to be confirmed)

Happy building! Questions → open an Issue
