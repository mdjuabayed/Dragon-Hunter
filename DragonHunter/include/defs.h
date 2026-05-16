#ifndef DEFS_H
#define DEFS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

// ── Screen ────────────────────────────────────────────────────────────────────
#define WIDTH           800
#define HEIGHT          600
#define FPS             60
#define FRAME_DELAY     (1000/FPS)

// ── Physics ───────────────────────────────────────────────────────────────────
#define GRAVITY         0.5f
#define JUMP_FORCE     -12.0f
#define PLAYER_SPEED    5
#define GROUND_Y        520
#define MAX_FALL_SPD    22.0f

// ── Limits ───────────────────────────────────────────────
#define MAX_PLATFORMS   16
#define MAX_ENEMIES     6
#define MAX_PARTICLES   160
#define MAX_COINS       16
#define MAX_SWORD_PICKS 4
#define MAX_PB_PICKS    4
#define MAX_POWERBALLS  4

// ── Sword settings ────────────────────────────────────────────────────────────
#define SWORD_MAX_HITS      5       // sword lasts 5 hits
#define SWORD_SWING_TIME    28
#define SWORD_COOL          36
#define SWORD_SPAWN_DELAY   480     // much rarer sword spawn
#define PB_SPAWN_DELAY      320

// ── Level system ─────────────────────────────────────────────────────────────
#define MAX_LEVELS      10

// ── Game states ───────────────────────────────────────────────────────────────
typedef enum {
    STATE_MENU = 0,
    STATE_LEVEL_BANNER,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_LEVEL_CLEAR,
    STATE_GAMEOVER,
    STATE_WIN
} GameState;

// ── Particle ──────────────────────────────────────────────────────────────────
typedef struct {
    float x, y, vx, vy;
    int   life, maxLife, size, active;
    Uint8 r, g, b;
} Particle;

// ── Platform ──────────────────────────────────────────────────────────────────
typedef struct {
    int x, y, w, h, type;
} Platform;

// ── Dragon (Enemy) ────────────────────────────────────────────────────────────
typedef struct {
    float x, y, vx, vy;
    int   hp, maxHp;
    int   alive, type, faceL;
    float angle, legAnim;
    int   shootTimer;
    int   chargeTimer, charging;
    float chargeVX;
} Dragon;

// ── Pickup ────────────────────────────────────────────────────────────────────
typedef struct {
    float x, y;
    int   type;     // 0=sword 1=powerball 2=health
    int   active;
} Pickup;

// ── Coin ─────────────────────────────────────────────────────────────────────
typedef struct {
    float x, y;
    int   active;
} Coin;

// ── Player ────────────────────────────────────────────────────────────────────
typedef struct {
    float x, y, vx, vy;
    int   hp, maxHp;
    int   facingRight;
    int   onGround;
    int   jumps;
    int   hasSword;
    int   swordHits;        // hits remaining on current sword
    int   swingTimer;
    int   swingCool;
    int   dmgCool;
    int   numPowerballs;
    int   pbCool;
    int   kills;
    int   coins;
    int   combo;
    int   comboTimer;
    int   score;
    int   jumpPressed;
} Player;

// ── Level config ──────────────────────────────────────────────────────────────
typedef struct {
    int   levelNum;
    int   killsRequired;
    int   maxFlying;
    int   maxGround;
    int   flyHp;
    int   gndHp;
    float speedMult;
    int   flySpawnDelay;
    int   gndSpawnDelay;
    int   chargeThreshold;
    Uint8 skyR, skyG, skyB;
    char  name[32];
    char  subtitle[64];
} LevelConfig;

// ── Audio ─────────────────────────────────────────────────────────────────────
typedef struct {
    Mix_Chunk* sword_swing;
    Mix_Chunk* sword_hit;
    Mix_Chunk* sword_pickup;
    Mix_Chunk* enemy_die;
    Mix_Chunk* player_hurt;
    Mix_Chunk* jump;
    Mix_Chunk* coin_pickup;
    Mix_Chunk* powerball_shoot;
    Mix_Chunk* powerball_hit;
    Mix_Chunk* level_clear;
    Mix_Chunk* powerball_pickup;
} Audio;

// ── Main game state ───────────────────────────────────────────────────────────
typedef struct {
    SDL_Window*   window;
    SDL_Renderer* renderer;
    TTF_Font*     fontTitle;
    TTF_Font*     fontLarge;
    TTF_Font*     fontSmall;
    Audio         audio;
    GameState     state;
    int           running;
    int           tick;
    int           levelNum;
    int           bannerTick;
    int           clearTick;
    LevelConfig   levelCfg;

    Player        player;
    Dragon        dragons[MAX_ENEMIES];
    Platform      plats[MAX_PLATFORMS];
    Pickup        swPicks[MAX_SWORD_PICKS];
    Pickup        pbPicks[MAX_PB_PICKS];
    Coin          coins[MAX_COINS];

    float         pbX[MAX_POWERBALLS];
    float         pbY[MAX_POWERBALLS];
    float         pbVX[MAX_POWERBALLS];
    int           pbActive[MAX_POWERBALLS];

    int           numSwPick;
    int           numPbPick;
    int           spawnFlyT;
    int           spawnGndT;
    int           swSpawnT;
    int           pbSpawnT;
    int           coinSpawnT;
    int           lastFlySpawnX;
    int           lastGndSpawnX;
    int           camX;
} Game;

#endif // DEFS_H