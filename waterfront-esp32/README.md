# Waterfront – Unmanned Kayak & SUP Rental System

**Fully autonomous 24/7 solar-powered kayak and paddleboard rental platform.**  
Users book via PWA → pay (Stripe fiat or BTCPay Lightning/Liquid BTC) → receive PIN/QR code → unlock smart locker via ESP32 + **HiveMQ** MQTT → take equipment → sensors detect return → deposit auto-released.

**Current Status**: Early but functional prototype. Booking calendar, Supabase realtime, auth, and ESP32 MQTT unlock/status working locally. Payments, full sensor logic, deposit handling, and admin telemetry next.

## Core Concept
Mobile-first PWA booking → secure payment → instant access code → solar-powered ESP32 locker opens via MQTT command → presence sensors confirm return → deposit released automatically. No staff required.

## Features – Status

| Feature                      | Status  | Notes                        |
| ---------------------------- | ------- | ---------------------------- |
| Guest booking calendar       | Working | React + Supabase Realtime    |
| Real-time availability       | Working | Supabase subscriptions       |
| PIN/QR generation & delivery | Partial | On-screen + future email/SMS |
| ESP32 MQTT locker control    | Working | Unlock, status, telemetry    |
| Kayak presence sensors       | Planned | Ultrasonic + limit switches  |
| Deposit hold & auto-release  | Planned | MQTT events + backend logic  |
| Stripe + BTCPay payments     | Planned | Webhooks critical path       |
| Admin dashboard + telemetry  | Planned | Realtime Supabase            |
| Offline-tolerant PWA         | Partial | Cached QR/PIN                |



## Tech Stack

- **Frontend/PWA**: Next.js 15+ (App Router), TypeScript, Tailwind CSS, shadcn/ui, DaysiUI, **Zod** (schema validation),
- **Backend/Database/Auth**: Supabase (PostgreSQL + Auth + Realtime + Edge Functions + Storage)
- **Payments**: Stripe Checkout + BTCPay Server (Lightning & Liquid BTC)
- **IoT Controller**: ESP32-S3 (ESP-IDF), MQTT
- **MQTT Broker**: **HiveMQ Cloud** (free tier, managed, TLS-native)
- **Dev Tools**: pnpm workspaces, PlatformIO, Aider, Vitest



## Architecture Overview

Clean **REST + MQTT** hybrid:
- PWA ↔ Supabase (bookings, auth, realtime)
- Payment success → Supabase Edge Function publishes unlock to HiveMQ
- ESP32 subscribes to location-specific topics, controls gates/sensors, publishes status/telemetry
- Supabase processes events for deposit release and admin alerts



## Project Structure

waterfront/ 

├── waterfront-web/          # Next.js PWA 

├── waterfront-esp32/        # ESP32 firmware (PlatformIO + ESP-IDF) 

├── supabase-local/          # Local Supabase dev 

├── waterfront-infra/        # Optional HiveMQ CE Docker (for testing) 

├── STYLE_GUIDE.md           # UI/UX + coding bible 

├── .github/workflows/       # CI (PlatformIO tests) └── README.md



## Quick Start – Local Development

### 1. MQTT Broker – HiveMQ Cloud (Recommended)
1. Sign up free at [https://console.hivemq.cloud](https://console.hivemq.cloud)
2. Create a cluster
3. Copy **Cluster URL**, port (8883 TLS), username & password

### 2. Clone & Install
```bash
git clone https://github.com/bbxtreme/waterfront.git
cd waterfront
cd waterfront-web && pnpm installxxxxxxxxxx 
```

### 3. Environment Variables

Create waterfront-web/.env.local:

env

```
NEXT_PUBLIC_SUPABASE_URL=your-supabase-url 
NEXT_PUBLIC_SUPABASE_ANON_KEY=your-anon-key # HiveMQ Cloud (TLS required) MQTT_BROKER_URL=mqtts://username:password@your-cluster.hivemq.cloud:8883
```

### 4. Data Validation (Zod)

All forms, Supabase responses, MQTT payloads, and environment variables are validated with **Zod**.  
Schemas live in `waterfront-web/src/lib/schemas/` (barrel export `index.ts`).

- Booking forms, calendar dates, payment data
- Realtime Supabase subscriptions
- HiveMQ / MQTT JSON payloads
- `.env` validation at build time (`src/env.ts`)

See `STYLE_GUIDE.md` → “Validation & Type Safety” for full conventions.

### 5. Run PWA

Bash

```
cd waterfront-web
pnpm dev
# → http://localhost:3000
```

### 6. Local Supabase (optional)

Bash

```
cd supabase-local
supabase start
```

### 7. ESP32 Firmware

- Open waterfront-esp32 folder in VS Code + PlatformIO extension
- Update data/config.json with your HiveMQ credentials + location slug/code
- Build → Upload firmware + filesystem (pio run -t uploadfs)
- Monitor: pio device monitor

See waterfront-esp32 docs for full hardware pinout and MQTT topics.



## MQTT Topics (HiveMQ)

Dynamic per location: 

waterfront/{location-slug}/{site-code}/compartments/{id}/command 

waterfront/{location-slug}/{site-code}/compartments/{id}/status 

waterfront/{location-slug}/{site-code}/config/update (remote config reload)



## Development Guidelines

- Follow STYLE_GUIDE.md strictly (web + embedded)
- Small, focused commits with clear messages
- Use **Aider** for code generation/refactoring... 
- **Always use Zod schemas** for any new form, API response, or MQTT payload (see STYLE_GUIDE.md)
- Test MQTT commands via HiveMQ Web Client or MQTTX CLI



## License

MIT License
