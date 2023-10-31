#pragma once
void InitServer();

void ProcessIncoming(unsigned long long socket, char *type,
                     char *value);
void ProcessDisconnect(unsigned long long socket);