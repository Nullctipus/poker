#include "gui.h"
#include "raylib.h"
#include "../pokerConfig.h"

float textWidth = -1;
float titleWidth = -1;
float getTextWidth()
{
    if (textWidth == -1)
    {
        textWidth = MeasureText("A", TEXT_SIZE);
    }

    return textWidth;
}
float getTitleWidth()
{
    if (titleWidth == -1)
    {
        titleWidth = MeasureText("A", TITLE_SIZE);
    }

    return titleWidth;
}

float getScaler()
{
    return GetRenderWidth() / (float)DEFAULT_WINDOW_WIDTH;
}

#pragma warning(push)
#pragma warning(disable : 4244)
Rectangle Scaled(Rectangle base)
{
    float scale = getScaler();
    base.x *= scale;
    base.y *= scale;
    base.width *= scale;
    base.height *= scale;
    return base;
}
#pragma warning(pop)