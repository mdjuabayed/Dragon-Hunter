#ifndef MENU_H
#define MENU_H

#include "defs.h"

void Menu_Draw(SDL_Renderer* r, TTF_Font* fT,
               TTF_Font* fL, TTF_Font* fS, int tick);
void GameOver_Draw(SDL_Renderer* r, TTF_Font* fT,
                   TTF_Font* fL, TTF_Font* fS,
                   Player* p, int level, int tick);

#endif