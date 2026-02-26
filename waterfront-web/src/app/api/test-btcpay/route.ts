import { NextResponse } from 'next/server';

export async function GET() {
  // Simulate API response - in a real app, this would check actual BTCPay connection
  const success = Math.random() > 0.5; // 50% chance of success for demo
  if (success) {
    return NextResponse.json({ success: true });
  } else {
    return NextResponse.json({ success: false, message: 'Connection refused or invalid credentials' });
  }
}
