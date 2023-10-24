#include <stdlib.h>
#ifdef SERVER_BUILD
#include "websockets/serverWebsocket.h"
#else
#include "core_basic_window.h"
#include "websockets/clientWebsocket.h"
#endif
int main(int argc, char const *argv[]) {
#ifdef SERVER_BUILD
  int port = DEFAULT_PORT;
  if(argc >= 2)
    port = atoi(argv[1]);
  
  websocketInitialize();
  websocketStart(port);
  websocketCleanup();
#else
  websocketInitialize();
  createWindow();
  websocketCleanup();
/* code */
#endif
  return 0;
}
