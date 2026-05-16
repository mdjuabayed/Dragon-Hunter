#include "../include/particle.h"

Particle parts[MAX_PARTICLES];

void Particle_InitAll(void)
{
    int i;
    for (i = 0; i < MAX_PARTICLES; i++)
        parts[i].active = 0;
}

void Particle_Spawn(float x, float y, float vx, float vy,
                    int life, Uint8 r, Uint8 g, Uint8 b, int sz)
{
    int i;
    for (i = 0; i < MAX_PARTICLES; i++)
    {
        if (!parts[i].active)
        {
            parts[i].x = x;
            parts[i].y = y;
            parts[i].vx = vx;
            parts[i].vy = vy;
            parts[i].life = life;
            parts[i].maxLife = life;
            parts[i].size = sz;
            parts[i].active = 1;
            parts[i].r = r;
            parts[i].g = g;
            parts[i].b = b;
            return;
        }
    }
}

void Particle_Explode(float x, float y,
                      Uint8 r, Uint8 g, Uint8 b, int count)
{
    int k;
    for (k = 0; k < count; k++)
    {
        float a = (float)k * 6.2832f / (float)count;
        float s = 1.5f + (float)(rand() % 20) * 0.15f;
        Particle_Spawn(x, y,
                       cosf(a) * s, sinf(a) * s,
                       28 + rand() % 20, r, g, b,
                       3 + rand() % 3);
    }
}

void Particle_Update(void)
{
    int i;
    for (i = 0; i < MAX_PARTICLES; i++)
    {
        if (!parts[i].active)
            continue;
        parts[i].x += parts[i].vx;
        parts[i].y += parts[i].vy;
        parts[i].vy += 0.08f;
        parts[i].vx *= 0.98f;
        if (--parts[i].life <= 0)
            parts[i].active = 0;
    }
}

void Particle_Render(SDL_Renderer *r, int camX)
{
    int i;
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    for (i = 0; i < MAX_PARTICLES; i++)
    {
        if (!parts[i].active)
            continue;
        float a = (float)parts[i].life / (float)parts[i].maxLife;
        Uint8 al = (Uint8)(a * 220.0f);
        SDL_SetRenderDrawColor(r,
                               parts[i].r, parts[i].g, parts[i].b, al);
        int sz = parts[i].size;
        SDL_Rect rc = {
            (int)parts[i].x - camX - sz / 2,
            (int)parts[i].y - sz / 2,
            sz, sz};
        SDL_RenderFillRect(r, &rc);
    }
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
}