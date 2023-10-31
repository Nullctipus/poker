#include "raylib.h"
#include "resources.h"
#include "cards.h"
/*

const unsigned char* cardData[4][13] = {
    {diam_2_png,diam_3_png,diam_4_png,diam_5_png,diam_6_png,diam_7_png,diam_8_png,diam_9_png,diam_10_png,diam_jack_png,diam_queen_png,diam_king_png,diam_ace_png,},
    {club_2_png,club_3_png,club_4_png,club_5_png,club_6_png,club_7_png,club_8_png,club_9_png,club_10_png,club_jack_png,club_queen_png,club_king_png,club_ace_png,},
    {hear_2_png,hear_3_png,hear_4_png,hear_5_png,hear_6_png,hear_7_png,hear_8_png,hear_9_png,hear_10_png,hear_jack_png,hear_queen_png,hear_king_png,hear_ace_png,},
    {spad_2_png,spad_3_png,spad_4_png,spad_5_png,spad_6_png,spad_7_png,spad_8_png,spad_9_png,spad_10_png,spad_jack_png,spad_queen_png,spad_king_png,spad_ace_png,},
};
const unsigned char* cardDataLen[4][13] = {
    {diam_2_png_size,diam_3_png_size,diam_4_png_size,diam_5_png_size,diam_6_png_size,diam_7_png_size,diam_8_png_size,diam_9_png_size,diam_10_png_size,diam_jack_png_size,diam_queen_png_size,diam_king_png_size,diam_ace_png_size,},
    {club_2_png_size,club_3_png_size,club_4_png_size,club_5_png_size,club_6_png_size,club_7_png_size,club_8_png_size,club_9_png_size,club_10_png_size,club_jack_png_size,club_queen_png_size,club_king_png_size,club_ace_png_size,},
    {hear_2_png_size,hear_3_png_size,hear_4_png_size,hear_5_png_size,hear_6_png_size,hear_7_png_size,hear_8_png_size,hear_9_png_size,hear_10_png_size,hear_jack_png_size,hear_queen_png_size,hear_king_png_size,hear_ace_png_size,},
    {spad_2_png_size,spad_3_png_size,spad_4_png_size,spad_5_png_size,spad_6_png_size,spad_7_png_size,spad_8_png_size,spad_9_png_size,spad_10_png_size,spad_jack_png_size,spad_queen_png_size,spad_king_png_size,spad_ace_png_size,},
};

Texture cardTextures[4][13] = { 
    {{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},},
    {{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},},
    {{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},},
    {{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},},
};
Texture *GetCardTexture(enum Suit suit,int card)
{
    if(cardTextures[suit][card].height == -1)
    {
        Image tmp = LoadImageFromMemory(".png",cardData[suit][card],cardDataLen[suit][card]-1);
        cardTextures[suit][card] = LoadTextureFromImage(tmp);
        UnloadImage(tmp);
    }
    return &cardTextures[suit][card];
}
*/
Texture defaultCard = {-1,0,0,0,0};
Texture GetCardTexture(enum Suit suit,int card){
    if(defaultCard.id == -1){
        Image tmp = LoadImageFromMemory(".png",default_card_png,default_card_png_size-1);
        defaultCard = LoadTextureFromImage(tmp);
        UnloadImage(tmp);
    }
    return defaultCard;
}
