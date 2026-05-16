#include "../include/pickup.h"
#include "../include/audio.h"

void Pickup_TrySpawnSword(Pickup swPicks[], int *numSw,
                          int *swSpawnT, Platform plats[],
                          int camX, int hasSword)
{
    (*swSpawnT)++;
    if (*swSpawnT < SWORD_SPAWN_DELAY)
        return;
    if (hasSword)
        return;
    if (*numSw >= 1)
        return; // only 1 sword at a time

    int pi;
    for (pi = 1; pi < MAX_PLATFORMS; pi++)
    {
        if (plats[pi].x > camX + WIDTH / 2 &&
            plats[pi].x < camX + WIDTH)
        {
            int si, dup = 0;
            for (si = 0; si < MAX_SWORD_PICKS; si++)
                if (swPicks[si].active &&
                    (int)swPicks[si].x > plats[pi].x &&
                    (int)swPicks[si].x < plats[pi].x + plats[pi].w)
                    dup = 1;
            if (!dup)
            {
                // Find free slot
                for (si = 0; si < MAX_SWORD_PICKS; si++)
                {
                    if (!swPicks[si].active)
                    {
                        swPicks[si].x = (float)(plats[pi].x + plats[pi].w / 2 - 9);
                        swPicks[si].y = (float)(plats[pi].y - 48);
                        swPicks[si].type = 0;
                        swPicks[si].active = 1;
                        (*numSw)++;
                        *swSpawnT = 0;
                        return;
                    }
                }
            }
        }
    }
}

void Pickup_TrySpawnPB(Pickup pbPicks[], int *numPb,
                       int *pbSpawnT, Platform plats[],
                       int camX)
{
    (*pbSpawnT)++;
    if (*pbSpawnT < PB_SPAWN_DELAY)
        return;
    if (*numPb >= 2)
        return;

    int pi;
    for (pi = 1; pi < MAX_PLATFORMS; pi++)
    {
        if (plats[pi].x > camX + WIDTH / 2 + 100 &&
            plats[pi].x < camX + WIDTH + 100)
        {
            int si;
            for (si = 0; si < MAX_PB_PICKS; si++)
            {
                if (!pbPicks[si].active)
                {
                    pbPicks[si].x = (float)(plats[pi].x + plats[pi].w / 3);
                    pbPicks[si].y = (float)(plats[pi].y - 28);
                    pbPicks[si].type = 1;
                    pbPicks[si].active = 1;
                    (*numPb)++;
                    *pbSpawnT = 0;
                    return;
                }
            }
        }
    }
}

void Pickup_TrySpawnCoins(Coin coins[], int *coinSpawnT,
                          Platform plats[], int camX)
{
    (*coinSpawnT)++;
    if (*coinSpawnT < 90)
        return;
    *coinSpawnT = 0;

    int pi;
    for (pi = 1; pi < MAX_PLATFORMS; pi++)
    {
        if (plats[pi].x <= camX + 100 ||
            plats[pi].x >= camX + WIDTH + 100)
            continue;

        int ci, alreadyHas = 0;
        for (ci = 0; ci < MAX_COINS; ci++)
            if (coins[ci].active &&
                (int)coins[ci].x > plats[pi].x &&
                (int)coins[ci].x < plats[pi].x + plats[pi].w)
                alreadyHas = 1;

        if (!alreadyHas)
        {
            for (ci = 0; ci < MAX_COINS; ci++)
            {
                if (!coins[ci].active)
                {
                    coins[ci].x = (float)(plats[pi].x + plats[pi].w / 2);
                    coins[ci].y = (float)(plats[pi].y - 16);
                    coins[ci].active = 1;
                    return;
                }
            }
        }
    }
}

void Pickup_CheckCollect(Pickup swPicks[], int *numSw,
                         Pickup pbPicks[], int *numPb,
                         Coin coins[],
                         Player *p, int camX,
                         Audio *audio)
{
    int i;
    // Swords
    for (i = 0; i < MAX_SWORD_PICKS; i++)
    {
        if (!swPicks[i].active)
            continue;
        if (p->x < swPicks[i].x + 22 && p->x + 36 > swPicks[i].x &&
            p->y < swPicks[i].y + 48 && p->y + 52 > swPicks[i].y)
        {
            p->hasSword = 1;
            p->swordHits = SWORD_MAX_HITS;
            swPicks[i].active = 0;
            (*numSw)--;
            if (*numSw < 0)
                *numSw = 0;
            Particle_Explode(swPicks[i].x + 9, swPicks[i].y + 24,
                             198, 208, 252, 14);
            Audio_Play(audio->sword_pickup);
        }
        // Cull out of screen
        if (swPicks[i].x + 22 < camX - 100)
        {
            swPicks[i].active = 0;
            (*numSw)--;
            if (*numSw < 0)
                *numSw = 0;
        }
    }

    // Powerballs
    for (i = 0; i < MAX_PB_PICKS; i++)
    {
        if (!pbPicks[i].active)
            continue;
        if (p->x < pbPicks[i].x + 26 && p->x + 36 > pbPicks[i].x &&
            p->y < pbPicks[i].y + 26 && p->y + 52 > pbPicks[i].y)
        {
            p->numPowerballs++;
            pbPicks[i].active = 0;
            (*numPb)--;
            if (*numPb < 0)
                *numPb = 0;
            Particle_Explode(pbPicks[i].x + 13, pbPicks[i].y + 13,
                             172, 75, 248, 14);
            Audio_Play(audio->powerball_pickup);
        }
        if (pbPicks[i].x + 26 < camX - 100)
        {
            pbPicks[i].active = 0;
            (*numPb)--;
            if (*numPb < 0)
                *numPb = 0;
        }
    }

    // Coins
    for (i = 0; i < MAX_COINS; i++)
    {
        if (!coins[i].active)
            continue;
        float ddx = coins[i].x - (p->x + 18);
        float ddy = coins[i].y - (p->y + 26);
        if (ddx * ddx + ddy * ddy < 30 * 30)
        {
            p->coins++;
            p->score += 25;
            coins[i].active = 0;
            Particle_Spawn(coins[i].x, coins[i].y,
                           0, -3, 18, 252, 208, 30, 5);
            Audio_Play(audio->coin_pickup);
        }
        if (coins[i].x < camX - 100)
            coins[i].active = 0;
    }
}