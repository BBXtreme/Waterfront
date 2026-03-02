export default function AdminConnectionsPage() {
  return (
    <div className="min-h-screen bg-background p-8">
      <div className="max-w-4xl mx-auto">
        <h1 className="text-3xl font-bold text-center mb-8">Connection Tests</h1>
        <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
          <Card className="shadow-sm hover:shadow-md transition-shadow rounded-xl border py-6 shadow-sm hover:shadow-md transition-shadow dark:bg-slate-800/50 backdrop-blur-sm">
            <CardHeader className="p-6 pb-0">
              <CardTitle className="font-medium">Supabase</CardTitle>
            </CardHeader>
            <CardContent className="p-6">
              <p className="text-muted-foreground mb-4">Test connection to Supabase database</p>
              <Button variant="outline" className="w-full">Test Supabase</Button>
            </CardContent>
          </Card>

          <Card className="shadow-sm hover:shadow-md transition-shadow rounded-xl border py-6 shadow-sm hover:shadow-md transition-shadow dark:bg-slate-800/50 backdrop-blur-sm">
            <CardHeader className="p-6 pb-0">
              <CardTitle className="font-medium">MQTT</CardTitle>
            </CardHeader>
            <CardContent className="p-6">
              <p className="text-muted-foreground mb-4">Test connection to MQTT broker</p>
              <Button variant="outline" className="w-full">Test MQTT</Button>
            </CardContent>
          </Card>

          <Card className="shadow-sm hover:shadow-md transition-shadow rounded-xl border py-6 shadow-sm hover:shadow-md transition-shadow dark:bg-slate-800/50 backdrop-blur-sm">
            <CardHeader className="p-6 pb-0">
              <CardTitle className="font-medium">Stripe</CardTitle>
            </CardHeader>
            <CardContent className="p-6">
              <p className="text-muted-foreground mb-4">Test connection to Stripe API</p>
              <Button variant="outline" className="w-full">Test Stripe</Button>
            </CardContent>
          </Card>

          <Card className="shadow-sm hover:shadow-md transition-shadow rounded-xl border py-6 shadow-sm hover:shadow-md transition-shadow dark:bg-slate-800/50 backdrop-blur-sm">
            <CardHeader className="p-6 pb-0">
              <CardTitle className="font-medium">BTCPay</CardTitle>
            </CardHeader>
            <CardContent className="p-6">
              <p className="text-muted-foreground mb-4">Test connection to BTCPay Server</p>
              <Button variant="btc" className="w-full">Test BTCPay</Button>
            </CardContent>
          </Card>
        </div>
      </div>
    </div>
  );
}
