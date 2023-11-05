#ifndef PLATFORM_WEB
#include "../common/vector/queue.h"
#include "clientWebsocket.h"
#include "core_basic_window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>


typedef unsigned long long SOCKET;

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#endif

#define WEBSOCKET_BUFF_LEN 2048
#define NAME_LEN 64

char *recbuff = NULL;
SOCKET serverSocket = INVALID_SOCKET;
SOCKET clientSocket = INVALID_SOCKET;

struct addrinfo hints;
struct addrinfo *servinfo;

struct sockaddr_in servAddr;
struct sockaddr_in clientAddr;
socklen_t clientAddrLen;

// for select()
fd_set activeFdSet;
fd_set readFdSet;

Queue sendQueue;
int socketErrorCheck(int returnValue, SOCKET socketToClose, const char *action,
                     int critical);
void (*dataReceived)(char *, char *);
void registerDataReceived(void (*callback)(char *, char *)) {
  dataReceived = callback;
}
void close_socket(SOCKET fd);
int websocketInitialize() {
  sendQueue = Queue_Create(sizeof(char) * WEBSOCKET_BUFF_LEN);
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

#if !_WIN32
int msleep(long msec) {
  struct timespec ts;
  int res;

  ts.tv_sec = msec / 1000;
  ts.tv_nsec = (msec % 1000) * 1000000;

  do {
    res = nanosleep(&ts, &ts);
  } while (res);

  return res;
}
#endif
void websocketSend(char *data) { Queue_Enqueue(&sendQueue, data); }

#if _WIN32
DWORD WINAPI
#else
void *
#endif
sendLoop(void *data) {
  int iRes;
  while (1) {
    if (!sendQueue.base_vector.length) {
#if _WIN32
      Sleep(100);
#else
      msleep(100);
#endif
      continue;
    }

    char *data = Queue_Next(&sendQueue);

    iRes = send(serverSocket, data, strlen(data), 0);
    if (socketErrorCheck(iRes, serverSocket, "send", 1))
      break;
  }

  close_socket(serverSocket);
#if _WIN32
  return 0;
#else
  pthread_exit(NULL);
#endif
}

#if _WIN32
DWORD WINAPI
#else
void *
#endif
receiveLoop(void *data) {
  int iRes;
  while (1) {

    iRes = recv(serverSocket, recbuff, WEBSOCKET_BUFF_LEN, 0);
    if (socketErrorCheck(iRes, serverSocket, "recv", 1))
      break;
    recbuff[strcspn(recbuff, "\r\n")] = 0;
    printf("receive %s\n", recbuff);
    char *type, *value;
    type = recbuff;
    value = strchr(type, ';');
    if (NULL != value) {
      *value = 0;
      value++;
    }
    dataReceived(type, value);
  }

  close_socket(serverSocket);
#if _WIN32
  return 0;
#else
  pthread_exit(NULL);
#endif
}
int websocketConnect(char *address, char *port) {

  int status;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  if ((status = getaddrinfo(address, port, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    InitializeConnection();
    return status;
  }

  serverSocket =
      socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
  if (socketErrorCheck(serverSocket, serverSocket, "socket", 1))
    return 1;

  struct addrinfo *p;
  // loop through all the results and connect to the first we can
  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((serverSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) ==
        -1) {
      perror("client: socket");
      continue;
    }

    if (connect(serverSocket, p->ai_addr, p->ai_addrlen) == -1) {
      close_socket(serverSocket);
      perror("client: connect");
      continue;
    }

    break;
  }

  freeaddrinfo(servinfo);
  if (p == NULL) {
    fprintf(stderr, "client: failed to connect\n");
    InitializeConnection();
    return 1;
  }
  status = send(serverSocket, "ping", 5, 0);
  if (socketErrorCheck(status, serverSocket, "send", 1))
    return 1;
  status = recv(serverSocket, recbuff, WEBSOCKET_BUFF_LEN, 0);
  if (socketErrorCheck(status, serverSocket, "recv", 1))
    return 1;
  recbuff[strcspn(recbuff, "\r\n")] = 0;
  printf("receive %s\n", recbuff);
  if (strcmp(recbuff, "pong"))
    printf("Server is likely not running poker server. Trying anyways...\n");

#if _WIN32
  HANDLE thread[2];
  thread[0] = CreateThread(NULL, 0, sendLoop, NULL, 0, NULL);
  thread[1] = CreateThread(NULL, 0, receiveLoop, NULL, 0, NULL);
#else
  pthread_t thread[2];
  pthread_create(&thread[0], NULL, sendLoop, NULL);
  pthread_create(&thread[1], NULL, receiveLoop, NULL);
#endif

  return 0;
}

int socketErrorCheck(int returnValue, SOCKET socketToClose, const char *action,
                     int critical) {
  const char *actionAttempted = action;
  if (returnValue != SOCKET_ERROR)
    return 0;
#ifdef _WIN32
  printf("socket error. %s failed with error: %d\n", actionAttempted,
         WSAGetLastError());
#endif
  close_socket(socketToClose);
  if (critical)
    InitializeConnection();
  return 1;
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
