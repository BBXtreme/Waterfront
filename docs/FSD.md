# Functional Specification Document (FSD)

**Project**: WATERFRONT -> Kayak Rental Booking Application with ESP32-Cashless Controller Integration (incl. BTC/Lightning Payments)

**Document Title**: Functional Specification Document (FSD)

**Version**: 1.2 (Updated 23 Feb 2026)

**Date**: February 23, 2026

**Author**: Grok AI (xAI) – for BangLee (@BangLee8888), Bremen, DE

**Changes in 1.2**: Revised for scratch implementation cloning RentalBuddy frontend and functionality; removed bookcars references; emphasized Supabase + Next.js starters for modular build.

This FSD defines the functional requirements for the WATERFRONT system, a self-service unmanned kayak rental platform inspired by HEIUKI Share and RentalBuddy. It covers user flows, admin features, ESP32 controller logic, and integration points. The system enables 24/7 rentals with digital locks, sensor-based return confirmation, and payment via Stripe/BTCPay.

## 1. Overview

### 1.1 System Purpose

WATERFRONT is a web-based PWA (Progressive Web App) for booking kayak rentals, integrated with ESP32-based vending controllers for unmanned locations. Key features:

- **User-Facing**: Book online, pay (fiat/BTC), unlock compartment via PWA, return kayak, auto-lock.
- **Admin-Facing**: Dashboard for monitoring bookings, telemetry, payments, and remote control.
- **ESP32 Controller**: MQTT-connected lock controller with sensors, provisioning, and failover.

### 1.2 Scope

- **In Scope**: PWA booking/payment, ESP32 lock control, MQTT telemetry, Supabase backend, Stripe/BTCPay integration.
- **Out of Scope**: Physical kayak storage design, maintenance scheduling, multi-language support (start with EN/DE).

### 1.3 Assumptions

- Users have smartphones with internet access.
- Locations have WiFi/LTE coverage.
- ESP32 boards are pre-flashed with firmware.
- Payments are processed via webhooks.

## 2. User Stories

### 2.1 Customer User Stories

- **US-001**: As a customer, I want to browse available kayaks by date/time/location so I can choose a rental.
- **US-002**: As a customer, I want to book and pay online so I can get instant confirmation.
- **US-003**: As a customer, I want to unlock the compartment via PWA so I can access my kayak.
- **US-004**: As a customer, I want the compartment to auto-lock on return so I don't have to worry about it.
- **US-005**: As a customer, I want deposit release on timely return so I get my money back.

### 2.2 Admin User Stories

- **US-101**: As an admin, I want to monitor real-time telemetry so I can check system health.
- **US-102**: As an admin, I want to view bookings and payments so I can manage operations.
- **US-103**: As an admin, I want to remotely unlock compartments so I can assist customers.
- **US-104**: As an admin, I want to configure compartments/pins remotely so I can adapt to new locations.

## 3. Functional Requirements

### 3.1 PWA Frontend (Next.js)

- **FR-USER-01**: Display calendar with availability, allow selection of time slots.
- **FR-USER-02**: Integrate Stripe/BTCPay checkout for payments.
- **FR-USER-03**: Show booking confirmation with PIN/QR for unlock.
- **FR-USER-04**: Provide unlock button that publishes MQTT command.
- **FR-USER-05**: Handle return confirmation via sensor events.

### 3.2 Backend (Supabase)

- **FR-BACK-01**: Store bookings, payments, and telemetry in DB.
- **FR-BACK-02**: Handle webhooks from Stripe/BTCPay.
- **FR-BACK-03**: Publish MQTT commands on payment success.
- **FR-BACK-04**: Subscribe to ESP32 events for status updates.

### 3.3 ESP32 Controller

- **FR-ESP-01**: Connect to MQTT broker and subscribe to commands.
- **FR-ESP-02**: Control servo/relay for compartment locks.
- **FR-ESP-03**: Detect kayak presence with ultrasonic sensors.
- **FR-ESP-04**: Publish telemetry and events via MQTT.
- **FR-ESP-05**: Support WiFi provisioning (BLE/SoftAP).
- **FR-ESP-06**: Failover to LTE if WiFi fails.
- **FR-ESP-07**: Load configuration from LittleFS JSON.

### 3.4 Admin Dashboard

- **FR-ADMIN-01**: Display real-time ESP32 status.
- **FR-ADMIN-02**: List bookings with payment status.
- **FR-ADMIN-03**: Allow remote unlock commands.
- **FR-ADMIN-04**: Show telemetry charts (battery, RSSI).

## 4. System Architecture

### 4.1 High-Level Architecture

- **Frontend**: Next.js PWA → Supabase API.
- **Backend**: Supabase (DB + Auth + Functions) → MQTT Broker.
- **ESP32**: MQTT Client → Sensors/Actuators.
- **Payments**: Stripe/BTCPay → Webhooks → Supabase.

### 4.2 Data Flow

1. User books → PWA → Supabase → Payment → Webhook → MQTT Unlock → ESP32 → Open Lock.
2. User returns → Sensor → MQTT Event → Supabase → Deposit Release.

### 4.3 MQTT Topics

- `waterfront/{location}/{locationCode}/compartments/{compartmentNumber}/command`: Unlock commands.
- `waterfront/{location}/{locationCode}/compartments/{compartmentNumber}/status`: Telemetry.
- `waterfront/{location}/{locationCode}/config/update`: Remote config.

### 4.4 Database Schema (Supabase)

- **bookings**: id, user_id, compartment_id, start_time, end_time, status, payment_id.
- **payments**: id, booking_id, amount, method (stripe/btcpay), status.
- **telemetry**: id, esp32_id, timestamp, data (JSON).

### 4.5 ESP32 Controller Module

Configuration is centralized in LittleFS JSON → loaded on boot → supports dynamic compartment count/pins per location.

- **Inputs**: MQTT commands, sensor data.
- **Outputs**: Relay pulses, MQTT events.
- **Logic**: State machine for lock control, sensor polling, provisioning.

## 5. Non-Functional Requirements

- **NFR-001**: Response time <2s for unlock.
- **NFR-002**: 99% uptime for ESP32.
- **NFR-003**: Secure payments (TLS, PCI compliance).
- **NFR-004**: Mobile-first PWA design.

## 6. System Architecture

### 6.1 Component Diagram

- PWA → Supabase → MQTT → ESP32.
- ESP32: WiFi/LTE, Sensors, Actuators.

### 6.2 Sequence Diagrams

- Booking Flow: User → PWA → Supabase → Payment → MQTT → ESP32.
- Return Flow: Sensor → MQTT → Supabase → Deposit Release.

## 7. Interface Specifications

### 7.1 PWA API

- POST /api/bookings: Create booking.
- GET /api/availability: Get slots.

### 7.2 MQTT Interface

- Publish: Commands to ESP32.
- Subscribe: Events from ESP32.

### 7.3 ESP32 Hardware

- GPIO for relays/sensors.
- UART for LTE modem.

## 8. Security Requirements

- **SEC-001**: Encrypt MQTT traffic (TLS).
- **SEC-002**: Authenticate admin users.
- **SEC-003**: Validate payment webhooks.

## 9. Performance Requirements

- **PERF-001**: Handle 100 concurrent bookings.
- **PERF-002**: ESP32 deep sleep for battery life.

## 10. Testing Requirements

- **TEST-001**: Unit tests for ESP32 logic.
- **TEST-002**: Integration tests for MQTT flow.
- **TEST-003**: E2E tests for booking flow.

## 11. Risks and Mitigations

- **RISK-001**: MQTT connection loss → Mitigate with QoS 1 and offline queues.
- **RISK-002**: Payment failures → Mitigate with webhooks and retries.
- **RISK-003**: Hardware failures → Mitigate with sensor redundancy.
- **RISK-004**: Hard-coded configuration → Mitigated by runtime loading from /config.json on LittleFS with MQTT remote update. Reduces re-flash needs and enables admin-driven changes for new locations/compartments.

## 12. Deployment Plan

- **DEP-001**: Supabase for backend.
- **DEP-002**: Vercel for PWA.
- **DEP-003**: PlatformIO for ESP32 flashing.

## 13. Maintenance Plan

- **MAINT-001**: Monitor logs in Supabase.
- **MAINT-002**: Update firmware via OTA.

## 14. Beginner Developer Guide

- **GUIDE-001**: Start with Next.js for PWA.
- **GUIDE-002**: Use Supabase for quick backend.
- **GUIDE-003**: Flash ESP32 with PlatformIO.
- **GUIDE-004**: Test MQTT with Mosquitto.
- **GUIDE-005**: Configuration: All settings now live in one file (/config.json). Edit locally via PlatformIO data upload or remotely via MQTT. See TSD.md for schema.

## 15. Appendices

- **APP-001**: Wireframes for PWA.
- **APP-002**: ESP32 pinout diagram.
- **APP-003**: MQTT payload examples.
