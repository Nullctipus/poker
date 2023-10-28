#ifdef SERVER_BUILD
#ifndef NULL_SERVER_WEBSOCKET
#define NULL_SERVER_WEBSOCKET

#define DEFAULT_PORT 27015
void sendAll(char *data, unsigned long long exclude);
void sendSingle(unsigned long long user, char *data);
void registerCallback(void (*callback)(unsigned long long, char *, char *,
                                       char *));
int websocketInitialize();
int websocketCleanup();

void websocketStart(int port);

#endif
#endif
