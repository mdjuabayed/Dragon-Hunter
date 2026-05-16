#ifndef GAME_H
#define GAME_H

#include "defs.h"

int  Game_Init(Game* g);
void Game_Reset(Game* g);
void Game_ResetLevel(Game* g);
void Game_NextLevel(Game* g);
void Game_Run(Game* g);
void Game_Cleanup(Game* g);

#endif