#ifndef NULL_CLIENT_WEBSOCKET
#define NULL_CLIENT_WEBSOCKET

int websocketInitialize();
int websocketCleanup();
int websocketConnect(char* address, char* port);
void websocketSend(char* data);
void registerDataReceived(void (*callback)(char*,char*));

#endif
