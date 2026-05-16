#include "../include/level.h"
#include <string.h>

LevelConfig g_levels[MAX_LEVELS];

void Level_InitConfigs(void)
{

  // Level 1
  {
    LevelConfig *c = &g_levels[0];
    c->levelNum = 1;
    c->killsRequired = 5;
    c->maxFlying = 1;
    c->maxGround = 1;
    c->flyHp = 3;
    c->gndHp = 4;
    c->speedMult = 1.0f;
    c->flySpawnDelay = 180;
    c->gndSpawnDelay = 220;
    c->chargeThreshold = 200;
    c->skyR = 8;
    c->skyG = 10;
    c->skyB = 35;
    strcpy(c->name, "The Awakening");
    strcpy(c->subtitle, "A hunter rises. The dragons stir.");
  }

  // Level 2
  {
    LevelConfig *c = &g_levels[1];
    c->levelNum = 2;
    c->killsRequired = 8;
    c->maxFlying = 2;
    c->maxGround = 1;
    c->flyHp = 4;
    c->gndHp = 5;
    c->speedMult = 1.1f;
    c->flySpawnDelay = 165;
    c->gndSpawnDelay = 205;
    c->chargeThreshold = 180;
    c->skyR = 6;
    c->skyG = 8;
    c->skyB = 30;
    strcpy(c->name, "Dragon Trail");
    strcpy(c->subtitle, "Their tracks are fresh. Stay sharp.");
  }

  // Level 3
  {
    LevelConfig *c = &g_levels[2];
    c->levelNum = 3;
    c->killsRequired = 12;
    c->maxFlying = 2;
    c->maxGround = 2;
    c->flyHp = 5;
    c->gndHp = 7;
    c->speedMult = 1.2f;
    c->flySpawnDelay = 150;
    c->gndSpawnDelay = 190;
    c->chargeThreshold = 160;
    c->skyR = 22;
    c->skyG = 8;
    c->skyB = 8;
    strcpy(c->name, "Ember Pass");
    strcpy(c->subtitle, "The mountains burn. Press on.");
  }

  // Level 4
  {
    LevelConfig *c = &g_levels[3];
    c->levelNum = 4;
    c->killsRequired = 16;
    c->maxFlying = 3;
    c->maxGround = 2;
    c->flyHp = 6;
    c->gndHp = 8;
    c->speedMult = 1.3f;
    c->flySpawnDelay = 138;
    c->gndSpawnDelay = 175;
    c->chargeThreshold = 145;
    c->skyR = 30;
    c->skyG = 10;
    c->skyB = 5;
    strcpy(c->name, "Ashen Peaks");
    strcpy(c->subtitle, "Ash chokes the sky. Fight through.");
  }

  // Level 5
  {
    LevelConfig *c = &g_levels[4];
    c->levelNum = 5;
    c->killsRequired = 20;
    c->maxFlying = 3;
    c->maxGround = 2;
    c->flyHp = 7;
    c->gndHp = 10;
    c->speedMult = 1.45f;
    c->flySpawnDelay = 125;
    c->gndSpawnDelay = 160;
    c->chargeThreshold = 130;
    c->skyR = 40;
    c->skyG = 8;
    c->skyB = 3;
    strcpy(c->name, "Inferno Ridge");
    strcpy(c->subtitle, "Half way. The heat is unbearable.");
  }

  // Level 6
  {
    LevelConfig *c = &g_levels[5];
    c->levelNum = 6;
    c->killsRequired = 25;
    c->maxFlying = 3;
    c->maxGround = 3;
    c->flyHp = 8;
    c->gndHp = 12;
    c->speedMult = 1.6f;
    c->flySpawnDelay = 115;
    c->gndSpawnDelay = 148;
    c->chargeThreshold = 115;
    c->skyR = 10;
    c->skyG = 3;
    c->skyB = 28;
    strcpy(c->name, "Cursed Abyss");
    strcpy(c->subtitle, "Darkness descends. Elite dragons emerge.");
  }

  // Level 7
  {
    LevelConfig *c = &g_levels[6];
    c->levelNum = 7;
    c->killsRequired = 30;
    c->maxFlying = 4;
    c->maxGround = 3;
    c->flyHp = 10;
    c->gndHp = 14;
    c->speedMult = 1.75f;
    c->flySpawnDelay = 105;
    c->gndSpawnDelay = 138;
    c->chargeThreshold = 100;
    c->skyR = 5;
    c->skyG = 15;
    c->skyB = 35;
    strcpy(c->name, "Storm Citadel");
    strcpy(c->subtitle, "Lightning and wyverns. No mercy.");
  }

  // Level 8
  {
    LevelConfig *c = &g_levels[7];
    c->levelNum = 8;
    c->killsRequired = 36;
    c->maxFlying = 4;
    c->maxGround = 3;
    c->flyHp = 12;
    c->gndHp = 17;
    c->speedMult = 1.9f;
    c->flySpawnDelay = 95;
    c->gndSpawnDelay = 128;
    c->chargeThreshold = 88;
    c->skyR = 3;
    c->skyG = 3;
    c->skyB = 18;
    strcpy(c->name, "Void Spire");
    strcpy(c->subtitle, "The void hungers. So do the dragons.");
  }

  // Level 9
  {
    LevelConfig *c = &g_levels[8];
    c->levelNum = 9;
    c->killsRequired = 42;
    c->maxFlying = 4;
    c->maxGround = 4;
    c->flyHp = 14;
    c->gndHp = 20;
    c->speedMult = 2.1f;
    c->flySpawnDelay = 88;
    c->gndSpawnDelay = 118;
    c->chargeThreshold = 75;
    c->skyR = 35;
    c->skyG = 3;
    c->skyB = 3;
    strcpy(c->name, "Dragon Throne");
    strcpy(c->subtitle, "The dragon king watches. He waits.");
  }

  // Level 10
  {
    LevelConfig *c = &g_levels[9];
    c->levelNum = 10;
    c->killsRequired = 50;
    c->maxFlying = 5;
    c->maxGround = 4;
    c->flyHp = 18;
    c->gndHp = 25;
    c->speedMult = 2.4f;
    c->flySpawnDelay = 78;
    c->gndSpawnDelay = 108;
    c->chargeThreshold = 62;
    c->skyR = 45;
    c->skyG = 2;
    c->skyB = 2;
    strcpy(c->name, "The Final Hunt");
    strcpy(c->subtitle, "Legendary. Only the greatest survive.");
  }
}

void Level_Get(int num, LevelConfig *out)
{
  if (num < 1)
    num = 1;
  if (num > MAX_LEVELS)
    num = MAX_LEVELS;
  *out = g_levels[num - 1];
}

int Level_IsComplete(int kills, LevelConfig *cfg)
{
  return kills >= cfg->killsRequired;
}