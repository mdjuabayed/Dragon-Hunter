#ifndef PARTICLE_H
#define PARTICLE_H

#include "defs.h"

extern Particle parts[MAX_PARTICLES];

void Particle_InitAll(void);
void Particle_Spawn(float x, float y, float vx, float vy,
                    int life, Uint8 r, Uint8 g, Uint8 b, int sz);
void Particle_Explode(float x, float y,
                      Uint8 r, Uint8 g, Uint8 b, int count);
void Particle_Update(void);
void Particle_Render(SDL_Renderer* r, int camX);

#endif