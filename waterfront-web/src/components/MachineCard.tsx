// src/components/MachineCard.tsx
import { cn } from "@/lib/utils";  // shadcn cn helper

interface MachineCardProps {
  status?: string;
}

export default function MachineCard({ status = "disconnected" }: MachineCardProps) {
  return (
    <div className="p-6 bg-card rounded-lg shadow-sm border">
      <div className="flex items-center gap-2">
        <span>Status:</span>
        <span
          className={cn(
  "px-2 py-1 rounded-full text-xs font-medium",
  status === "connected"
    ? "bg-green-50 text-green-700 dark:bg-green-950 dark:text-green-300"  // ← changed from 600 to 700
    : "bg-red-50 text-red-700 dark:bg-red-950 dark:text-red-300"
)}
        >
          {status}
        </span>
      </div>
    </div>
  );
}