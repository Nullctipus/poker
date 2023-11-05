#pragma once

#include "raylib.h"

typedef struct draggable_struct {
  float offsetX, offsetY;
  int dragging;
} Draggable;

void update_Draggable(Draggable *draggable, Rectangle *toDrag);
