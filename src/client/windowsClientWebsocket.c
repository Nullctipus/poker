#ifndef PLATFORM_WEB
#include "clientWebsocket.h"
#include <stdio.h>
#include <stdlib.h>
#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#define WEBSOCKET_BUFF_LEN 2048
#define NAME_LEN 64

char *recbuff = NULL;
SOCKET serverSocket = INVALID_SOCKET;
SOCKET clientSocket = INVALID_SOCKET;

struct sockaddr_in servAddr;
struct sockaddr_in clientAddr;
socklen_t clientAddrLen;

// for select()
fd_set activeFdSet;
fd_set readFdSet;
void close_socket(SOCKET fd);
int websocketInitialize() {
  int ret = 0;
#ifdef _WIN32
  WSADATA wsadata;
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
  setvbuf(stdout, NULL, _IONBF, 0);
#endif
  recbuff = malloc(WEBSOCKET_BUFF_LEN * sizeof(char));
  return ret;
}
int websocketCleanup() {
  int ret = 0;
#ifdef _WIN32
  if ((ret = WSACleanup()) == SOCKET_ERROR) {
    fprintf(stderr, "WSACleanup failed with error %d\n", WSAGetLastError());
  }
#endif
  free(recbuff);
  return ret;
}
int websocketConnect(char* address, int port){
  *(volatile char*)0 = 0; // Trap
  return 0;
}

int socketErrorCheck(int returnValue, SOCKET socketToClose, const char *action,
                     int critical) {
  const char *actionAttempted = action;
  if (returnValue != SOCKET_ERROR)
    return 0;

  printf("socket error. %s failed with error: %d\n", actionAttempted,
         WSAGetLastError());
  close_socket(socketToClose);
  if (!critical)
    return 1;
#if _WIN32
  WSACleanup();
#endif
  exit(1);
}
void close_socket(SOCKET fd) {
#ifndef _WIN32
  shutdown(fd, SHUT_RDWR);
  close(fd);
#else
  closesocket(fd);
#endif
}
#endif
