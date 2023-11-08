#pragma once
#include "raylib.h"
typedef struct{
    char* text;
    int textLength;
    int textMaxLength;
    int hovering;
    Rectangle rect;
} MiniTextBox;

MiniTextBox *createMiniTextBox(char* text, int textMaxLength,Rectangle rect);
void UpdateMiniTextBox(MiniTextBox *textBox);
void DrawMiniTextBox(MiniTextBox *textBox);