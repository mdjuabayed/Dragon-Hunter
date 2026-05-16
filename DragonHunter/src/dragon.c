#include "../include/dragon.h"
#include "../include/audio.h"
#include "../include/player.h"

int Dragon_CountActive(Dragon dragons[], int count, int type)
{
    int n = 0, i;
    for (i = 0; i < count; i++)
        if (dragons[i].alive && dragons[i].type == type)
            n++;
    return n;
}

void Dragon_SpawnFlying(Dragon dragons[], int count,
                        int camX, LevelConfig *cfg,
                        int *lastSpawnX)
{
    int spawnX = camX + WIDTH + 60 + rand() % 80;
    
    int i;
    for (i = 0; i < count; i++)
    {
        if (dragons[i].alive && dragons[i].type == 0)
        {
            float dx = dragons[i].x - spawnX;
            if (dx < 0)
                dx = -dx;
            if (dx < 300)
                return;
        }
    }
    for (i = 0; i < count; i++)
    {
        if (!dragons[i].alive)
        {
            Dragon *d = &dragons[i];
            d->x = (float)spawnX;
            d->y = (float)(60 + rand() % 180);
            d->vx = 0;
            d->vy = 0;
            d->hp = cfg->flyHp;
            d->maxHp = cfg->flyHp;
            d->alive = 1;
            d->type = 0;
            d->faceL = 1;
            d->angle = 0;
            d->legAnim = 0;
            d->shootTimer = 90;
            d->chargeTimer = 0;
            d->charging = 0;
            d->chargeVX = 0;
            *lastSpawnX = camX;
            return;
        }
    }
}

void Dragon_SpawnGround(Dragon dragons[], int count,
                        int camX, LevelConfig *cfg,
                        int *lastSpawnX)
{
    int spawnX = camX + WIDTH + 80 + rand() % 80;


    int i;
    for (i = 0; i < count; i++)
    {
        if (dragons[i].alive && dragons[i].type == 1)
        {
            float dx = dragons[i].x - spawnX;
            if (dx < 0)
                dx = -dx;
            if (dx < 350)
                return;
        }
    }
    for (i = 0; i < count; i++)
    {
        if (!dragons[i].alive)
        {
            Dragon *d = &dragons[i];
            d->x = (float)spawnX;
            d->y = (float)(GROUND_Y - 76);
            d->vx = 0;
            d->vy = 0;
            d->hp = cfg->gndHp;
            d->maxHp = cfg->gndHp;
            d->alive = 1;
            d->type = 1;
            d->faceL = 1;
            d->angle = 0;
            d->legAnim = 0;
            d->shootTimer = 80;
            d->chargeTimer = 0;
            d->charging = 0;
            d->chargeVX = 0;
            *lastSpawnX = camX;
            return;
        }
    }
}

static void drop_coins(Coin coins[], float x, float y, int n)
{
    int i, ci;
    for (i = 0; i < n; i++)
    {
        for (ci = 0; ci < MAX_COINS; ci++)
        {
            if (!coins[ci].active)
            {
                coins[ci].x = x + (float)(rand() % 30 - 15);
                coins[ci].y = y;
                coins[ci].active = 1;
                break;
            }
        }
    }
}

void Dragon_Update(Dragon dragons[], int count,
                   Player *p, int tick,
                   Coin coins[], LevelConfig *cfg)
{
    int i;
    float spd = cfg->speedMult;

    for (i = 0; i < count; i++)
    {
        if (!dragons[i].alive)
            continue;
        Dragon *d = &dragons[i];

        if (d->type == 0)
        {
            // Flying: smooth pursuit + sine weave
            float tdx = (p->x + 18) - (d->x + 40);
            float tdy = (p->y + 26) - (d->y + 38);
            float dist = sqrtf(tdx * tdx + tdy * tdy);
            float fspd = (1.2f + (float)p->kills * 0.06f) * spd;
            if (fspd > 3.2f)
                fspd = 3.2f;
            if (dist > 80)
            {
                d->x += tdx / dist * fspd;
                d->y += tdy / dist * fspd;
            }
            d->angle += 0.04f;
            d->y += sinf(d->angle) * 1.8f;
            if (d->y < 45)
                d->y = 45;
            if (d->y > 390)
                d->y = 390;
            d->faceL = (d->x + 40 > p->x + 18) ? 1 : 0;
        }
        else
        {
            // Ground: walk + charge
            float gspd = (1.4f + (float)p->kills * 0.05f) * spd;
            if (gspd > 3.5f)
                gspd = 3.5f;

            if (!d->charging)
            {
                if (p->x + 18 < d->x + 38)
                {
                    d->x -= gspd;
                    d->faceL = 1;
                }
                else
                {
                    d->x += gspd;
                    d->faceL = 0;
                }
                d->y = (float)(GROUND_Y - 76);

                d->chargeTimer++;
                float distP = fabsf((p->x + 18) - (d->x + 38));
                int thresh = cfg->chargeThreshold;
                if (d->chargeTimer > thresh && distP < 320)
                {
                    d->charging = 1;
                    d->chargeTimer = 0;
                    float dir = (p->x + 18 < d->x + 38) ? -1.0f : 1.0f;
                    d->chargeVX = dir * (gspd * 3.0f + 2.0f);
                    d->faceL = (d->chargeVX < 0) ? 1 : 0;
                    Particle_Explode(d->x + 38, d->y + 38, 255, 80, 30, 16);
                    Particle_Spawn(d->x + 38, d->y + 10,
                                   -d->chargeVX * 0.3f, -3.0f,
                                   30, 255, 60, 0, 6);
                }
            }
            else
            {
                d->x += d->chargeVX;
                d->y = (float)(GROUND_Y - 76);
                d->chargeTimer++;
                if (d->chargeTimer > 55)
                {
                    d->charging = 0;
                    d->chargeTimer = 0;
                    d->chargeVX = 0;
                    d->shootTimer = -60;
                }
                if (tick % 4 == 0)
                {
                    Particle_Spawn(d->x + 38, d->y + 68,
                                   -d->chargeVX * 0.1f + (float)(rand() % 6 - 3),
                                   -1.0f - (float)(rand() % 3),
                                   12, 200, 80, 20, 5);
                }
            }
            d->legAnim += (d->faceL ? -1.0f : 1.0f) *
                          (d->charging ? 0.55f : 0.22f);
        }

        // Cull off-screen
        if (d->x + (d->type == 0 ? 80 : 76) < -200)
            d->alive = 0;
    }
}

void Dragon_CheckSwordHit(Dragon dragons[], int count,
                          Player *p, Coin coins[],
                          Audio *audio)
{
    if (p->swingTimer <= 0)
        return;
    if (!p->hasSword)
        return;

    int i;
    float swX = p->facingRight ? (p->x + 36) : (p->x - 60);
    float swW = 60;

    for (i = 0; i < count; i++)
    {
        if (!dragons[i].alive)
            continue;
        Dragon *d = &dragons[i];
        float dw = (float)(d->type == 0 ? 80 : 76);
        float dh = (float)(d->type == 0 ? 76 : 68);

        if (swX < d->x + dw && swX + swW > d->x &&
            p->y < d->y + dh && p->y + 52 > d->y)
        {
            d->hp = 0; // One-hit kill
            Particle_Explode(d->x + 40, d->y + 38, 252, 75, 25, 14);
            Audio_Play(audio->sword_hit);

            // Consume 1 sword hit
            p->swordHits--;
            if (p->swordHits <= 0)
            {
                p->hasSword = 0;
                p->swordHits = 0;
                p->swingTimer = 0;
                p->swingCool = 0;
            }

            if (d->hp <= 0)
            {
                d->alive = 0;
                p->kills++;
                p->score += 100 * (p->combo > 1 ? p->combo : 1);
                p->combo++;
                p->comboTimer = 90;
                drop_coins(coins, d->x + 40, d->y + 20, 2);
                Particle_Explode(d->x + 40, d->y + 38, 252, 45, 45, 16);
                Particle_Explode(d->x + 40, d->y + 38, 252, 155, 25, 12);
                Audio_Play(audio->enemy_die);
            }
        }
    }
}

void Dragon_CheckPowerballHit(Dragon dragons[], int count,
                              Player *p,
                              float pbX[], float pbY[],
                              int pbActive[],
                              Coin coins[],
                              Audio *audio)
{
    int i, j;
    for (i = 0; i < count; i++)
    {
        if (!dragons[i].alive)
            continue;
        Dragon *d = &dragons[i];
        float dw = (float)(d->type == 0 ? 80 : 76);
        float dh = (float)(d->type == 0 ? 76 : 68);

        for (j = 0; j < MAX_POWERBALLS; j++)
        {
            if (!pbActive[j])
                continue;
            if (pbX[j] < d->x + dw && pbX[j] + 24 > d->x &&
                pbY[j] < d->y + dh && pbY[j] + 24 > d->y)
            {
                d->hp = 0;
                d->alive = 0;
                pbActive[j] = 0;
                p->kills++;
                p->score += 150 * (p->combo > 1 ? p->combo : 1);
                p->combo++;
                p->comboTimer = 90;
                Particle_Explode(d->x + 40, d->y + 38, 172, 75, 248, 18);
                Particle_Explode(d->x + 40, d->y + 38, 248, 95, 248, 14);
                Particle_Explode(d->x + 40, d->y + 20, 255, 200, 100, 12);
                drop_coins(coins, d->x + 40, d->y + 20, 3);
                Audio_Play(audio->powerball_hit);
                Audio_Play(audio->enemy_die);
            }
        }
    }
}

void Dragon_CheckBodyTouch(Dragon dragons[], int count,
                           Player *p, Audio *audio)
{
    int i;
    for (i = 0; i < count; i++)
    {
        if (!dragons[i].alive)
            continue;
        Dragon *d = &dragons[i];
        float dw = (float)(d->type == 0 ? 80 : 76);
        float dh = (float)(d->type == 0 ? 76 : 68);

        if (p->x < d->x + dw && p->x + 36 > d->x &&
            p->y < d->y + dh && p->y + 52 > d->y)
        {
            if (p->dmgCool == 0)
            {
                int dmg = (d->type == 1 && d->charging) ? 2 : 1;
                Player_TakeDamage(p, dmg);
                Audio_Play(audio->player_hurt);
                if (d->type == 1 && d->charging)
                {
                    d->charging = 0;
                    d->chargeTimer = 0;
                    d->chargeVX = 0;
                }
            }
        }
    }
}