#pragma once

#include "button.h"
#include "draggable.h"
#include "raylib.h"

#define TEXT_BOX_TITLE_LENGTH 64
enum textbox_button{
    NONE = 0,
    SUBMIT = 1,
    CANCEL = 2,
};

typedef struct textbox_struct
{
    Rectangle backgroundRect;
    Rectangle textRect;
    int draggable;
    char title[TEXT_BOX_TITLE_LENGTH+1];
    int titleOffset;
    char* text;
    int textMaxLength;
    int textLength;
    Button *submit;
    Button *cancel;

    int hovering;
    Draggable drag;
} TextBox;

TextBox *create_TextBox(Rectangle backgroundRect,char* title,char* text,int textMaxLength,int draggable);
void destroy_TextBox(TextBox **textBox);
enum textbox_button update_TextBox(TextBox *textBox);
void draw_TextBox(TextBox *textBox);
