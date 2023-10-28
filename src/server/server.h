#ifdef SERVER_BUILD
#ifndef SERVER_H
#define SERVER_H

void InitServer();

void ProcessIncoming(unsigned long long socket, char *game, char *type,
                     char *value);

#endif
#endif
