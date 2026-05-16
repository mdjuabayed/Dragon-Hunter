#ifndef DRAGON_H
#define DRAGON_H

#include "defs.h"
#include "particle.h"

void Dragon_Update(Dragon dragons[], int count,
                   Player* p, int tick,
                   Coin coins[], LevelConfig* cfg);
void Dragon_SpawnFlying(Dragon dragons[], int count,
                        int camX, LevelConfig* cfg,
                        int* lastSpawnX);
void Dragon_SpawnGround(Dragon dragons[], int count,
                        int camX, LevelConfig* cfg,
                        int* lastSpawnX);
int  Dragon_CountActive(Dragon dragons[], int count, int type);
void Dragon_CheckSwordHit(Dragon dragons[], int count,
                           Player* p, Coin coins[],
                           Audio* audio);
void Dragon_CheckPowerballHit(Dragon dragons[], int count,
                               Player* p,
                               float pbX[], float pbY[],
                               int pbActive[],
                               Coin coins[],
                               Audio* audio);
void Dragon_CheckBodyTouch(Dragon dragons[], int count,
                            Player* p, Audio* audio);

#endif