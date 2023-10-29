#include "raylib.h"
#if defined(PLATFORM_WEB)
#include <emscripten/websocket.h>

int websocketInitialize(){
    
    return 0;
}
int websocketCleanup(){
    
    return 0;
}
int websocketConnect(char* address, int port){
    
    return 0;
}

#endif
