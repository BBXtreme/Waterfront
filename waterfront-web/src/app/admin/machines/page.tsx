export default function AdminMachinesPage() {
  return (
    <div className="bg-wave text-white py-10">
      <div className="max-w-6xl mx-auto px-4 sm:px-6 lg:px-8">
        <h1 className="text-3xl font-bold text-center mb-8">Machine Management</h1>
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
          <Card className="shadow-sm hover:shadow-md transition-shadow rounded-xl border py-6 shadow-sm hover:shadow-md transition-shadow dark:bg-slate-800/60 backdrop-blur">
            <CardHeader className="p-6 pb-0">
              <CardTitle className="font-medium">Compartment 1</CardTitle>
            </CardHeader>
            <CardContent className="p-6">
              <div className="space-y-4">
                <div className="flex justify-between items-center">
                  <span>Status</span>
                  <Badge className="bg-green-100 text-green-700 dark:bg-green-900 dark:text-green-200">Available</Badge>
                </div>
                <div className="flex justify-between items-center">
                  <span>Battery</span>
                  <Badge className="bg-green-100 text-green-700 dark:bg-green-900 dark:text-green-200">95%</Badge>
                </div>
                <Button variant="outline" className="w-full">Unlock</Button>
              </div>
            </CardContent>
          </Card>

          <Card className="shadow-sm hover:shadow-md transition-shadow rounded-xl border py-6 shadow-sm hover:shadow-md transition-shadow dark:bg-slate-800/60 backdrop-blur">
            <CardHeader className="p-6 pb-0">
              <CardTitle className="font-medium">Compartment 2</CardTitle>
            </CardHeader>
            <CardContent className="p-6">
              <div className="space-y-4">
                <div className="flex justify-between items-center">
                  <span>Status</span>
                  <Badge className="bg-amber-100 text-amber-800 dark:bg-amber-900">Booked</Badge>
                </div>
                <div className="flex justify-between items-center">
                  <span>Battery</span>
                  <Badge className="bg-green-100 text-green-700 dark:bg-green-900 dark:text-green-200">88%</Badge>
                </div>
                <Button variant="outline" className="w-full">Unlock</Button>
              </div>
            </CardContent>
          </Card>

          <Card className="shadow-sm hover:shadow-md transition-shadow rounded-xl border py-6 shadow-sm hover:shadow-md transition-shadow dark:bg-slate-800/60 backdrop-blur">
            <CardHeader className="p-6 pb-0">
              <CardTitle className="font-medium">Compartment 3</CardTitle>
            </CardHeader>
            <CardContent className="p-6">
              <div className="space-y-4">
                <div className="flex justify-between items-center">
                  <span>Status</span>
                  <Badge className="bg-red-100 text-red-700 dark:bg-red-900 dark:text-red-200">Overdue</Badge>
                </div>
                <div className="flex justify-between items-center">
                  <span>Battery</span>
                  <Badge className="bg-amber-100 text-amber-800 dark:bg-amber-900">72%</Badge>
                </div>
                <Button variant="destructive" className="w-full">Force Unlock</Button>
              </div>
            </CardContent>
          </Card>
        </div>
      </div>
    </div>
  );
}
