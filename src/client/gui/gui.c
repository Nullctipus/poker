#include "gui.h"
#include "raylib.h"
#include "../pokerConfig.h"


float getScaler()
{
    return GetRenderWidth()/(float)DEFAULT_WINDOW_WIDTH;
}

#pragma warning(push)
#pragma warning(disable : 4244)
Rectangle Scaled(Rectangle base){
    float scale = getScaler();
    base.x*=scale;
    base.y*=scale;
    base.width*=scale;
    base.height*=scale;
    return base;
}
#pragma warning(pop)