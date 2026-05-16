#ifndef PLAYER_H
#define PLAYER_H

#include "defs.h"

void Player_Init(Player* p);
void Player_HandleInput(Player* p, const Uint8* keys);
void Player_Update(Player* p, Platform plats[]);
void Player_TakeDamage(Player* p, int amount);

#endif