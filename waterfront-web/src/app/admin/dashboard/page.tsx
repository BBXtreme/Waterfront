import MachineCard from '@/components/MachineCard';
import { Button } from '@/components/ui/button';

import { useState } from 'react';
import MachineCard from '@/components/MachineCard';
import { Button } from '@/components/ui/button';
import { Card, CardHeader, CardTitle, CardDescription, CardContent } from '@/components/ui/card';
import { cn } from '@/lib/utils';

export default function AdminDashboardPage() {
  const [stripeStatus, setStripeStatus] = useState<string>("Not Tested");
  const [stripeResult, setStripeResult] = useState<string>("");
  const [btcPayStatus, setBTCPayStatus] = useState<string>("Not Tested");
  const [btcPayResult, setBTCPayResult] = useState<string>("");
  const [supabaseStatus, setSupabaseStatus] = useState<string>("Not Tested");
  const [supabaseResult, setSupabaseResult] = useState<string>("");
  
  const testConnection = async (url: string, setStatus: (status: string) => void, setResult: (result: string) => void) => {
    setStatus("Pending");
    try {
      const response = await fetch(url);
      const data = await response.json();
      if (data.success) {
        setStatus("OK");
        setResult(`Last test: OK - just now`);
      } else {
        setStatus("Error");
        setResult(`Failed: ${data.message || 'Connection error'}`);
      }
    } catch (error) {
      setStatus("Error");
      setResult(`Failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
    }
  };
  
  const testStripeConnection = () => testConnection('/api/test-stripe', setStripeStatus, setStripeResult);
  const testBTCPayConnection = () => testConnection('/api/test-btcpay', setBTCPayStatus, setBTCPayResult);
  const testSupabaseConnection = () => testConnection('/api/test-supabase', setSupabaseStatus, setSupabaseResult);

  return (
    <div className="min-h-screen bg-gradient-to-br from-background to-muted/10 p-[50px]">
      <header className="bg-background/80 backdrop-blur-sm border-b sticky top-0 z-10 px-6 py-4 flex justify-between items-center shadow-lg rounded-lg mb-8">
        <h1 className="text-2xl font-semibold">Admin Dashboard</h1>
      </header>

      <main className="max-w-7xl mx-auto">
        <h2 className="text-xl font-medium text-center mb-6">Overview</h2>
        <div className="grid grid-cols-1 md:grid-cols-3 gap-2.5">
          <MachineCard
            title="Machine 1"
            status="connected"
            message="Operational"
            timestamp="2023-10-01 12:00:00"
            isConnected={true}
          >
            <div className="flex gap-4">
              <Button className="px-5 py-2.5 mr-2.5">Action</Button>
            </div>
          </MachineCard>

          <MachineCard
            title="Machine 2"
            status="disconnected"
            message="Offline"
            timestamp="2023-10-01 11:00:00"
            isConnected={false}
          >
            <div className="flex gap-4">
              <Button className="px-5 py-2.5 mr-2.5">Action</Button>
            </div>
          </MachineCard>

          <MachineCard
            title="Machine 3"
            status="connecting"
            message="Reconnecting..."
            timestamp="2023-10-01 10:00:00"
            isConnected={false}
          >
            <div className="flex gap-4">
              <Button className="px-5 py-2.5 mr-2.5">Action</Button>
            </div>
          </MachineCard>
        </div>
        
        <div className="mt-10 border-t pt-8">
          <h2 className="text-lg font-semibold mb-4">Payment Gateway Tests</h2>
          <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
            <Card className="max-w-xs">
              <CardHeader className="p-4 pb-0">
                <CardTitle className="text-md font-medium">Stripe Status</CardTitle>
                <CardDescription>Check connection to Stripe API</CardDescription>
              </CardHeader>
              <CardContent className="p-4">
                <div className="flex items-center gap-2 mb-4">
                  <div 
                    className={cn(
                      "w-3 h-3 rounded-full",
                      stripeStatus === "OK" ? "bg-green-500" : stripeStatus === "Pending" ? "bg-yellow-500" : "bg-red-500"
                    )}
                  />
                  <span className="text-sm">{stripeStatus}</span>
                </div>
                <Button 
                  variant="outline" 
                  size="sm" 
                  onClick={testStripeConnection}
                  disabled={stripeStatus === "Pending"}
                >
                  Test Connection
                </Button>
                <p className="text-xs text-muted-foreground mt-2">{stripeResult}</p>
              </CardContent>
            </Card>
            
            <Card className="max-w-xs">
              <CardHeader className="p-4 pb-0">
                <CardTitle className="text-md font-medium">BTCPay/Lightning Status</CardTitle>
                <CardDescription>Check connection to BTCPay Server</CardDescription>
              </CardHeader>
              <CardContent className="p-4">
                <div className="flex items-center gap-2 mb-4">
                  <div 
                    className={cn(
                      "w-3 h-3 rounded-full",
                      btcPayStatus === "OK" ? "bg-green-500" : btcPayStatus === "Pending" ? "bg-yellow-500" : "bg-red-500"
                    )}
                  />
                  <span className="text-sm">{btcPayStatus}</span>
                </div>
                <Button 
                  variant="outline" 
                  size="sm" 
                  onClick={testBTCPayConnection}
                  disabled={btcPayStatus === "Pending"}
                >
                  Test Connection
                </Button>
                <p className="text-xs text-muted-foreground mt-2">{btcPayResult}</p>
              </CardContent>
            </Card>
            
            <Card className="max-w-xs">
              <CardHeader className="p-4 pb-0">
                <CardTitle className="text-md font-medium">Supabase Status</CardTitle>
                <CardDescription>Check connection to Supabase</CardDescription>
              </CardHeader>
              <CardContent className="p-4">
                <div className="flex items-center gap-2 mb-4">
                  <div 
                    className={cn(
                      "w-3 h-3 rounded-full",
                      supabaseStatus === "OK" ? "bg-green-500" : supabaseStatus === "Pending" ? "bg-yellow-500" : "bg-red-500"
                    )}
                  />
                  <span className="text-sm">{supabaseStatus}</span>
                </div>
                <Button 
                  variant="outline" 
                  size="sm" 
                  onClick={testSupabaseConnection}
                  disabled={supabaseStatus === "Pending"}
                >
                  Test Connection
                </Button>
                <p className="text-xs text-muted-foreground mt-2">{supabaseResult}</p>
              </CardContent>
            </Card>
          </div>
        </div>
      </main>
    </div>
  );
}
