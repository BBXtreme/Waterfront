// webui_server.h - Header file for web server functions
// This header declares functions for managing the HTTP server in SoftAP mode.
// It provides an interface for starting/stopping the server for WiFi provisioning.
// Used in conjunction with webui_server.cpp for web-based configuration.

#ifndef WEBUI_SERVER_H
#define WEBUI_SERVER_H

// Start SoftAP mode with provisioning SSID
void start_softap();

// Start the REST server for handling HTTP requests
void start_rest_server();

// Stop the REST server
void stop_rest_server();

#endif // WEBUI_SERVER_H
