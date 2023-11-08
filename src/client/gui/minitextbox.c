#include "minitextbox.h"
#include "../pokerConfig.h"
#include "gui.h"
#include "raylib.h"
#include <stdlib.h>
#include <string.h>

MiniTextBox *createMiniTextBox(char *text, int textMaxLength, Rectangle rect) {
  MiniTextBox *ret = malloc(sizeof(MiniTextBox));
  ret->text = text;
  ret->textMaxLength = textMaxLength;
  ret->textLength = (int)strlen(text);
  ret->rect = rect;
  return ret;
}
void UpdateMiniTextBox(MiniTextBox *textBox) {
  textBox->hovering =
      CheckCollisionPointRec(GetMousePosition(), Scaled(textBox->rect));
  if (textBox->hovering) {
    SetMouseCursor(MOUSE_CURSOR_IBEAM);
    int key = GetCharPressed();
    while (key > 0) {
      if ((key >= 32) && (key <= 125) && key != ',' &&
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
    return;
  }
  SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}
void DrawMiniTextBox(MiniTextBox *textBox) {
  float scale = getScaler();
  DrawRectangleRec(Scaled(textBox->rect), TEXTBOX_BACKGROUND);
  if (TEXTBOX_BORDER_WIDTH > 0)
    DrawRectangleLinesEx(
        Scaled(textBox->rect), (float)TEXTBOX_BORDER_WIDTH * scale,
        (textBox->hovering ? TEXTBOX_TEXT_BORDER_HOVER : TEXTBOX_TEXT_BORDER));

  DrawText(textBox->text, (int)((textBox->rect.x + 15) * scale),
           (int)((textBox->rect.y + 40) * scale), (int)(TEXT_SIZE * scale),
           TEXTBOX_TEXT);
}