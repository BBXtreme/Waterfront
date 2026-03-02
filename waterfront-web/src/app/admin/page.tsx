import Link from 'next/link';
import { Button } from '@/components/ui/button';
import { Card, CardContent, CardHeader, CardTitle } from '@/components/ui/card';

export default function AdminPage() {
  return (
    <div className="min-h-screen bg-background py-12 md:py-16 lg:py-20">
      <div className="container mx-auto px-4 sm:px-6 lg:px-8">
        <h1 className="font-bold text-4xl md:text-5xl tracking-tight mb-8">Admin Panel</h1>
        <div className="grid grid-cols-1 md:grid-cols-3 gap-6">
          <Card className="shadow-sm hover:shadow-md transition-shadow rounded-xl border py-6 shadow-sm hover:shadow-md transition-shadow dark:bg-slate-800/50 backdrop-blur-sm">
            <CardHeader>
              <CardTitle>Dashboard</CardTitle>
            </CardHeader>
            <CardContent>
              <p className="text-muted-foreground mb-4">Overview of machines and bookings.</p>
              <Link href="/admin/dashboard">
                <Button variant="default" size="default" className="transition-all duration-200 ease-in-out">
                  Go to Dashboard
                </Button>
              </Link>
            </CardContent>
          </Card>
          <Card className="shadow-sm hover:shadow-md transition-shadow rounded-xl border py-6 shadow-sm hover:shadow-md transition-shadow dark:bg-slate-800/50 backdrop-blur-sm">
            <CardHeader>
              <CardTitle>Bookings</CardTitle>
            </CardHeader>
            <CardContent>
              <p className="text-muted-foreground mb-4">Manage customer bookings.</p>
              <Link href="/admin/bookings">
                <Button variant="default" size="default" className="transition-all duration-200 ease-in-out">
                  Manage Bookings
                </Button>
              </Link>
            </CardContent>
          </Card>
          <Card className="shadow-sm hover:shadow-md transition-shadow rounded-xl border py-6 shadow-sm hover:shadow-md transition-shadow dark:bg-slate-800/50 backdrop-blur-sm">
            <CardHeader>
              <CardTitle>Machines</CardTitle>
            </CardHeader>
            <CardContent>
              <p className="text-muted-foreground mb-4">Monitor and control machines.</p>
              <Link href="/admin/machines">
                <Button variant="default" size="default" className="transition-all duration-200 ease-in-out">
                  Manage Machines
                </Button>
              </Link>
            </CardContent>
          </Card>
          <Card className="shadow-sm hover:shadow-md transition-shadow rounded-xl border py-6 shadow-sm hover:shadow-md transition-shadow dark:bg-slate-800/50 backdrop-blur-sm">
            <CardHeader>
              <CardTitle>Connections</CardTitle>
            </CardHeader>
            <CardContent>
              <p className="text-muted-foreground mb-4">Check system connections.</p>
              <Link href="/admin/connections">
                <Button variant="default" size="default" className="transition-all duration-200 ease-in-out">
                  View Connections
                </Button>
              </Link>
            </CardContent>
          </Card>
          <Card className="shadow-sm hover:shadow-md transition-shadow rounded-xl border py-6 shadow-sm hover:shadow-md transition-shadow dark:bg-slate-800/50 backdrop-blur-sm">
            <CardHeader>
              <CardTitle>Logs</CardTitle>
            </CardHeader>
            <CardContent>
              <p className="text-muted-foreground mb-4">View system logs.</p>
              <p className="text-btc-accent">BTC logs available</p>
              <Link href="/admin/logs">
                <Button variant="default" size="default" className="transition-all duration-200 ease-in-out">
                  View Logs
                </Button>
              </Link>
            </CardContent>
          </Card>
        </div>
      </div>
    </div>
  );
}
