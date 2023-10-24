#ifdef SERVER_BUILD
#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include "clientWebsocket.h"
#include <stdio.h>
#include <string.h>
#include "../vector/vector.h"

#define WEBSOCKET_BUFF_LEN 2048
#define NAME_LEN 64
char *recbuff = NULL;
Vector clients;
SOCKET serverSocket = INVALID_SOCKET;
SOCKET clientSocket = INVALID_SOCKET;

struct sockaddr_in servAddr;
struct sockaddr_in clientAddr;
socklen_t clientAddrLen;

// for select()
fd_set activeFdSet;
fd_set readFdSet;

void close_socket(int fd);

int socketErrorCheck(int returnValue, SOCKET socketToClose, const char *action);

int websocketInitialize()
{
    int ret = 0;

#ifdef _WIN32
    WSADATA wsaData;
    if ((ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
    {
        fprintf(stderr, "WSAStartup failed with error %d\n", WSAGetLastError());
        exit(ret);
    }
    printf("Winsock found!\n"
           "Current status is: %s.\n",
           wsaData.szSystemStatus);
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        printf("This app doesn't support %u.%u!", LOBYTE(wsaData.wVersion),
               HIBYTE(wsaData.wVersion));
        websocketCleanup();
        exit(ret);
    }
    printf("The dll supports the Winsock version %u.%u!\n",
           LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));

    printf("The highest version this dll can support: %u.%u\n",
           LOBYTE(wsaData.wHighVersion), HIBYTE(wsaData.wHighVersion));

    setvbuf(stdout, NULL, _IONBF, 0);
#endif

    recbuff = malloc(WEBSOCKET_BUFF_LEN * sizeof(char));
    return ret;
}
int websocketCleanup()
{
    int ret = 0;
#ifdef _WIN32
    if ((ret = WSACleanup()) == SOCKET_ERROR)
    {
        fprintf(stderr, "WSACleanup failed with error %d\n", WSAGetLastError());
    }
#endif
    free(recbuff);
    return ret;
}
void createWebsocket(int port)
{
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        printf("socket failed\n");
        websocketCleanup();
        exit(1);
    }

    int iRes;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);

    iRes = bind(serverSocket, (struct sockaddr *)&servAddr, sizeof(servAddr));
    socketErrorCheck(iRes, serverSocket, "bind", 1);

    iRes = listen(serverSocket, SOMAXCONN);
    socketErrorCheck(iRes, serverSocket, "listen", 1);

    FD_ZERO(&activeFdSet);
    FD_SET(serverSocket, &activeFdSet);
}
void sendAll(char *data, SOCKET exclude)
{
    size_t buffsize = strlen(data);

    for (int j = 0; j < clients.length; j++)
    {
        SOCKET sock = *(SOCKET *)Vector_At(&clients, j);
        if (sock == serverSocket || sock == exclude)
            continue;
        int iRes = send(sock, data, buffsize, 0);
        if (socketErrorCheck(iRes, sock, "send", 0))
        {
            Vector_RemoveAt(&clients, j, 0);
        }
    }
}
void createConnection()
{
    clientAddrLen = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientSocket == INVALID_SOCKET)
    {
        printf("accept failed with error: %d\n", WSAGetLastError());
        close_socket(clientSocket);
        return;
    }
    printf("Got connection with uid: %u\n", clientSocket);
    FD_SET(clientSocket, &activeFdSet);

    Vector_PushBack(&clients, &clientSocket);
}
void clientDisconnect(SOCKET currSocket, int index)
{
    printf("connection with socket &d closing\n", currSocket);
    int iRes = shutdown(currSocket, SD_SEND);
    if (!socketErrorCheck(iRes, currSocket, "shutdown", 0))
        close_socket(currSocket);

    FD_CLR(currSocket, &activeFdSet);
    Vector_RemoveAt(&clients, index, 0);
    sprintf(recbuff, "USERLEFT;%u\n", currSocket);
    sendAll(recbuff, -1);
}
void dataReceived(SOCKET currSocket, int index, char *type, char *value)
{
    int iRes;
    if (strcmp(type, "ping") == 0)
    {
        iRes = send(currSocket, "pong\n", 5, 0);
        socketErrorCheck(iRes, currSocket, "send", 0);
        return;
    }
    if (strcmp(type, "join") == 0)
    {

        sprintf_s(recbuff, 7 + NAME_LEN, "join;%s\n", value);
        int slen = strlen(recbuff);
        for (int j = 0; j < clients.length; j++)
        {
            SOCKET sock = *(SOCKET *)Vector_At(&clients, j);
            if (serverSocket == sock || currSocket == sock)
                continue;
            iRes = send(sock, recbuff, slen, 0);
            if (socketErrorCheck(iRes, sock, "send", 0))
            {
                Vector_RemoveAt(&clients, j--, 0);
            }
        }
        return;
    }
}
void websocketStart(int port)
{

    int iRes;
    createWebsocket(port);

    clients = Vector_Create(sizeof(SOCKET));
    Vector_PushBack(&clients, &serverSocket);

    printf("Server started on port %d\n"
           "Waiting for connections...\n",
           port);

    while (1)
    {
        readFdSet = activeFdSet;
        iRes = select(FD_SETSIZE, &readFdSet, NULL, NULL, NULL);
        socketErrorCheck(iRes, serverSocket, "select", 1);

        SOCKET currSocket;
        int i;
        for (i = 0; i < (int)clients.length; i++)
        {
            currSocket = *(SOCKET *)Vector_At(&clients, i);
            if (FD_ISSET(currSocket, &readFdSet))
                break;
        }
        if (currSocket == serverSocket) // new connection
        {
            createConnection();
            continue;
        }

        memset(recbuff, 0, WEBSOCKET_BUFF_LEN); // zero buffer
        iRes = recv(currSocket, recbuff, WEBSOCKET_BUFF_LEN, 0);

        if (iRes < 0)
        {
            printf("recv failed: %d\n", WSAGetLastError());
            close_socket(currSocket);
            FD_CLR(currSocket, &activeFdSet);
            Vector_RemoveAt(&clients, i, 0);
            continue;
        }
        if (iRes == 0)
        {
            clientDisconnect(currSocket, i);
            continue;
        }

        recbuff[strcspn(recbuff, "\r\n")] = 0; // remove trailing new line
        printf("received data: %s\n", recbuff);

        char *type, *value;
        type = recbuff;
        value = strchr(type, ';');
        if (NULL != value)
        {
            *value = 0;
            value++;
        }
        dataReceived(currSocket, i, type, value);
    }
}

int socketErrorCheck(int returnValue, SOCKET socketToClose, const char *action, int critical)
{
    const char *actionAttempted = action;
    if (returnValue != SOCKET_ERROR)
        return 0;

    printf("socket error. %s failed with error: %d\n", actionAttempted, WSAGetLastError());
    close_socket(socketToClose);
    if (!critical)
        return 1;
#if _WIN32
    WSACleanup();
#endif
    exit(1);
}
void close_socket(int fd)
{
#ifndef _WIN32
    shutdown(fd, SHUT_RDWR);
    close(fd);
#else
    closesocket(fd);
#endif
}

#endif