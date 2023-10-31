#pragma once

#include "raylib.h"

typedef struct button_struct {
    Rectangle *parent;
    Rectangle local;
    char* text;
    int textOffset;

    int hovered;
    Rectangle rect;
} Button;

Button *create_Button(Rectangle rect, Rectangle *parent,char* text);
int update_Button(Button *button);
void draw_Button(Button *button);
