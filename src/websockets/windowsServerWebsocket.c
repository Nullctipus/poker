#ifdef SERVER_BUILD
#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include "clientWebsocket.h"
#include <windows.h>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include "../vector/vector.h"

WSADATA wsadata;

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

int socketErrorCheck(int returnValue, SOCKET socketToClose, const char *action);

int websocketInitialize()
{
    int ret;
    if ((ret = WSAStartup(MAKEWORD(2, 2), &wsadata)) != 0)
    {
        fprintf(stderr, "WSAStartup failed with error %d\n", WSAGetLastError());
        exit(ret);
    }
    printf("Winsock found!\n"
           "Current status is: %s.\n",
           wsadata.szSystemStatus);
    if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2)
    {
        printf("This app doesn't support %u.%u!", LOBYTE(wsadata.wVersion),
               HIBYTE(wsadata.wVersion));
        websocketCleanup();
        exit(ret);
    }
    printf("The dll supports the Winsock version %u.%u!\n",
           LOBYTE(wsadata.wVersion), HIBYTE(wsadata.wVersion));

    printf("The highest version this dll can support: %u.%u\n",
           LOBYTE(wsadata.wHighVersion), HIBYTE(wsadata.wHighVersion));
    recbuff = malloc(WEBSOCKET_BUFF_LEN * sizeof(char));
    return ret;
}
int websocketCleanup()
{
    int ret;
    if ((ret = WSACleanup()) == SOCKET_ERROR)
    {
        fprintf(stderr, "WSACleanup failed with error %d\n", WSAGetLastError());
    }
    free(recbuff);
    return ret;
}
void websocketStart(int port)
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
            clientAddrLen = sizeof(clientAddr);
            clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
            if (clientSocket == INVALID_SOCKET)
            {
                printf("accept failed with error: %d\n", WSAGetLastError());
                closesocket(clientSocket);
                continue;
            }
            printf("Got connection with uid: %u\n", clientSocket);
            FD_SET(clientSocket, &activeFdSet);

            Vector_PushBack(&clients, &clientSocket);
            continue;
        }

        memset(recbuff, 0, WEBSOCKET_BUFF_LEN); // zero buffer
        iRes = recv(currSocket, recbuff, WEBSOCKET_BUFF_LEN, 0);

        if (iRes < 0)
        {
            printf("recv failed: %d\n", WSAGetLastError());
            closesocket(currSocket);
            FD_CLR(currSocket, &activeFdSet);
            Vector_RemoveAt(&clients, i, 0);
            continue;
        }
        if (iRes == 0)
        {
            printf("connection with socket &d closing\n", currSocket);
            iRes = shutdown(currSocket, SD_SEND);
            if (!socketErrorCheck(iRes, currSocket, "shutdown", 0))
                closesocket(currSocket);

            FD_CLR(currSocket, &activeFdSet);
            Vector_RemoveAt(&clients, i, 0);
            sprintf(recbuff, "USERLEFT;%u\n", currSocket);
            size_t buffsize = strlen(recbuff);

            for (int j = 0; j < clients.length; j++)
            {
                SOCKET sock = *(SOCKET *)Vector_At(&clients, j);
                if (serverSocket == sock)
                    continue;
                iRes = send(sock, recbuff, buffsize, 0);
                if (socketErrorCheck(iRes, sock, "send", 0))
                {
                    Vector_RemoveAt(&clients, j, 0);
                }
            }
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
        if (strcmp(type, "ping") == 0)
        {
            iRes = send(currSocket, "pong\n", 5, 0);
            socketErrorCheck(iRes, currSocket, "send", 0);
            continue;
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
            continue;
        }
    }
}

int socketErrorCheck(int returnValue, SOCKET socketToClose, const char *action, int critical)
{
    const char *actionAttempted = action;
    if (returnValue != SOCKET_ERROR)
        return 0;

    printf("socket error. %s failed with error: %d\n", actionAttempted, WSAGetLastError());
    closesocket(socketToClose);
    if (!critical)
        return 1;
    WSACleanup();
    exit(1);
}

#endif
#endif