#include "raylib.h"
#if defined(PLATFORM_WEB)
#include "clientWebsocket.h"
#include "core_basic_window.h"
#include <emscripten/emscripten.h>
#include <emscripten/websocket.h>
#include <stdio.h>

#define WEBSOCKET_BUFF_LEN 2048
#define NAME_LEN 64

char *recbuff = NULL;
void (*dataReceived)(char *, char *);
EMSCRIPTEN_WEBSOCKET_T server;

EM_BOOL onopen(int eventType,
               const EmscriptenWebSocketOpenEvent *websocketEvent,
               void *userData) {
  server = websocketEvent->socket;
  websocketSend("ping");
  return EM_TRUE;
}
EM_BOOL onerror(int eventType,
                const EmscriptenWebSocketErrorEvent *websocketEvent,
                void *userData) {
  puts("onerror");

  return EM_TRUE;
}
EM_BOOL onclose(int eventType,
                const EmscriptenWebSocketCloseEvent *websocketEvent,
                void *userData) {
  printf("Close reason: %s\n", websocketEvent->reason);
  InitializeConnection();
  return EM_TRUE;
}
int firstMessage = 1;
EM_BOOL onmessage(int eventType,
                  const EmscriptenWebSocketMessageEvent *websocketEvent,
                  void *userData) {
  char *data = (char *)websocketEvent->data;

  data[strcspn(data, "\r\n")] = 0;
  printf("message: %s\n", data);
  if (firstMessage) {
    firstMessage = 0;

    if (strcmp(data, "pong"))
      printf("Server is likely not running poker server. Trying anyways...\n");

    return EM_TRUE;
  }

  char *type, *value;
  type = data;
  value = strchr(type, ';');
  if (NULL != value) {
    *value = 0;
    value++;
  }
  dataReceived(type, value);

  return EM_TRUE;
}

int websocketInitialize() {
  recbuff = malloc(WEBSOCKET_BUFF_LEN * sizeof(char));
  if (!emscripten_websocket_is_supported()) {
    return 1;
  }
  return 0;
}
int websocketCleanup() {
  emscripten_websocket_deinitialize();
  return 0;
}
int websocketConnect(char *address, char *port) {
  sprintf(recbuff, "ws://%s:%s", address, port);
  EmscriptenWebSocketCreateAttributes ws_attrs = {recbuff, NULL, EM_TRUE};

  EMSCRIPTEN_WEBSOCKET_T ws = emscripten_websocket_new(&ws_attrs);
  emscripten_websocket_set_onopen_callback(ws, NULL, onopen);
  emscripten_websocket_set_onerror_callback(ws, NULL, onerror);
  emscripten_websocket_set_onclose_callback(ws, NULL, onclose);
  emscripten_websocket_set_onmessage_callback(ws, NULL, onmessage);
  return 0;
}
void websocketSend(const char *data) {

  EMSCRIPTEN_RESULT result;
  result = emscripten_websocket_send_binary(server, (void *)data,
                                            strlen(data) * sizeof(char));
  if (result) {
    printf("Failed to emscripten_websocket_send_binary(): %d\n", result);
  }
}
void registerDataReceived(void (*callback)(char *, char *)) {
  dataReceived = callback;
}

#endif
