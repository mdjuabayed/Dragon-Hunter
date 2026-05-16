#ifndef AUDIO_H
#define AUDIO_H

#include "defs.h"

int  Audio_Init(Audio* a);
void Audio_Play(Mix_Chunk* chunk);
void Audio_PlayVolume(Mix_Chunk* chunk, int vol);
void Audio_Free(Audio* a);
void Audio_Quit(void);

#endif