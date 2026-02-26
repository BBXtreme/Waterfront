create or replace function update_updated_at()
returns trigger as $$    
begin
  new.updated_at = now();
  return new;
end;
    $$ language plpgsql;

create trigger bookings_updated_at
  before update on bookings
  for each row execute function update_updated_at();