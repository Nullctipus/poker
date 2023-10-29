#include "draggable.h"
#include "gui.h"
#include "raylib.h"

void update_Draggable(Draggable* draggable, Rectangle *toDrag){
    float scale = getScaler();
    Vector2 mousepos = GetMousePosition();
    mousepos.x/=scale;
    mousepos.y/=scale;
    int shouldDrag = CheckCollisionPointRec(mousepos, *toDrag) && IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    if(draggable->dragging)
        shouldDrag = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    if(draggable->dragging && !shouldDrag ){
        draggable->dragging = 0;
        draggable->offsetX = 0;
        draggable->offsetY = 0;
        return;
    }
    if(draggable->dragging && shouldDrag){
        toDrag->x = mousepos.x-draggable->offsetX;
        toDrag->y = mousepos.y-draggable->offsetY;
    }
    if(!draggable->dragging && shouldDrag){
        draggable->dragging = 1;
        draggable->offsetX = mousepos.x-toDrag->x;
        draggable->offsetY = mousepos.y-toDrag->y;
    }
}