#ifndef NULL_CLIENT_WEBSOCKET
#define NULL_CLIENT_WEBSOCKET

int websocketInitialize();
int websocketCleanup();
int websocketConnect(char* address, int port);

#endif
