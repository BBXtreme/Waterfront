# Waterfront Project Coding Conventions

This CONVENTIONS.md file provides guidelines for maintaining consistent, high-quality code across the Waterfront monorepo. It is designed to be loaded into Aider sessions (e.g., via `aider --read CONVENTIONS.md`) to ensure AI-assisted code generation aligns with best practices. These conventions draw from established standards like Airbnb TypeScript, Google C++ Style Guide (adapted for embedded), ESP-IDF recommendations, and Next.js best practices, tailored to our tech stack: TypeScript/Next.js for the web app, C++/ESP-IDF for the ESP32 firmware, and Markdown for documentation.

Follow these rules strictly when writing, editing, or refactoring code. Prioritize readability, maintainability, security, and performance—especially for the embedded IoT component where resources are limited.

## General Principles
- **Clean Code**: Write code that is self-explanatory. Use meaningful names, avoid abbreviations unless standard (e.g., MQTT, GPIO). Keep functions small (< 20 lines where possible) and focused on one responsibility (SOLID principles).
- **Error Handling**: Always handle errors explicitly. Use try-catch in TypeScript; in C++, use return codes or exceptions sparingly (prefer RAII). Log errors with context but avoid exposing sensitive info.
- **Security**: Sanitize inputs, use prepared statements in Supabase queries, enable TLS for MQTT, and follow OWASP guidelines for web. Never hardcode secrets—use environment variables or config files.
- **Performance**: Optimize for low-power in ESP32 (e.g., deep sleep, efficient loops). In web, use React Query for caching and lazy loading.
- **Internationalization**: Prepare for i18n (e.g., use next-intl if needed), but English-only for now.
- **Accessibility**: Follow WCAG in web UI (e.g., alt text for images, semantic HTML).
- **Version Control**: Use conventional commits (feat:, fix:, refactor:, chore:, docs:, test:). Branch names: `feat/booking-payments`, `fix/esp32-mqtt`. Keep commits atomic and descriptive.

## Git and Workflow Best Practices
- **Monorepo Structure**: Keep subprojects isolated (e.g., waterfront-web/, waterfront-esp32/). Use pnpm workspaces for dependency management.
- **Aider Usage**: Start sessions with `--read CONVENTIONS.md --model xai/grok-code-fast-1`. Provide clear, step-by-step instructions. Review diffs thoroughly before accepting.
- **CI/CD**: Use GitHub Actions for linting, testing, and deployment (Vercel for web, PlatformIO for ESP32 builds).
- **Dependencies**: Pin versions in package.json/platformio.ini. Audit regularly (pnpm audit, PlatformIO checks).
- **Code Reviews**: Self-review via Aider diffs; for major changes, use PRs even if solo.

## TypeScript Conventions (waterfront-web/)
- **Style Guide Base**: Airbnb TypeScript + Prettier/Tailwind rules.
- **Formatting**: Use Prettier (line length 100). Enforce with ESLint (airbnb-typescript base + custom: no-console in prod, prefer-arrow-functions).
- **Type Safety**: Strict mode enabled. Use interfaces for props/types; prefer type over interface for simplicity. Avoid `any`; use generics where appropriate.
- **Components**: Functional only with hooks. Use shadcn/ui primitives. Colocate styles/logic (e.g., Button.tsx with Tailwind classes).
- **File Structure**:
  - `app/`: Next.js App Router pages (e.g., page.tsx for routes).
  - `components/`: Reusable UI (e.g., BookingCalendar.tsx).
  - `lib/`: Utilities, services (e.g., supabaseClient.ts, mqttService.ts).
  - `types/`: Shared types (e.g., Booking.ts).
  - Naming: PascalCase for components (BookingForm.tsx), camelCase for utils (getAvailability.ts).
- **Imports**: Absolute paths via tsconfig (`@/components/...`). Group imports: React/Next first, then third-party, then local.
- **State Management**: Use React Query for data fetching/mutations; Zustand for simple global state (e.g., auth). Avoid Redux.
- **API Interactions**: Use Supabase JS client for DB/auth/realtime. Handle Stripe/BTCPay webhooks in API routes.
- **PWA Features**: Use next-pwa for offline support; cache QR/PINs.
- **Testing**: Vitest/Jest for units (components, utils). React Testing Library for UI. Aim for 80% coverage on core flows (booking, payments).
- **Example Snippet** (for Aider reference):
  ```tsx
  // app/booking/page.tsx
  import { useQuery } from '@tanstack/react-query';
  import { supabase } from '@/lib/supabaseClient';
  
  interface BookingProps {
    slotId: string;
  }
  
  export default function BookingPage({ params }: { params: BookingProps }) {
    const { data: availability, isLoading } = useQuery({
      queryKey: ['availability', params.slotId],
      queryFn: async () => {
        const { data, error } = await supabase.from('slots').select('*').eq('id', params.slotId);
        if (error) throw error;
        return data;
      },
    });
  
    if (isLoading) return <div>Loading...</div>;
  
    return (
      <div className="container mx-auto p-4">
        <h1 className="text-2xl font-bold">Book Slot {params.slotId}</h1>
        {/* ... */}
      </div>
    );
  }

## C++ Conventions (waterfront-esp32/)

- **Style Guide Base**: Google C++ (indent 2 spaces, 80-col line limit) + ESP-IDF recommendations. Use clang-format.

- **Language Features**: C++11+ (auto, lambdas, smart pointers). Avoid raw pointers; use std::unique_ptr where possible.

- **File Structure**:

  - src/: Main sources (main.cpp, mqtt_client.cpp, gate_controller.cpp).
  - include/: Headers.
  - test/: Unit tests (Catch2).
  - Naming: snake_case for functions/variables, CamelCase for classes. Files: lowercase_with_underscores.cpp.

- **Modularity**: One class/module per file. Use namespaces (e.g., namespace waterfront).

- **ESP-IDF Specific**: Use esp_err_t for error returns. Prefer FreeRTOS tasks over raw loops for concurrency. Configure via menuconfig/sdkconfig.

- **Configuration**: Load from config.json (SPIFFS/LittleFS). Use nlohmann/json for parsing.

- **MQTT/Sensors**: Use esp-mqtt or PubSubClient. Handle reconnects gracefully. For sensors (ultrasonic), debounce and filter readings.

- **Power Management**: Implement deep sleep; wake on MQTT or timer. Monitor ADC for battery/solar.

- **Testing**: Unit tests with Catch2. Mock ESP-IDF APIs for isolation.

- **Build**: PlatformIO only. Define partitions for OTA updates.

- **Example Snippet** (for Aider reference):

  - C++

    ```
    // src/mqtt_client.cpp
    #include "mqtt_client.h"
    #include <esp_log.h>
    #include <nlohmann/json.hpp>
    #include "config_loader.h"
    
    namespace waterfront {
    
    static const char* TAG = "MQTT_CLIENT";
    
    esp_err_t MqttClient::init() {
      Config config = ConfigLoader::load();
      mqtt_config.uri = config.mqtt_server.c_str();
      mqtt_config.client_id = config.device_id.c_str();
      // TLS setup...
      client = esp_mqtt_client_init(&mqtt_config);
      if (!client) return ESP_FAIL;
    
      esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, event_handler, this);
      return esp_mqtt_client_start(client);
    }
    
    void MqttClient::event_handler(void* args, esp_event_base_t base, int32_t event_id, void* data) {
      // Handle connect, disconnect, data...
      ESP_LOGI(TAG, "MQTT Event: %d", event_id);
    }
    
    }  // namespace waterfront
    ```

  ## Documentation Conventions

  - **Format**: Markdown (.md) files in root on subfolder /docs/ folders (e.g., TSD.md for Technical Specs, FSD.md for Functional).
  - **Content**: Use headings, tables, diagrams (Mermaid/PlantUML). Update after major changes.
  - **Specs**: Include flows, schemas, edge cases. For ESP32: pinouts, test checklists.
  - **Inline Docs**: Doxygen-style in C++ headers. JSDoc in TypeScript.

  ## Testing and Quality

  - **Coverage**: >80% on critical paths (bookings, unlocks, sensors).
  - **Tools**: Catch2 (C++), Vitest (TS). Integration tests for MQTT flows.
  - **Linting**: ESLint/Prettier (web), clang-tidy (ESP32).

  

  Load this file in every Aider session to maintain consistency. If evolving the project (e.g., adding Python scripts), update this document accordingly.