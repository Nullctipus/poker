#include "button.h"
#include "gui.h"
#include "raylib.h"
#include "../pokerConfig.h"
#include <stdlib.h>

Button *create_Button(Rectangle rect, Rectangle *parent, char *text)
{
    Button *ret = malloc(sizeof(Button));
    ret->local = rect;
    ret->parent = parent;
    ret->rect = rect;
    ret->text = text;
    ret->hovered = 0;
    return ret;
}

#pragma warning(push)
#pragma warning(disable : 4244)
int update_Button(Button *button)
{
    if(button == NULL) return 0;

    if (button->parent != NULL)
    {
        button->rect.x = button->local.x + button->parent->x;
        button->rect.y = button->local.y + button->parent->y;
    }
    Vector2 mpos = GetMousePosition();
    float scale = getScaler();
    mpos.x/=scale;
    mpos.y/=scale;
    button->hovered = CheckCollisionPointRec(mpos, button->rect);

    return button->hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void draw_Button(Button *button)
{
    if(button == NULL) return;

    float scale = getScaler();
    Rectangle scaled = Scaled(button->rect);
    DrawRectangleRec(scaled, (button->hovered ? BUTTON_BACKGROUND_HOVER : BUTTON_BACKGROUND));
#if BUTTON_BORDER != 0
    DrawRectangleLinesEx(scaled, BUTTON_BORDER, (button->hovered ? BUTTON_BACKGROUND_BORDER_HOVER : BUTTON_BACKGROUND_BORDER));
#endif
    DrawText(button->text,scale* (button->rect.x + BUTTON_MARGIN_LEFT), scale * (button->rect.y + BUTTON_MARGIN_TOP), -1, BUTTON_TEXT);
}
#pragma warning(pop)