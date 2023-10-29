#pragma once
void InitServer();

void ProcessIncoming(unsigned long long socket, char *game, char *type,
                     char *value);
