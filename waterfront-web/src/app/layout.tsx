import type { Metadata } from "next";
import { Inter } from "next/font/google";
import "./globals.css";

const inter = Inter({ subsets: ["latin"] });

export const metadata: Metadata = {
  title: "Waterfront – Connection Tests",
  description: "Testing Vercel + Supabase + MQTT",
};

export default function RootLayout({
  children,
}: {
  children: React.ReactNode;
}) {
  return (
    <html lang="de">
      <body className={inter.className + " antialiased min-h-screen bg-gray-50"}>
        {children}
      </body>
    </html>
  );
}