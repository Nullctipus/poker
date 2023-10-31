#pragma once

#define DEFAULT_PORT 27015
void sendAll(char *data, unsigned long long exclude);
void sendSingle(unsigned long long user, char *data);
void registerCallback(void (*callback)(unsigned long long, char *,
                                       char *));

void registerDisconnect(void (*callback)(unsigned long long));
int websocketInitialize();
int websocketCleanup();

void websocketStart(int port);
