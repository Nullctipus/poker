#include "gui.h"
#include "../pokerConfig.h"
#include "raylib.h"
#include <stdio.h>

float textWidth = -1;
float titleWidth = -1;
float getTextWidth() {
  if (textWidth == -1) {
    textWidth = (float)MeasureText("A", TEXT_SIZE);
  }

  return textWidth;
}
float getTitleWidth() {
  if (titleWidth == -1) {
    titleWidth = (float)MeasureText("A", TITLE_SIZE);
  }

  return titleWidth;
}

float getScaler() { return GetRenderWidth() / (float)DEFAULT_WINDOW_WIDTH; }

Rectangle Scaled(Rectangle base) {
  float scale = getScaler();
  base.x *= scale;
  base.y *= scale;
  base.width *= scale;
  base.height *= scale;
  return base;
}

int im_button(Rectangle rect, const char *text) {
  Vector2 mpos = GetMousePosition();
  float scale = getScaler();
  mpos.x /= scale;
  mpos.y /= scale;
  int hovered = CheckCollisionPointRec(mpos, rect);

  Rectangle scaled = Scaled(rect);
  DrawRectangleRec(scaled,
                   (hovered ? BUTTON_BACKGROUND_HOVER : BUTTON_BACKGROUND));

#if BUTTON_BORDER != 0
  DrawRectangleLinesEx(
      scaled, BUTTON_BORDER,
      (hovered ? BUTTON_BACKGROUND_BORDER_HOVER : BUTTON_BACKGROUND_BORDER));
#endif
  int textOffset =
      (int)((rect.width - MeasureText(text, BUTTON_TEXT_SIZE)) / 2.0);
  DrawText(text, (int)(scale * (rect.x + textOffset)),
           (int)(scale * (rect.y + (rect.height - BUTTON_TEXT_SIZE) / 2.0)),
           (int)(scale * BUTTON_TEXT_SIZE), BUTTON_TEXT);

  return hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}
void im_label(Rectangle rect, const char *text) {
  int textOffset = (int)((rect.width - MeasureText(text, TEXT_SIZE)) / 2.0);
  float scale = getScaler();

  DrawText(text, (int)(scale * (rect.x + textOffset)),
           (int)(scale * (rect.y + (rect.height - TEXT_SIZE) / 2.0)),
           (int)(scale * TEXT_SIZE), TEXT_COLOR);
}