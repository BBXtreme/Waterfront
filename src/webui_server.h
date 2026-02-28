// Adapted from original webui_server.h
#ifndef WEBUI_SERVER_H
#define WEBUI_SERVER_H

#include <WebServer.h>
#include <DNSServer.h>

extern WebServer server;
extern DNSServer dnsServer;

void start_softap();
void start_rest_server();
void stop_rest_server();
void start_dns_server();
void stop_dns_server();

#endif // WEBUI_SERVER_H
