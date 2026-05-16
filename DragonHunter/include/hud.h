#ifndef HUD_H
#define HUD_H

#include "defs.h"

void HUD_Draw(SDL_Renderer* r, TTF_Font* fL, TTF_Font* fS,
              Player* p, int level, int killsNeeded, int tick);
void HUD_DrawLevelBanner(SDL_Renderer* r, TTF_Font* fL,
                          TTF_Font* fS, LevelConfig* cfg,
                          int bannerTick);
void HUD_DrawLevelClear(SDL_Renderer* r, TTF_Font* fL,
                         TTF_Font* fS, LevelConfig* cfg,
                         Player* p, int tick);
void HUD_DrawPause(SDL_Renderer* r, TTF_Font* fL,
                    TTF_Font* fS, int tick);
void HUD_DrawWin(SDL_Renderer* r, TTF_Font* fL,
                  TTF_Font* fS, Player* p, int tick);

#endif