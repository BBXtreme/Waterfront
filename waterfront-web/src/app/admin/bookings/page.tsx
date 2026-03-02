export default function AdminBookingsPage() {
  return (
    <div className="min-h-screen bg-background p-8">
      <div className="max-w-6xl mx-auto">
        <h1 className="text-3xl font-bold text-center mb-8">Booking Management</h1>
        <Card className="shadow-sm hover:shadow-md transition-shadow rounded-xl border py-6 shadow-sm hover:shadow-md transition-shadow dark:bg-slate-800/50 backdrop-blur-sm">
          <CardHeader className="p-6 pb-0">
            <CardTitle className="font-medium">Recent Bookings</CardTitle>
          </CardHeader>
          <CardContent className="p-6">
            <div className="space-y-4">
              <div className="flex justify-between items-center p-4 bg-muted/50 rounded-lg">
                <div>
                  <p className="font-medium">John Doe</p>
                  <p className="text-sm text-muted-foreground">Single Kayak - 2024-01-15 10:00 AM</p>
                </div>
                <div className="flex gap-2">
                  <Badge className="bg-btc-accent text-white">BTC Paid</Badge>
                  <Button variant="outline" size="sm">Edit</Button>
                </div>
              </div>
              <div className="flex justify-between items-center p-4 bg-muted/50 rounded-lg">
                <div>
                  <p className="font-medium">Jane Smith</p>
                  <p className="text-sm text-muted-foreground">Double Kayak - 2024-01-16 2:00 PM</p>
                </div>
                <div className="flex gap-2">
                  <Badge className="bg-green-100 text-green-700 dark:bg-green-900 dark:text-green-200">Confirmed</Badge>
                  <Button variant="outline" size="sm">Edit</Button>
                </div>
              </div>
              <div className="flex justify-between items-center p-4 bg-muted/50 rounded-lg">
                <div>
                  <p className="font-medium">Bob Johnson</p>
                  <p className="text-sm text-muted-foreground">Canoe - 2024-01-17 11:00 AM</p>
                </div>
                <div className="flex gap-2">
                  <Badge className="bg-amber-100 text-amber-800 dark:bg-amber-900">Pending</Badge>
                  <Button variant="outline" size="sm">Edit</Button>
                </div>
              </div>
            </div>
          </CardContent>
        </Card>
      </div>
    </div>
  );
}
