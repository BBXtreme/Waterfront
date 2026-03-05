## 10. Data Validation & Type Safety (Zod)

**Zod** is the single source of truth for all data shapes in the Waterfront PWA.

### File Structure (create these)



waterfront-web/ └── src/ ├── lib/ │   ├── schemas/ │   │   ├── index.ts              # barrel export │   │   ├── booking.ts │   │   ├── mqtt.ts │   │   ├── supabase.ts │   │   ├── payment.ts │   │   └── env.ts                # environment validation │   └── env.ts                    # re-export validated env



### Core Conventions (strictly follow)
- Every schema must be exported as `const xxxSchema = z.object({...})`
- Export the inferred type: `export type Booking = z.infer<typeof bookingSchema>`
- Use `.refine()` / `.superRefine()` for business rules (date ranges, compartment availability, BTC amount > 0)
- Error messages must be user-friendly and match STYLE_GUIDE tone (e.g. “Booking must start within 48 hours”)
- Combine with `react-hook-form` + `@hookform/resolvers/zod` for all forms
- MQTT payloads: parse on receipt with `.parse()` or `.safeParse()` + graceful fallback
- Supabase responses: wrap `.select()` results with schema.parse() in data layer

### Example – booking.ts
```ts
import { z } from "zod";

export const bookingSchema = z.object({
  locationSlug: z.string().min(1, "Location is required"),
  compartmentId: z.number().int().positive(),
  startDate: z.date().refine(d => d > new Date(), "Start date must be in the future"),
  durationHours: z.number().int().min(1).max(48),
  paymentMethod: z.enum(["stripe", "btc"]),
}).refine(data => {
  // custom business rule example
  return true;
}, { message: "Invalid booking combination" });

export type Booking = z.infer<typeof bookingSchema>;
```

### AI Coding Instructions (add to every Aider prompt)

"Strictly follow STYLE_GUIDE.md v1.1, especially Section 10 (Zod). Use existing schemas from src/lib/schemas/, never invent raw interfaces. Always validate MQTT payloads and form data with .parse() or safeParse()."

### Environment Validation

src/env.ts (create if missing):

TypeScript

```
import { z } from "zod";

const envSchema = z.object({
  NEXT_PUBLIC_SUPABASE_URL: z.string().url(),
  NEXT_PUBLIC_SUPABASE_ANON_KEY: z.string().min(1),
  MQTT_BROKER_URL: z.string().url(),
});

export const env = envSchema.parse(process.env);
```

### Benefits Enforced

- Zero runtime type errors in booking → payment → MQTT unlock flow
- Perfect TypeScript inference with shadcn forms
- Future-proof for admin telemetry and deposit logic

