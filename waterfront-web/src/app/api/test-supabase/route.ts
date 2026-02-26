import { NextResponse } from 'next/server';
import { createClient } from '@supabase/supabase-js';

export async function GET() {
  try {
    // Use environment variables for Supabase
    const supabase = createClient(
      process.env.NEXT_PUBLIC_SUPABASE_URL || '',
      process.env.NEXT_PUBLIC_SUPABASE_ANON_KEY || ''
    );
    
    // Simple test query to check connection
    const { data, error } = await supabase.from('mqtt_logs').select('*').limit(1);
    
    if (error) {
      return NextResponse.json({ success: false, message: error.message });
    }
    
    return NextResponse.json({ success: true });
  } catch (error: any) {
    return NextResponse.json({ success: false, message: error.message || 'Unknown error' });
  }
}
