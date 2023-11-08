#include "cards.h"
#include "raylib.h"
#include "resources.h"

const unsigned char *cardData[4][13] = {
    {
        diam_2_png,
        diam_3_png,
        diam_4_png,
        diam_5_png,
        diam_6_png,
        diam_7_png,
        diam_8_png,
        diam_9_png,
        diam_10_png,
        diam_jack_png,
        diam_queen_png,
        diam_king_png,
        diam_ace_png,
    },
    {
        club_2_png,
        club_3_png,
        club_4_png,
        club_5_png,
        club_6_png,
        club_7_png,
        club_8_png,
        club_9_png,
        club_10_png,
        club_jack_png,
        club_queen_png,
        club_king_png,
        club_ace_png,
    },
    {
        hear_2_png,
        hear_3_png,
        hear_4_png,
        hear_5_png,
        hear_6_png,
        hear_7_png,
        hear_8_png,
        hear_9_png,
        hear_10_png,
        hear_jack_png,
        hear_queen_png,
        hear_king_png,
        hear_ace_png,
    },
    {
        spad_2_png,
        spad_3_png,
        spad_4_png,
        spad_5_png,
        spad_6_png,
        spad_7_png,
        spad_8_png,
        spad_9_png,
        spad_10_png,
        spad_jack_png,
        spad_queen_png,
        spad_king_png,
        spad_ace_png,
    },
};
unsigned int cardDataLen[4][13] = {-1};

Texture cardTextures[4][13] = {
    {
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
    },
    {
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
    },
    {
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
    },
    {
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
    },
};
Texture GetCardTexture(enum Suit suit, int card) {
  if (cardDataLen[0][0] == -1) {
    cardDataLen[0][0] = diam_2_png_size;
    cardDataLen[0][1] = diam_3_png_size;
    cardDataLen[0][2] = diam_4_png_size;
    cardDataLen[0][3] = diam_5_png_size;
    cardDataLen[0][4] = diam_6_png_size;
    cardDataLen[0][5] = diam_7_png_size;
    cardDataLen[0][6] = diam_8_png_size;
    cardDataLen[0][7] = diam_9_png_size;
    cardDataLen[0][8] = diam_10_png_size;
    cardDataLen[0][9] = diam_jack_png_size;
    cardDataLen[0][10] = diam_queen_png_size;
    cardDataLen[0][11] = diam_king_png_size;
    cardDataLen[0][12] = diam_ace_png_size;
    cardDataLen[1][0] = club_2_png_size;
    cardDataLen[1][1] = club_3_png_size;
    cardDataLen[1][2] = club_4_png_size;
    cardDataLen[1][3] = club_5_png_size;
    cardDataLen[1][4] = club_6_png_size;
    cardDataLen[1][5] = club_7_png_size;
    cardDataLen[1][6] = club_8_png_size;
    cardDataLen[1][7] = club_9_png_size;
    cardDataLen[1][8] = club_10_png_size;
    cardDataLen[1][9] = club_jack_png_size;
    cardDataLen[1][10] = club_queen_png_size;
    cardDataLen[1][11] = club_king_png_size;
    cardDataLen[1][12] = club_ace_png_size;
    cardDataLen[2][0] = hear_2_png_size;
    cardDataLen[2][1] = hear_3_png_size;
    cardDataLen[2][2] = hear_4_png_size;
    cardDataLen[2][3] = hear_5_png_size;
    cardDataLen[2][4] = hear_6_png_size;
    cardDataLen[2][5] = hear_7_png_size;
    cardDataLen[2][6] = hear_8_png_size;
    cardDataLen[2][7] = hear_9_png_size;
    cardDataLen[2][8] = hear_10_png_size;
    cardDataLen[2][9] = hear_jack_png_size;
    cardDataLen[2][10] = hear_queen_png_size;
    cardDataLen[2][11] = hear_king_png_size;
    cardDataLen[2][12] = hear_ace_png_size;
    cardDataLen[3][0] = spad_2_png_size;
    cardDataLen[3][1] = spad_3_png_size;
    cardDataLen[3][2] = spad_4_png_size;
    cardDataLen[3][3] = spad_5_png_size;
    cardDataLen[3][4] = spad_6_png_size;
    cardDataLen[3][5] = spad_7_png_size;
    cardDataLen[3][6] = spad_8_png_size;
    cardDataLen[3][7] = spad_9_png_size;
    cardDataLen[3][8] = spad_10_png_size;
    cardDataLen[3][9] = spad_jack_png_size;
    cardDataLen[3][10] = spad_queen_png_size;
    cardDataLen[3][11] = spad_king_png_size;
    cardDataLen[3][12] = spad_ace_png_size;
  }
  if (cardTextures[suit][card].height == -1) {
    Image tmp = LoadImageFromMemory(".png", cardData[suit][card],
                                    cardDataLen[suit][card] - 1);
    cardTextures[suit][card] = LoadTextureFromImage(tmp);
    UnloadImage(tmp);
  }
  return cardTextures[suit][card];
}
/*
Texture defaultCard = {-1, 0, 0, 0, 0};
Texture GetCardTexture(enum Suit suit, int card) {
  if (defaultCard.id == -1) {
    Image tmp = LoadImageFromMemory(".png", default_card_png,
                                    default_card_png_size - 1);
    defaultCard = LoadTextureFromImage(tmp);
    UnloadImage(tmp);
  }
  return defaultCard;
}
*/