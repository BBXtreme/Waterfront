import Link from 'next/link';
import { Button } from '@/components/ui/button';

export default function Home() {
  return (
    <div className="min-h-screen bg-wave text-white flex flex-col items-center justify-center py-16 md:py-24">
      <h1 className="text-5xl md:text-6xl font-bold mb-6 text-center">Waterfront – Dev Start</h1>
      <p className="text-lg mb-8 text-center max-w-md text-white/80">
        Minimal setup • Nordend / Frankfurt • Connection tests first
      </p>
      <div className="space-y-8">
        <Link href="/test-connections">
          <Button variant="default" size="lg" className="transition-all duration-200 ease-in-out">
            Test Connections
          </Button>
        </Link>
        <Link href="/admin">
          <Button variant="outline" size="lg" className="transition-all duration-200 ease-in-out">
            Admin Panel
          </Button>
        </Link>
        <Link href="/booking">
          <Button variant="default" size="lg" className="transition-all duration-200 ease-in-out">
            Book a Kayak
          </Button>
        </Link>
        <Link href="/payment">
          <Button variant="btc" size="lg" className="transition-all duration-200 ease-in-out">
            Make Payment
          </Button>
        </Link>
        <Link href="/confirmation">
          <Button variant="default" size="lg" className="transition-all duration-200 ease-in-out">
            View Confirmation
          </Button>
        </Link>
        <Link href="/admin/login">
          <Button variant="outline" size="lg" className="transition-all duration-200 ease-in-out">
            Admin Login
          </Button>
        </Link>
      </div>
    </div>
  );
}
