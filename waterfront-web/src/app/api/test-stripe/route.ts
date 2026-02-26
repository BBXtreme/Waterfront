import { NextResponse } from 'next/server';

export async function GET() {
  // Simulate API response - in a real app, this would check actual Stripe connection
  const success = Math.random() > 0.3; // 70% chance of success for demo
  if (success) {
    return NextResponse.json({ success: true });
  } else {
    return NextResponse.json({ success: false, message: 'Invalid API key or network error' });
  }
}
