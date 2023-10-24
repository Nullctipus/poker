#ifndef SERVER_BUILD
#include <stdio.h>
#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include "clientWebsocket.h"
#include <windows.h>
#include <winsock2.h>

WSADATA wsadata;

int websocketInitialize() {
  int ret;
  if ((ret = WSAStartup(MAKEWORD(2, 2), &wsadata)) != 0) {
    fprintf(stderr, "WSAStartup failed with error %d\n", WSAGetLastError());
    return ret;
  }
  printf("Winsock found!\n"
         "Current status is: %s.\n",
         wsadata.szSystemStatus);
  if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2) {
    printf("This app doesn't support %u.%u!", LOBYTE(wsadata.wVersion),
           HIBYTE(wsadata.wVersion));
    websocketCleanup();
    return 1;
  }
  printf("The dll supports the Winsock version %u.%u!\n",
         LOBYTE(wsadata.wVersion), HIBYTE(wsadata.wVersion));

  printf("The highest version this dll can support: %u.%u\n",
         LOBYTE(wsadata.wHighVersion), HIBYTE(wsadata.wHighVersion));
  return ret;
}
int websocketCleanup() {
  int ret;
  if ((ret = WSACleanup()) == SOCKET_ERROR) {
    fprintf(stderr, "WSACleanup failed with error %d\n", WSAGetLastError());
  }
  return ret;
}

#endif
#endif