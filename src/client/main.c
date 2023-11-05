#include "client.h"
#include "clientWebsocket.h"
#include "core_basic_window.h"
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  websocketInitialize();
  registerDataReceived(&ProcessData);
  createWindow();
  websocketCleanup();
  /* code */
  return 0;
}
