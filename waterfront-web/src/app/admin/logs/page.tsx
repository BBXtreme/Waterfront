export default function AdminLogsPage() {
  return (
    <div className="min-h-screen bg-background p-8">
      <div className="max-w-6xl mx-auto">
        <h1 className="text-3xl font-bold text-center mb-8">System Logs</h1>
        <Card className="shadow-sm hover:shadow-md transition-shadow rounded-xl border py-6 shadow-sm hover:shadow-md transition-shadow dark:bg-slate-800/50 backdrop-blur-sm">
          <CardHeader className="p-6 pb-0">
            <CardTitle className="font-medium">Recent Logs</CardTitle>
          </CardHeader>
          <CardContent className="p-6">
            <div className="space-y-4">
              <div className="flex justify-between items-center p-4 bg-muted/50 rounded-lg">
                <div>
                  <p className="font-medium">MQTT Connection</p>
                  <p className="text-sm text-muted-foreground">Connected to broker successfully</p>
                </div>
                <Badge className="bg-green-100 text-green-700 dark:bg-green-900 dark:text-green-200">Success</Badge>
              </div>
              <div className="flex justify-between items-center p-4 bg-muted/50 rounded-lg">
                <div>
                  <p className="font-medium">Booking Created</p>
                  <p className="text-sm text-muted-foreground">New booking for John Doe</p>
                </div>
                <Badge className="bg-btc-accent text-white">BTC Payment</Badge>
              </div>
              <div className="flex justify-between items-center p-4 bg-muted/50 rounded-lg">
                <div>
                  <p className="font-medium">Payment Processed</p>
                  <p className="text-sm text-muted-foreground">Stripe payment for booking #1234</p>
                </div>
                <Badge className="bg-green-100 text-green-700 dark:bg-green-900 dark:text-green-200">Success</Badge>
              </div>
            </div>
          </CardContent>
        </Card>
      </div>
    </div>
  );
}
