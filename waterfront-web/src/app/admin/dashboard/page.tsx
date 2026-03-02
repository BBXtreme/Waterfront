export default function AdminDashboardPage() {
  return (
    <div className="min-h-screen bg-gradient-to-br from-background to-muted/10 p-[50px]">
      <header className="bg-background/80 backdrop-blur-sm border-b sticky top-0 z-10 px-6 py-4 flex justify-between items-center">
        <h1 className="text-2xl font-semibold">Admin Dashboard</h1>
        <Link href="/admin/connections">
          <Button variant="outline">Run Connection Tests</Button>
        </Link>
      </header>

      <main className="mt-8">
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
          <Card className="shadow-sm hover:shadow-md transition-shadow rounded-xl border py-6 shadow-sm hover:shadow-md transition-shadow dark:bg-slate-800/50 backdrop-blur-sm">
            <CardHeader className="p-6 pb-0">
              <CardTitle className="font-medium">Machine Status</CardTitle>
            </CardHeader>
            <CardContent className="p-6">
              <div className="space-y-4">
                <div className="flex justify-between items-center">
                  <span>Compartment 1</span>
                  <Badge className="bg-green-100 text-green-700 dark:bg-green-900 dark:text-green-200">Available</Badge>
                </div>
                <div className="flex justify-between items-center">
                  <span>Compartment 2</span>
                  <Badge className="bg-amber-100 text-amber-800 dark:bg-amber-900">Booked</Badge>
                </div>
                <div className="flex justify-between items-center">
                  <span>Compartment 3</span>
                  <Badge className="bg-red-100 text-red-700 dark:bg-red-900 dark:text-red-200">Overdue</Badge>
                </div>
              </div>
            </CardContent>
          </Card>

          <Card className="shadow-sm hover:shadow-md transition-shadow rounded-xl border py-6 shadow-sm hover:shadow-md transition-shadow dark:bg-slate-800/50 backdrop-blur-sm">
            <CardHeader className="p-6 pb-0">
              <CardTitle className="font-medium">Recent Bookings</CardTitle>
            </CardHeader>
            <CardContent className="p-6">
              <div className="space-y-4">
                <div className="flex justify-between items-center">
                  <span>John Doe</span>
                  <Badge className="bg-btc-gradient/20 text-btc-accent border-btc-accent/30">BTC Paid</Badge>
                </div>
                <div className="flex justify-between items-center">
                  <span>Jane Smith</span>
                  <Badge className="bg-green-100 text-green-700 dark:bg-green-900 dark:text-green-200">Confirmed</Badge>
                </div>
                <div className="flex justify-between items-center">
                  <span>Bob Johnson</span>
                  <Badge className="bg-amber-100 text-amber-800 dark:bg-amber-900">Pending</Badge>
                </div>
              </div>
            </CardContent>
          </Card>

          <Card className="shadow-sm hover:shadow-md transition-shadow rounded-xl border py-6 shadow-sm hover:shadow-md transition-shadow dark:bg-slate-800/50 backdrop-blur-sm">
            <CardHeader className="p-6 pb-0">
              <CardTitle className="font-medium">Payment Logs</CardTitle>
            </CardHeader>
            <CardContent className="p-6">
              <div className="space-y-4">
                <div className="flex justify-between items-center">
                  <span>Transaction #1234</span>
                  <Badge className="bg-btc-accent text-white">BTC</Badge>
                </div>
                <div className="flex justify-between items-center">
                  <span>Transaction #1235</span>
                  <Badge className="bg-green-100 text-green-700 dark:bg-green-900 dark:text-green-200">Stripe</Badge>
                </div>
                <div className="flex justify-between items-center">
                  <span>Transaction #1236</span>
                  <Badge className="bg-btc-accent text-white">BTC</Badge>
                </div>
              </div>
            </CardContent>
          </Card>
        </div>
      </main>
    </div>
  );
}
