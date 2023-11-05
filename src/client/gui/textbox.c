
#include "textbox.h"
#include "../pokerConfig.h"
#include "button.h"
#include "draggable.h"
#include "gui.h"
#include "raylib.h"


#include <stdlib.h>
#include <string.h>

TextBox *create_TextBox(Rectangle backgroundRect, char *title, char *text,
                        int textMaxLength, int draggable) {
  TextBox *ret = malloc(sizeof(TextBox));
  ret->backgroundRect = backgroundRect;
  strncpy(ret->title, title, TEXT_BOX_TITLE_LENGTH + 1);
  ret->title[TEXT_BOX_TITLE_LENGTH] = 0;
  ret->textMaxLength = textMaxLength;
  ret->textLength = strlen(text);
  ret->text = text;
  ret->draggable = draggable;
  Rectangle buttonRect;
  buttonRect.width = backgroundRect.width / 2 - 30;
  buttonRect.x = 10;
  buttonRect.y = backgroundRect.height - 40;
  buttonRect.height = 30;
  ret->submit = create_Button(buttonRect, &ret->backgroundRect, "Submit");

  buttonRect.x = backgroundRect.width - buttonRect.width - 10;
  ret->cancel = create_Button(buttonRect, &ret->backgroundRect, "Cancel");

  ret->textRect.width = backgroundRect.width - 20;
  ret->textRect.height = backgroundRect.height - 85;
  ret->textRect.x = 10;
  ret->textRect.y = 30;

  int textWidth = MeasureText(title, TITLE_SIZE);
  ret->titleOffset = (backgroundRect.width - textWidth) / 2;

  if (draggable)
    memset(&ret->drag, 0, sizeof(ret->drag));

  return ret;
}
void destroy_TextBox(TextBox **textBox) {
  if (textBox == NULL)
    return;
  free((*textBox)->submit);
  free((*textBox)->cancel);
  free(*textBox);
  *textBox = NULL;
}
#pragma warning(push)
#pragma warning(disable : 4244)
enum textbox_button update_TextBox(TextBox *textBox) {
  if (textBox == NULL)
    return NONE;

  textBox->hovering =
      CheckCollisionPointRec(GetMousePosition(), Scaled(textBox->textRect));
  if (textBox->hovering && (!textBox->draggable || !textBox->drag.dragging)) {
    SetMouseCursor(MOUSE_CURSOR_IBEAM);
    int key = GetCharPressed();
    while (key > 0) {
      if ((key >= 32) && (key <= 125) &&
          (textBox->textLength < textBox->textMaxLength)) {
        textBox->text[textBox->textLength] = (char)key;
        textBox->text[textBox->textLength + 1] =
            '\0'; // Add null terminator at the end of the string.
        textBox->textLength++;
      }
      key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
      textBox->textLength--;
      if (textBox->textLength < 0)
        textBox->textLength = 0;
      textBox->text[textBox->textLength] = '\0';
    }
    return NONE;
  }
  SetMouseCursor(MOUSE_CURSOR_DEFAULT);

  if (textBox->draggable)
    update_Draggable(&textBox->drag, &textBox->backgroundRect);

  textBox->textRect.x = textBox->backgroundRect.x + 10;
  textBox->textRect.y = textBox->backgroundRect.y + 35;
  if (update_Button(textBox->submit))
    return SUBMIT;
  if (update_Button(textBox->cancel))
    return CANCEL;

  return NONE;
}
void draw_TextBox(TextBox *textBox) {
  if (textBox == NULL)
    return;
  float scale = getScaler();
  DrawRectangleRec(Scaled(textBox->backgroundRect), TEXTBOX_BACKGROUND);
  if (TEXTBOX_BORDER_WIDTH)
    DrawRectangleLinesEx(
        Scaled(textBox->textRect), TEXTBOX_BORDER_WIDTH * scale,
        (textBox->hovering ? TEXTBOX_TEXT_BORDER_HOVER : TEXTBOX_TEXT_BORDER));

  DrawText(textBox->text, (textBox->backgroundRect.x + 15) * scale,
           (textBox->backgroundRect.y + 40) * scale, TEXT_SIZE * scale,
           TEXTBOX_TEXT);
  DrawText(textBox->title,
           (textBox->backgroundRect.x + textBox->titleOffset) * scale,
           (textBox->backgroundRect.y + 10) * scale, TITLE_SIZE * scale,
           TEXTBOX_TEXT);

  draw_Button(textBox->submit);
  draw_Button(textBox->cancel);
}
#pragma warning(pop)