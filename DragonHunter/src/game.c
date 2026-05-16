#include "../include/game.h"
#include "../include/draw.h"
#include "../include/particle.h"
#include "../include/player.h"
#include "../include/dragon.h"
#include "../include/platform.h"
#include "../include/pickup.h"
#include "../include/hud.h"
#include "../include/menu.h"
#include "../include/audio.h"
#include "../include/level.h"

// ─────────────────────────────────────────────────────────────────────────────
int Game_Init(Game *g)
{
    memset(g, 0, sizeof(Game));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("[SDL] Init failed: %s\n", SDL_GetError());
        return 0;
    }
    if (TTF_Init() < 0)
    {
        printf("[TTF] Init failed: %s\n", TTF_GetError());
        return 0;
    }

    g->window = SDL_CreateWindow(
        "Dragon Hunter - Ancient Legends",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, 0);
    if (!g->window)
    {
        printf("[SDL] Window failed\n");
        return 0;
    }

    g->renderer = SDL_CreateRenderer(g->window, -1,
                                     SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!g->renderer)
    {
        printf("[SDL] Renderer failed\n");
        return 0;
    }

    SDL_SetRenderDrawBlendMode(g->renderer, SDL_BLENDMODE_BLEND);

    // Load fonts
    const char *bP[] = {
        "C:/Windows/Fonts/arialbd.ttf", "C:/Windows/Fonts/calibrib.ttf",
        "C:/Windows/Fonts/verdanab.ttf", "C:/Windows/Fonts/trebucbd.ttf",
        "C:/Windows/Fonts/impact.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", NULL};
    const char *rP[] = {
        "C:/Windows/Fonts/arial.ttf", "C:/Windows/Fonts/calibri.ttf",
        "C:/Windows/Fonts/verdana.ttf", "C:/Windows/Fonts/tahoma.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", NULL};
    g->fontTitle = LoadFont(bP, 30);
    g->fontLarge = LoadFont(bP, 19);
    g->fontSmall = LoadFont(rP, 13);

    // Init subsystems
    Audio_Init(&g->audio);
    Level_InitConfigs();
    Particle_InitAll();
    srand((unsigned)time(NULL));

    g->state = STATE_MENU;
    g->running = 1;
    g->levelNum = 1;

    return 1;
}

// ─────────────────────────────────────────────────────────────────────────────
void Game_Reset(Game *g)
{
    g->levelNum = 1;
    Game_ResetLevel(g);
}

// ─────────────────────────────────────────────────────────────────────────────
void Game_ResetLevel(Game *g)
{
    Level_Get(g->levelNum, &g->levelCfg);

    Player_Init(&g->player);
    Particle_InitAll();

    // Reset enemies
    int i;
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        g->dragons[i].alive = 0;
        g->dragons[i].charging = 0;
        g->dragons[i].chargeTimer = 0;
    }

    // Reset pickups and coins
    for (i = 0; i < MAX_SWORD_PICKS; i++)
        g->swPicks[i].active = 0;
    for (i = 0; i < MAX_PB_PICKS; i++)
        g->pbPicks[i].active = 0;
    for (i = 0; i < MAX_COINS; i++)
        g->coins[i].active = 0;
    for (i = 0; i < MAX_POWERBALLS; i++)
        g->pbActive[i] = 0;

    g->numSwPick = 0;
    g->numPbPick = 0;
    g->spawnFlyT = 0;
    g->spawnGndT = 120;
    g->swSpawnT = 0;
    g->pbSpawnT = 0;
    g->coinSpawnT = 0;
    g->camX = 0;
    g->bannerTick = 0;
    g->clearTick = 0;

    g->lastFlySpawnX = 0;
    g->lastGndSpawnX = 0;

    Platform_InitLevel(g->plats, g->levelNum, g->camX);

    g->state = STATE_LEVEL_BANNER;
    g->tick = 0;
}

// ─────────────────────────────────────────────────────────────────────────────
void Game_NextLevel(Game *g)
{
    int prevKills = g->player.kills;
    int prevCoins = g->player.coins;
    int prevScore = g->player.score;

    g->levelNum++;
    if (g->levelNum > MAX_LEVELS)
    {
        g->state = STATE_WIN;
        return;
    }
    Game_ResetLevel(g);

    // Carry over stats
    g->player.kills = prevKills;
    g->player.coins = prevCoins;
    g->player.score = prevScore;
    // Restore HP as bonus
    g->player.hp = g->player.maxHp;
}

// ─────────────────────────────────────────────────────────────────────────────
void Game_Run(Game *g)
{
    SDL_Event ev;

    while (g->running)
    {
        Uint32 frameStart = SDL_GetTicks();
        g->tick++;

        // ── EVENTS ────────────────────────────────────────────────────────────
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
            {
                g->running = 0;
                break;
            }

            if (ev.type == SDL_KEYDOWN)
            {
                SDL_Scancode sc = ev.key.keysym.scancode;

                if (g->state == STATE_MENU)
                {
                    if (sc == SDL_SCANCODE_RETURN || sc == SDL_SCANCODE_KP_ENTER)
                    {
                        Game_Reset(g);
                    }
                    if (sc == SDL_SCANCODE_ESCAPE)
                        g->running = 0;
                }
                else if (g->state == STATE_LEVEL_BANNER)
                {
                    if (g->bannerTick > 60 &&
                        (sc == SDL_SCANCODE_RETURN || sc == SDL_SCANCODE_KP_ENTER))
                    {
                        g->state = STATE_PLAYING;
                    }
                }
                else if (g->state == STATE_PLAYING)
                {
                    if (sc == SDL_SCANCODE_ESCAPE)
                        g->state = STATE_PAUSED;

                    // Jump
                    if ((sc == SDL_SCANCODE_SPACE || sc == SDL_SCANCODE_UP) &&
                        !g->player.jumpPressed &&
                        g->player.jumps < 2)
                    {
                        g->player.vy = JUMP_FORCE;
                        g->player.jumps++;
                        g->player.jumpPressed = 1;
                        Particle_Spawn(g->player.x + 18, g->player.y + 52,
                                       0, -1, 20, 185, 165, 125, 4);
                        Audio_Play(g->audio.jump);
                    }
                }
                else if (g->state == STATE_PAUSED)
                {
                    if (sc == SDL_SCANCODE_ESCAPE)
                        g->state = STATE_PLAYING;
                    if (sc == SDL_SCANCODE_RETURN || sc == SDL_SCANCODE_KP_ENTER)
                    {
                        g->state = STATE_MENU;
                    }
                }
                else if (g->state == STATE_LEVEL_CLEAR)
                {
                    if (sc == SDL_SCANCODE_RETURN || sc == SDL_SCANCODE_KP_ENTER)
                    {
                        Game_NextLevel(g);
                    }
                    if (sc == SDL_SCANCODE_ESCAPE)
                        g->state = STATE_MENU;
                }
                else if (g->state == STATE_GAMEOVER)
                {
                    if (sc == SDL_SCANCODE_RETURN || sc == SDL_SCANCODE_KP_ENTER)
                    {
                        Game_Reset(g);
                    }
                    if (sc == SDL_SCANCODE_ESCAPE)
                        g->state = STATE_MENU;
                }
                else if (g->state == STATE_WIN)
                {
                    if (sc == SDL_SCANCODE_RETURN || sc == SDL_SCANCODE_KP_ENTER)
                    {
                        Game_Reset(g);
                    }
                    if (sc == SDL_SCANCODE_ESCAPE)
                        g->state = STATE_MENU;
                }
            }

            if (ev.type == SDL_KEYUP)
            {
                SDL_Scancode sc = ev.key.keysym.scancode;
                if (sc == SDL_SCANCODE_SPACE || sc == SDL_SCANCODE_UP)
                    g->player.jumpPressed = 0;
            }
        }

        // ── UPDATE ────────────────────────────────────────────────────────────
        if (g->state == STATE_LEVEL_BANNER)
        {
            g->bannerTick++;
        }
        else if (g->state == STATE_PLAYING)
        {
            const Uint8 *keys = SDL_GetKeyboardState(NULL);
            Player_HandleInput(&g->player, keys);

            // Sword attack
            if (keys[SDL_SCANCODE_Z] &&
                g->player.hasSword &&
                g->player.swingTimer == 0 &&
                g->player.swingCool == 0)
            {
                g->player.swingTimer = SWORD_SWING_TIME;
                g->player.swingCool = SWORD_COOL;
                int k;
                for (k = 0; k < 12; k++)
                {
                    float a = (float)(rand() % 70) * 0.089f - 0.52f;
                    float s = 2.8f + (float)(rand() % 3);
                    int fR = g->player.facingRight;
                    Particle_Spawn(g->player.x + (fR ? 36.0f : -2.0f), g->player.y + 18,
                                   cosf(a) * s * (fR ? 1 : -1), sinf(a) * s - 1.6f, 22, 198, 208, 252, 4);
                }
                Audio_Play(g->audio.sword_swing);
            }

            // Powerball fire
            if (keys[SDL_SCANCODE_Q] &&
                g->player.numPowerballs > 0 &&
                g->player.pbCool == 0)
            {
                int j;
                for (j = 0; j < MAX_POWERBALLS; j++)
                {
                    if (!g->pbActive[j])
                    {
                        int fR = g->player.facingRight;
                        g->pbX[j] = g->player.x + (fR ? 36.0f : 0.0f);
                        g->pbY[j] = g->player.y + 18;
                        g->pbVX[j] = fR ? 11.0f : -11.0f;
                        g->pbActive[j] = 1;
                        g->player.numPowerballs--;
                        g->player.pbCool = 40;
                        Particle_Explode(g->pbX[j], g->pbY[j], 172, 75, 248, 14);
                        Audio_Play(g->audio.powerball_shoot);
                        break;
                    }
                }
            }

            // Update player + platforms
            Player_Update(&g->player, g->plats);
            Platform_Recycle(g->plats, g->camX);

            // Camera
            int tCam = (int)g->player.x - WIDTH * 2 / 5;
            if (tCam < 0)
                tCam = 0;
            g->camX += (tCam - g->camX) / 8 + 1;

            // Clamp player
            if (g->player.x < g->camX)
                g->player.x = (float)g->camX;
            if (g->player.y > HEIGHT + 80)
                g->player.hp = 0;

            // Update powerballs
            int j;
            for (j = 0; j < MAX_POWERBALLS; j++)
            {
                if (!g->pbActive[j])
                    continue;
                g->pbX[j] += g->pbVX[j];
                if (g->pbX[j] < g->camX - 80 || g->pbX[j] > g->camX + WIDTH + 80)
                    g->pbActive[j] = 0;
            }

            // Spawn enemies
            g->spawnFlyT++;
            if (g->spawnFlyT >= g->levelCfg.flySpawnDelay &&
                Dragon_CountActive(g->dragons, MAX_ENEMIES, 0) < g->levelCfg.maxFlying)
            {
                Dragon_SpawnFlying(g->dragons, MAX_ENEMIES, g->camX,
                                   &g->levelCfg, &g->lastFlySpawnX);
                g->spawnFlyT = 0;
            }

            g->spawnGndT++;
            if (g->spawnGndT >= g->levelCfg.gndSpawnDelay &&
                Dragon_CountActive(g->dragons, MAX_ENEMIES, 1) < g->levelCfg.maxGround)
            {
                Dragon_SpawnGround(g->dragons, MAX_ENEMIES, g->camX,
                                   &g->levelCfg, &g->lastGndSpawnX);
                g->spawnGndT = 0;
            }

            // Update enemies
            Dragon_Update(g->dragons, MAX_ENEMIES,
                          &g->player, g->tick,
                          g->coins, &g->levelCfg);

            // Combat
            Dragon_CheckSwordHit(g->dragons, MAX_ENEMIES,
                                 &g->player, g->coins, &g->audio);
            Dragon_CheckPowerballHit(g->dragons, MAX_ENEMIES,
                                     &g->player,
                                     g->pbX, g->pbY, g->pbActive,
                                     g->coins, &g->audio);
            Dragon_CheckBodyTouch(g->dragons, MAX_ENEMIES,
                                  &g->player, &g->audio);

            // Pickups
            Pickup_TrySpawnSword(g->swPicks, &g->numSwPick, &g->swSpawnT,
                                 g->plats, g->camX, g->player.hasSword);
            Pickup_TrySpawnPB(g->pbPicks, &g->numPbPick, &g->pbSpawnT,
                              g->plats, g->camX);
            Pickup_TrySpawnCoins(g->coins, &g->coinSpawnT, g->plats, g->camX);
            Pickup_CheckCollect(g->swPicks, &g->numSwPick,
                                g->pbPicks, &g->numPbPick,
                                g->coins, &g->player, g->camX, &g->audio);

            Particle_Update();

            // Check level complete
            if (Level_IsComplete(g->player.kills, &g->levelCfg))
            {
                g->state = STATE_LEVEL_CLEAR;
                g->clearTick = 0;
                Audio_Play(g->audio.level_clear);
            }

            // Check death
            if (g->player.hp <= 0)
            {
                Particle_Explode(g->player.x + 18, g->player.y + 26, 252, 55, 55, 16);
                Particle_Explode(g->player.x + 18, g->player.y + 26, 252, 195, 45, 12);
                g->state = STATE_GAMEOVER;
                g->tick = 0;
            }
        }
        else if (g->state == STATE_LEVEL_CLEAR)
        {
            g->clearTick++;
            Particle_Update();
        }
        else if (g->state == STATE_GAMEOVER)
        {
            Particle_Update();
        }
        else if (g->state == STATE_WIN)
        {
            Particle_Update();
        }

        // ── RENDER ────────────────────────────────────────────────────────────
        SDL_RenderClear(g->renderer);

        if (g->state == STATE_MENU)
        {
            Menu_Draw(g->renderer, g->fontTitle, g->fontLarge, g->fontSmall, g->tick);
        }
        else if (g->state == STATE_LEVEL_BANNER)
        {
            DrawBG(g->renderer, 0, g->tick,
                   g->levelCfg.skyR, g->levelCfg.skyG, g->levelCfg.skyB);
            int i;
            for (i = 0; i < MAX_PLATFORMS; i++)
                DrawPlat(g->renderer, g->plats[i], 0, g->tick);
            HUD_DrawLevelBanner(g->renderer, g->fontLarge, g->fontSmall,
                                &g->levelCfg, g->bannerTick);
        }
        else if (g->state == STATE_PLAYING || g->state == STATE_PAUSED)
        {
            DrawBG(g->renderer, g->camX, g->tick,
                   g->levelCfg.skyR, g->levelCfg.skyG, g->levelCfg.skyB);

            // Platforms
            int i;
            for (i = 0; i < MAX_PLATFORMS; i++)
                DrawPlat(g->renderer, g->plats[i], g->camX, g->tick);

            // Pickups
            for (i = 0; i < MAX_SWORD_PICKS; i++)
            {
                if (!g->swPicks[i].active)
                    continue;
                DrawSwordPick(g->renderer, g->swPicks[i].x, g->swPicks[i].y,
                              g->tick, g->camX);
                TxtC(g->renderer, g->fontSmall, "SWORD",
                     (int)g->swPicks[i].x - g->camX + 9,
                     (int)g->swPicks[i].y - 16, 195, 178, 252);
            }
            for (i = 0; i < MAX_PB_PICKS; i++)
            {
                if (!g->pbPicks[i].active)
                    continue;
                DrawPowerPick(g->renderer, g->pbPicks[i].x, g->pbPicks[i].y,
                              g->tick, g->camX, i);
                TxtC(g->renderer, g->fontSmall, "POWERBALL",
                     (int)g->pbPicks[i].x - g->camX,
                     (int)g->pbPicks[i].y - 16, 195, 135, 252);
            }

            // Coins
            for (i = 0; i < MAX_COINS; i++)
                if (g->coins[i].active)
                    DrawCoinObj(g->renderer, g->coins[i].x, g->coins[i].y,
                                g->tick, g->camX);

            // Powerballs
            for (i = 0; i < MAX_POWERBALLS; i++)
            {
                if (!g->pbActive[i])
                    continue;
                DrawPowerball(g->renderer, g->pbX[i], g->pbY[i],
                              g->pbVX[i], g->camX, g->tick);
            }

            // Enemies - ground first then flying
            for (i = 0; i < MAX_ENEMIES; i++)
            {
                if (g->dragons[i].alive && g->dragons[i].type == 1)
                    DrawDragon(g->renderer,
                               (int)g->dragons[i].x - g->camX,
                               (int)g->dragons[i].y,
                               &g->dragons[i], g->tick);
            }
            for (i = 0; i < MAX_ENEMIES; i++)
            {
                if (g->dragons[i].alive && g->dragons[i].type == 0)
                    DrawDragon(g->renderer,
                               (int)g->dragons[i].x - g->camX,
                               (int)g->dragons[i].y,
                               &g->dragons[i], g->tick);
            }

            // Sword arc
            if (g->player.swingTimer > 0 && g->player.hasSword)
            {
                float prog = 1.0f - (float)g->player.swingTimer / 28.0f;
                int swX3 = (int)g->player.x + (g->player.facingRight ? 36 : -2) - g->camX;
                int swY3 = (int)g->player.y + 18;
                SDL_SetRenderDrawBlendMode(g->renderer, SDL_BLENDMODE_BLEND);
                int d2;
                for (d2 = 0; d2 < 60; d2 += 2)
                {
                    float t = (float)d2 / 60.0f;
                    SDL_SetRenderDrawColor(g->renderer,
                                           (Uint8)(175 + t * 40), (Uint8)(195 + t * 40), 252,
                                           (Uint8)(212 * (1.0f - t) * (1.0f - prog * 0.5f)));
                    float ang = (g->player.facingRight ? -1.0f : 0.2f) + prog * 1.6f + t * 0.42f;
                    int ex = swX3 + (int)(cosf(ang) * (56 - (float)d2)) * (g->player.facingRight ? 1 : -1);
                    int ey = swY3 + (int)(sinf(ang) * (56 - (float)d2));
                    SDL_RenderDrawLine(g->renderer, swX3, swY3, ex, ey);
                }
                SDL_SetRenderDrawBlendMode(g->renderer, SDL_BLENDMODE_NONE);
            }

            // Player
            DrawPlayer(g->renderer,
                       (int)g->player.x - g->camX, (int)g->player.y,
                       g->player.hasSword, g->player.swingTimer,
                       g->player.swordHits, g->player.dmgCool,
                       g->tick, g->player.facingRight);

            Particle_Render(g->renderer, g->camX);

            HUD_Draw(g->renderer, g->fontLarge, g->fontSmall,
                     &g->player, g->levelNum,
                     g->levelCfg.killsRequired, g->tick);

            if (g->state == STATE_PAUSED)
                HUD_DrawPause(g->renderer, g->fontLarge, g->fontSmall, g->tick);
        }
        else if (g->state == STATE_LEVEL_CLEAR)
        {
            DrawBG(g->renderer, g->camX, g->tick,
                   g->levelCfg.skyR, g->levelCfg.skyG, g->levelCfg.skyB);
            Particle_Render(g->renderer, g->camX);
            HUD_DrawLevelClear(g->renderer, g->fontLarge, g->fontSmall,
                               &g->levelCfg, &g->player, g->clearTick);
        }
        else if (g->state == STATE_GAMEOVER)
        {
            GameOver_Draw(g->renderer, g->fontTitle, g->fontLarge, g->fontSmall,
                          &g->player, g->levelNum, g->tick);
            Particle_Render(g->renderer, 0);
        }
        else if (g->state == STATE_WIN)
        {
            HUD_DrawWin(g->renderer, g->fontLarge, g->fontSmall,
                        &g->player, g->tick);
            Particle_Render(g->renderer, 0);
        }

        SDL_RenderPresent(g->renderer);

        Uint32 ft = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > (int)ft)
            SDL_Delay(FRAME_DELAY - ft);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
void Game_Cleanup(Game *g)
{
    Audio_Free(&g->audio);
    Audio_Quit();
    if (g->fontTitle)
        TTF_CloseFont(g->fontTitle);
    if (g->fontLarge)
        TTF_CloseFont(g->fontLarge);
    if (g->fontSmall)
        TTF_CloseFont(g->fontSmall);
    TTF_Quit();
    if (g->renderer)
        SDL_DestroyRenderer(g->renderer);
    if (g->window)
        SDL_DestroyWindow(g->window);
    SDL_Quit();
}