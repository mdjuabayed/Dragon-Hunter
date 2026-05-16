#ifndef DRAW_H
#define DRAW_H

#include "defs.h"

// Low level primitives
void Circle(SDL_Renderer* r, int cx, int cy, int rad);
void Thick(SDL_Renderer* r, int x1, int y1, int x2, int y2, int t);
void Tri(SDL_Renderer* r, int x1, int y1,
         int x2, int y2, int x3, int y3);
void HBar(SDL_Renderer* r, int x, int y, int w, int h,
          int cur, int max2, Uint8 R, Uint8 G, Uint8 B);

// Text helpers
TTF_Font* LoadFont(const char** paths, int size);
void Txt(SDL_Renderer* r, TTF_Font* f, const char* t,
         int x, int y, Uint8 R, Uint8 G, Uint8 B);
void TxtS(SDL_Renderer* r, TTF_Font* f, const char* t,
          int x, int y, Uint8 R, Uint8 G, Uint8 B);
void TxtC(SDL_Renderer* r, TTF_Font* f, const char* t,
          int cx, int y, Uint8 R, Uint8 G, Uint8 B);

// Scene drawing
void DrawBG(SDL_Renderer* r, int camX, int tick,
            Uint8 skyR, Uint8 skyG, Uint8 skyB);
void DrawPlayer(SDL_Renderer* r, int x, int y,
                int hasSword, int swTimer,
                int swordHits, int dmg,
                int tick, int fR);
void DrawDragon(SDL_Renderer* r, int screenX, int screenY,
                Dragon* d, int tick);
void DrawPlat(SDL_Renderer* r, Platform p, int camX, int tick);
void DrawSwordPick(SDL_Renderer* r, float x, float y,
                   int tick, int camX);
void DrawPowerPick(SDL_Renderer* r, float x, float y,
                   int tick, int camX, int idx);
void DrawHealthPick(SDL_Renderer* r, float x, float y,
                    int tick, int camX);
void DrawCoinObj(SDL_Renderer* r, float x, float y,
                 int tick, int camX);
void DrawPowerball(SDL_Renderer* r, float x, float y,
                   float vx, int camX, int tick);

#endif