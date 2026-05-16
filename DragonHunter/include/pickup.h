#ifndef PICKUP_H
#define PICKUP_H

#include "defs.h"
#include "particle.h"

void Pickup_TrySpawnSword(Pickup swPicks[], int* numSw,
                           int* swSpawnT, Platform plats[],
                           int camX, int hasSword);
void Pickup_TrySpawnPB(Pickup pbPicks[], int* numPb,
                        int* pbSpawnT, Platform plats[],
                        int camX);
void Pickup_TrySpawnCoins(Coin coins[], int* coinSpawnT,
                           Platform plats[], int camX);
void Pickup_CheckCollect(Pickup swPicks[], int* numSw,
                          Pickup pbPicks[], int* numPb,
                          Coin coins[],
                          Player* p, int camX,
                          Audio* audio);

#endif