#ifndef LEVEL_H
#define LEVEL_H

#include "defs.h"

extern LevelConfig g_levels[MAX_LEVELS];

void Level_InitConfigs(void);
void Level_Get(int num, LevelConfig* out);
int  Level_IsComplete(int kills, LevelConfig* cfg);

#endif