-- Option 1 (sha256 – immutable enough)
CREATE EXTENSION IF NOT EXISTS pgcrypto;

ALTER TABLE bookings
ADD COLUMN access_code text GENERATED ALWAYS AS (
    substring(encode(sha256((id::text || created_at::text)::bytea), 'hex'), 1, 6)
) STORED;