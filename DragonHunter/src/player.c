#include "../include/player.h"
#include "../include/particle.h"

void Player_Init(Player *p)
{
    p->x = 120.0f;
    p->y = 460.0f;
    p->vx = 0;
    p->vy = 0;
    p->hp = 6;
    p->maxHp = 6;
    p->facingRight = 1;
    p->onGround = 0;
    p->jumps = 0;
    p->hasSword = 0;
    p->swordHits = 0;
    p->swingTimer = 0;
    p->swingCool = 0;
    p->dmgCool = 0;
    p->numPowerballs = 0;
    p->pbCool = 0;
    p->kills = 0;
    p->coins = 0;
    p->combo = 0;
    p->comboTimer = 0;
    p->score = 0;
    p->jumpPressed = 0;
}

void Player_HandleInput(Player *p, const Uint8 *keys)
{
    p->vx = 0;
    if (keys[SDL_SCANCODE_LEFT])
    {
        p->vx = -(float)PLAYER_SPEED;
        p->facingRight = 0;
    }
    if (keys[SDL_SCANCODE_RIGHT])
    {
        p->vx = (float)PLAYER_SPEED;
        p->facingRight = 1;
    }

    // Jump handled in Game_Run because we need audio
}

void Player_Update(Player *p, Platform plats[])
{
    // Gravity
    p->vy += GRAVITY;
    if (p->vy > MAX_FALL_SPD)
        p->vy = MAX_FALL_SPD;

    p->x += p->vx;
    p->y += p->vy;

    p->onGround = 0;
    int i;
    for (i = 0; i < MAX_PLATFORMS; i++)
    {
        Platform *pl = &plats[i];
        float prevBot = p->y + 52 - p->vy - 1;

        // Land on top
        if (p->x + 32 > pl->x && p->x + 4 < pl->x + pl->w &&
            p->y + 52 >= pl->y && prevBot <= pl->y + 10 && p->vy >= 0)
        {
            p->y = (float)(pl->y - 52);
            p->vy = 0;
            p->onGround = 1;
            p->jumps = 0;
        }
        // Hit underside
        if (p->x + 28 > pl->x && p->x + 8 < pl->x + pl->w &&
            p->y <= pl->y + pl->h && p->y - p->vy >= pl->y + pl->h && p->vy < 0)
        {
            p->y = (float)(pl->y + pl->h);
            p->vy = 1.0f;
        }
    }

    // Cooldowns
    if (p->swingCool > 0)
        p->swingCool--;
    if (p->swingTimer > 0)
        p->swingTimer--;
    if (p->dmgCool > 0)
        p->dmgCool--;
    if (p->pbCool > 0)
        p->pbCool--;
    if (p->comboTimer > 0)
        p->comboTimer--;
    else
        p->combo = 0;
}

void Player_TakeDamage(Player *p, int amount)
{
    if (p->dmgCool > 0)
        return;
    p->hp -= amount;
    p->dmgCool = 68;
    Particle_Explode(p->x + 18, p->y + 26, 252, 75, 25, 12);
}