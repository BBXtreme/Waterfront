'use client';

import { useSearchParams } from 'next/navigation';
import { Card, CardContent, CardHeader, CardTitle } from '@/components/ui/card';
import { Badge } from '@/components/ui/badge';

export default function ConfirmationPage() {
  const searchParams = useSearchParams();
  const name = searchParams.get('name') || 'N/A';
  const email = searchParams.get('email') || 'N/A';
  const date = searchParams.get('date') || 'N/A';
  const time = searchParams.get('time') || 'N/A';
  const kayakType = searchParams.get('kayakType') || 'N/A';

  return (
    <div className="min-h-screen bg-gradient-to-br from-background to-muted/10 p-[50px]">
      <div className="max-w-2xl mx-auto">
        <Card className="shadow-sm rounded-lg overflow-hidden bg-card border border-border">
          <CardHeader className="p-[25px] pb-0">
            <CardTitle className="font-medium text-center">Booking Confirmation</CardTitle>
          </CardHeader>
          <CardContent className="p-[25px] space-y-4">
            <div className="text-center">
              <Badge variant="success" className="mb-4">Booking Confirmed</Badge>
              <p className="text-muted-foreground">Thank you for your booking! Here are the details:</p>
            </div>
            <div className="space-y-2">
              <p><strong>Name:</strong> {name}</p>
              <p><strong>Email:</strong> {email}</p>
              <p><strong>Date:</strong> {date}</p>
              <p><strong>Time:</strong> {time}</p>
              <p><strong>Kayak Type:</strong> {kayakType}</p>
            </div>
            <p className="text-sm text-muted-foreground">
              You will receive a confirmation email shortly. Please arrive 15 minutes early for check-in.
            </p>
          </CardContent>
        </Card>
      </div>
    </div>
  );
}
