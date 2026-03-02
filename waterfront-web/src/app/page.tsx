"use client";

import Link from "next/link";
import { Button } from '@/components/ui/button';

export default function Home() {
  return (
    <div className="min-h-screen bg-wave text-white flex flex-col items-center justify-center py-16 md:py-24 p-8">
      <h1 className="text-5xl md:text-6xl font-bold mb-6 text-center">Waterfront – Dev Start</h1>
      <p className="text-lg mb-8 text-center max-w-md text-white/80">
        Minimal setup • Nordend / Frankfurt • Connection tests first
      </p>
      <div className="space-y-8">
        <Link href="/test-connections">
          <Button variant="default" size="lg" className="transition-all duration-200 ease-in-out">
            → Start Connection Tests
          </Button>
        </Link>
        <Link href="/booking">
          <Button variant="btc" size="lg" className="transition-all duration-200 ease-in-out">
            → Start Booking Tests
          </Button>
        </Link>
      </div>
    </div>
  );
}
