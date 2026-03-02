# Waterfront Style Guide

**Version**: 1.1 (Refined for Page-Specific Application)  
**Date**: March 2026  
**Purpose**: Enforce consistent, professional, thematic UI across **all PWA screens** (booking flow, payment, confirmation, admin dashboard, telemetry, machine status).  
Blend: x.ai minimalism (clean cards, generous whitespace, subtle elegance) + thebitcoinportal.com energy (vibrant Bitcoin-orange gradients, bold CTAs for payments/crypto) + waterfront personality (fluid cyan-blues, subtle wave motifs for outdoor/booking feel).

## 1. Core Principles
- Minimal yet Energetic: Clean layouts + whitespace, punchy orange accents/gradients only for BTC/payment/CTAs.
- Mobile-First & PWA-Optimized: Responsive (sm/md/lg breakpoints), touch targets ≥44px, high outdoor contrast.
- Theming: Full light/dark mode via Tailwind `dark:` prefix + CSS variables.
- Accessibility: ≥4.5:1 contrast, visible focus rings (`focus-visible:ring-waterfront-primary`), reduced-motion friendly.
- Performance: Tailwind utilities only; no heavy custom CSS files.

## 2. Color Palette (Tailwind Extensions Required)

Define in `tailwind.config.js` → `theme.extend.colors` and `backgroundImage`:

- `--waterfront-primary`: #0EA5E9 (bright cyan-blue – booking actions, headers, availability)
- `--waterfront-primary-dark`: #0284C7 (hover/active)
- `--btc-accent`: #F7931A (Bitcoin orange – payments, success, BTC buttons/invoices)
- `--btc-accent-dark`: #D97706 (hover)
- Light `--background`: #FFFFFF / `--foreground`: #111827
- Dark `--background`: #0F172A / `--foreground`: #F1F5F9
- `--muted`: #6B7280
- `--border`: #E2E8F0 (light) / #334155 (dark)
- Success: #10B981 | Warning: #F59E0B | Destructive: #EF4444 | Info: #3B82F6

Gradients (add to `backgroundImage`):
- `btc-gradient`: linear-gradient(135deg, #F7931A 0%, #F5A623 100%)
- `wave-gradient`: linear-gradient(to right, #0EA5E9 0%, #0284C7 100%)
- `card-overlay-light`: linear-gradient(to bottom, rgba(255,255,255,0.05), rgba(255,255,255,0))

## 3. Typography

Add to `globals.css`:

```css
@import url('https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700&family=Space+Grotesk:wght@500;600;700&display=swap');

body { font-family: 'Inter', system-ui, sans-serif; }
h1, h2, h3 { font-family: 'Space Grotesk', sans-serif; }
```

- H1: font-bold text-4xl md:text-5xl tracking-tight
- H2: font-semibold text-3xl md:text-4xl
- H3: font-semibold text-2xl
- Body: text-base leading-relaxed text-foreground
- Small/Muted: text-sm text-muted-foreground
- Buttons/CTAs: font-medium text-base
- Numbers/PINs/Telemetry: font-medium (Space Grotesk)

## 4. Spacing & Layout

- Container: container mx-auto px-4 sm:px-6 lg:px-8 (center + responsive padding)
- Card padding: p-6 (standard) / p-8 (admin/large)
- Section spacing: py-12 md:py-16 lg:py-20
- Whitespace: space-y-6 md:space-y-8 gap-6 md:gap-10

## 5. Core Components (shadcn/ui Base)

### Buttons

Use variants in button.tsx:

- default: bg-waterfront-primary hover:bg-waterfront-primary-dark text-white shadow-sm hover:shadow
- btc: bg-btc-gradient hover:brightness-110 text-white shadow-md hover:shadow-lg
- outline: border-2 border-waterfront-primary text-waterfront-primary hover:bg-waterfront-primary/10
- ghost: hover:bg-accent/50
- destructive: bg-destructive text-destructive-foreground
- Sizes: sm (px-4 py-2), default (px-6 py-3), lg (px-8 py-4)

### Cards

- Base: border border-border bg-card shadow-sm rounded-xl overflow-hidden hover:shadow-md transition-shadow
- Admin/Telemetry: dark:bg-slate-800/50 backdrop-blur-sm border-slate-700

### Badges

- Available: bg-green-100 text-green-800 dark:bg-green-900/40 dark:text-green-300
- Booked/Overdue: bg-amber-100 text-amber-800 dark:bg-amber-900/40
- BTC/Paid: bg-btc-gradient/20 text-btc-accent border-btc-accent/30

### Inputs / Select / etc.

- border-input focus-visible:ring-2 focus-visible:ring-waterfront-primary focus-visible:border-waterfront-primary

### Wave Background (Waterfront twist)

Add class .bg-wave in globals.css:

CSS

```
.bg-wave {
  background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%),
              radial-gradient(circle at 20% 30%, rgba(14,165,233,0.12) 0%, transparent 60%);
  background-blend-mode: overlay;
}
```

## 6. Page-Specific Rules (Apply These Exactly!)

### Landing / Home Page

- Hero/Intro: .bg-wave text-white py-16 md:py-24 + H1 with text-5xl md:text-6xl
- CTA Button: btc variant (orange gradient) for "Book Now" or "Connect Wallet"
- Feature Cards: Minimal cards, waterfront-primary accents on icons/headings

### Booking Calendar / Selection Page

- Calendar/Header: bg-waterfront-primary/10 dark:bg-waterfront-primary/20 subtle tint
- Available Slots: bg-green-50 dark:bg-green-950/30 border-green-200
- Select/Add-ons: waterfront-primary focus rings, default button variant
- Next Button: waterfront-primary (blue) large size

### Checkout / Payment Page

- Payment Options: Radio cards with border-btc-accent/50 for BTC/Lightning choice
- BTC Button: btc variant + gradient, larger shadow, hover:brightness-125
- Fiat Button: default variant (blue)
- Summary Card: border-btc-accent/30 bg-gradient-to-b from-card to-card/80

### Booking Confirmation / PIN/QR Page

- Success Banner: bg-green-100 dark:bg-green-900/40 text-green-800 or bg-btc-gradient/10 text-btc-accent if BTC paid
- QR Code Card: Centered, p-8 border-2 border-waterfront-primary rounded-2xl shadow-lg
- PIN Display: Large mono font text-6xl tracking-widest font-bold text-waterfront-primary

### Admin Dashboard / Telemetry Overview

- Layout: Grid of cards (grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6)
- Status Cards: Minimal, hover:shadow-md, use badges for online/offline/battery
- BTC Payment Logs: Orange accents (text-btc-accent) on rows/cells
- Charts/Tables: Subtle bg-muted/50 rows, waterfront-primary for highlights

### Machine Status / Detail Page

- Header: bg-wave py-10 text-white
- Telemetry Cards: dark:bg-slate-800/60 backdrop-blur, icons in text-waterfront-primary
- Unlock/Override Button: destructive variant if needed, otherwise default

## 7. Dark Mode Adjustments

- Accent strength: Slightly desaturate orange/blue in dark mode (use brightness-90 if needed)
- Cards: Add transparency (bg-opacity-80 or /50)
- Text: Ensure --foreground high contrast

## 8. AI Coding Guidelines (Aider/Grok/Claude)

- Prefix every prompt: "Strictly follow STYLE_GUIDE.md v1.1 page-specific rules and use only defined Tailwind classes/variants."
- Always use custom variants (btc, default) instead of raw colors.
- For any page: "Apply page-specific rules from Section 6 for [Page Name]."
- After changes: "Ensure mobile responsiveness and dark mode support."

## 9. Next Steps / To-Do

- Update tailwind.config.js + globals.css with colors, gradients, fonts, .bg-wave.
- Extend shadcn/ui components with new variants (btc button, etc.).
- Add subtle transitions: transition-all duration-200 ease-in-out.
- Test outdoor visibility (high contrast mode).

Keep it clean, energetic where it counts (payments/BTC), and waterfront-fresh everywhere else!
