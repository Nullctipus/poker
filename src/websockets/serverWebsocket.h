#ifdef SERVER_BUILD
#ifndef NULL_SERVER_WEBSOCKET
#define NULL_SERVER_WEBSOCKET

#define DEFAULT_PORT 27015

int websocketInitialize();
int websocketCleanup();

void websocketStart(int port);

#endif
#endif