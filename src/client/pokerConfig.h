#pragma once

#define BUTTON_BACKGROUND                                                      \
  CLITERAL(Color) { 10, 10, 10, 255 }
#define BUTTON_BACKGROUND_HOVER                                                \
  CLITERAL(Color) { 50, 10, 10, 255 }
#define BUTTON_BACKGROUND_BORDER                                               \
  CLITERAL(Color) { 100, 100, 100, 255 }
#define BUTTON_BACKGROUND_BORDER_HOVER                                         \
  CLITERAL(Color) { 100, 40, 40, 255 }
#define BUTTON_TEXT                                                            \
  CLITERAL(Color) { 200, 200, 200, 255 }
#define BUTTON_BORDER 2
#define BUTTON_MARGIN_TOP 5

#define TEXTBOX_BACKGROUND                                                     \
  CLITERAL(Color) { 10, 10, 10, 255 }
#define TEXTBOX_TEXT                                                           \
  CLITERAL(Color) { 200, 200, 200, 255 }
#define TEXTBOX_TEXT_BORDER                                                    \
  CLITERAL(Color) { 255, 255, 255, 255 }
#define TEXTBOX_TEXT_BORDER_HOVER                                              \
  CLITERAL(Color) { 255, 255, 255, 255 }
#define TEXTBOX_BORDER_WIDTH 2

#define TEXT_COLOR                                                             \
  CLITERAL(Color) { 128, 0, 255, 255 }

#define TEXT_SIZE 30
#define TITLE_SIZE 20
#define BUTTON_TEXT_SIZE TITLE_SIZE

#define WINDOW_TITLE "Poker"
#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720

#define CARD_WIDTH 1024
#define CARD_HEIGHT 1228
#define CARD_HIGHLIGHT                                                         \
  CLITERAL(Color) { 255, 0, 255, 255 }
#define CARD_HIGHLIGHT_THICKNESS 4
#define CARD_DISCARD                                                           \
  CLITERAL(Color) { 255, 0, 0, 255 }
#define CARD_DISCARD_THICKNESS 10