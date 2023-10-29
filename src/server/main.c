#include "server.h"
#include "serverWebsocket.h"
#include <stdlib.h>
int main(int argc, char const *argv[]) {
  int port = DEFAULT_PORT;
  if (argc >= 2)
    port = atoi(argv[1]);

  websocketInitialize();
  InitServer();
  registerCallback(&ProcessIncoming);
  websocketStart(port);
  websocketCleanup();
  return 0;
}
