#include "../include/hud.h"
#include "../include/draw.h"

void HUD_Draw(SDL_Renderer *r, TTF_Font *fL, TTF_Font *fS,
              Player *p, int level, int killsNeeded, int tick)
{
    // Left panel
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 172);
    SDL_Rect pan = {5, 5, 310, 138};
    SDL_RenderFillRect(r, &pan);
    SDL_SetRenderDrawColor(r, 162, 128, 36, 182);
    SDL_RenderDrawRect(r, &pan);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(r, 202, 165, 46, 255);
    SDL_RenderDrawLine(r, 5, 5, 315, 5);
    SDL_RenderDrawLine(r, 5, 6, 315, 6);

    // Level label
    char lvlStr[32];
    snprintf(lvlStr, sizeof(lvlStr), "LEVEL %d", level);
    TxtS(r, fS, lvlStr, 12, 8, 252, 205, 46);

    // Health bar
    HBar(r, 50, 22, 244, 16, p->hp, p->maxHp, 52, 215, 70);
    TxtS(r, fS, "LIFE", 12, 22, 252, 125, 125);

    // Kill progress
    char ks[64];
    snprintf(ks, sizeof(ks), "KILLS: %d / %d", p->kills, killsNeeded);
    TxtS(r, fL, ks, 12, 45, 252, 205, 36);

    // Score
    char sc[64];
    snprintf(sc, sizeof(sc), "SCORE: %d", p->score);
    TxtS(r, fS, sc, 12, 68, 200, 200, 200);

    // Coins
    SDL_SetRenderDrawColor(r, 252, 205, 30, 255);
    Circle(r, 18, 90, 7);
    SDL_SetRenderDrawColor(r, 252, 230, 98, 255);
    Circle(r, 18, 90, 5);
    char cs[32];
    snprintf(cs, sizeof(cs), "x %d", p->coins);
    TxtS(r, fS, cs, 30, 83, 252, 215, 38);

    SDL_SetRenderDrawColor(r, 142, 110, 26, 255);
    SDL_RenderDrawLine(r, 12, 100, 298, 100);

    // Sword slot
    int hasSw = p->hasSword;
    SDL_SetRenderDrawColor(r, hasSw ? 36 : 18, hasSw ? 36 : 18, hasSw ? 62 : 35, 255);
    SDL_Rect ss = {12, 104, 42, 32};
    SDL_RenderFillRect(r, &ss);
    SDL_SetRenderDrawColor(r, hasSw ? 182 : 68, hasSw ? 148 : 58, hasSw ? 46 : 15, 255);
    SDL_RenderDrawRect(r, &ss);
    if (hasSw)
    {
        SDL_SetRenderDrawColor(r, 215, 225, 252, 255);
        Thick(r, 33, 106, 33, 122, 2);
        SDL_SetRenderDrawColor(r, 175, 145, 30, 255);
        SDL_RenderDrawLine(r, 26, 122, 40, 122);
        SDL_SetRenderDrawColor(r, 95, 55, 14, 255);
        Thick(r, 33, 122, 33, 130, 2);
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(r, 142, 145, 252, (Uint8)(36 + (int)(sinf((float)tick * 0.15f) * 26)));
        SDL_RenderFillRect(r, &ss);
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
        // Sword hit dots
        char hits[16];
        snprintf(hits, sizeof(hits), "x%d", p->swordHits);
        TxtS(r, fS, hits, 56, 108, hasSw ? 200 : 80, hasSw ? 180 : 60, hasSw ? 60 : 20);
    }
    TxtS(r, fS, hasSw ? "[Z]" : "SWORD", 14, 137, hasSw ? 182 : 68, hasSw ? 155 : 58, hasSw ? 55 : 15);

    // Powerball slot
    int numPB = p->numPowerballs;
    SDL_SetRenderDrawColor(r, numPB > 0 ? 26 : 16, numPB > 0 ? 16 : 16, numPB > 0 ? 40 : 32, 255);
    SDL_Rect ps = {60, 104, 42, 32};
    SDL_RenderFillRect(r, &ps);
    SDL_SetRenderDrawColor(r, numPB > 0 ? 158 : 68, numPB > 0 ? 75 : 58, numPB > 0 ? 248 : 78, 255);
    SDL_RenderDrawRect(r, &ps);
    if (numPB > 0)
    {
        int i;
        for (i = 0; i < 10; i++)
        {
            float a1 = (float)i * 3.14159f / 5 - 1.5708f;
            float a2 = (float)(i + 1) * 3.14159f / 5 - 1.5708f;
            int r1 = i % 2 == 0 ? 11 : 5, r2 = (i + 1) % 2 == 0 ? 11 : 5;
            SDL_SetRenderDrawColor(r, 152, 70, 245, 255);
            SDL_RenderDrawLine(r, 81 + (int)(cosf(a1) * r1), 120 + (int)(sinf(a1) * r1),
                               81 + (int)(cosf(a2) * r2), 120 + (int)(sinf(a2) * r2));
        }
        char ps2[8];
        snprintf(ps2, sizeof(ps2), "x%d", numPB);
        TxtS(r, fS, ps2, 106, 112, 198, 134, 252);
    }
    TxtS(r, fS, numPB > 0 ? "[Q]" : "PBALL", 62, 137, numPB > 0 ? 178 : 68, numPB > 0 ? 95 : 58, numPB > 0 ? 248 : 78);

    // Combo display
    if (p->combo > 1 && p->comboTimer > 0)
    {
        char comb[32];
        snprintf(comb, sizeof(comb), "%dx COMBO!", p->combo);
        TxtC(r, fL, comb, WIDTH / 2, HEIGHT / 2 - 60, 255,
             (Uint8)(160 + (int)(sinf((float)tick * 0.2f) * 60)), 0);
    }

    // Controls panel (right)
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 142);
    SDL_Rect cp = {WIDTH - 218, 5, 213, 92};
    SDL_RenderFillRect(r, &cp);
    SDL_SetRenderDrawColor(r, 162, 128, 36, 182);
    SDL_RenderDrawRect(r, &cp);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(r, 202, 165, 46, 255);
    SDL_RenderDrawLine(r, WIDTH - 218, 5, WIDTH - 5, 5);
    TxtS(r, fS, "LEFT/RIGHT : Move", WIDTH - 213, 13, 158, 145, 95);
    TxtS(r, fS, "SPACE/UP   : Jump x2", WIDTH - 213, 31, 158, 145, 95);
    TxtS(r, fS, "Z          : Sword", WIDTH - 213, 49, 158, 145, 95);
    TxtS(r, fS, "Q          : Powerball", WIDTH - 213, 67, 158, 145, 95);
    TxtS(r, fS, "ESC        : Pause", WIDTH - 213, 79, 158, 145, 95);
}

// ─────────────────────────────────────────────────────────────────────────────
void HUD_DrawLevelBanner(SDL_Renderer *r, TTF_Font *fL,
                         TTF_Font *fS, LevelConfig *cfg,
                         int bannerTick)
{
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 195);
    SDL_Rect panel = {WIDTH / 2 - 272, HEIGHT / 2 - 72, 544, 144};
    SDL_RenderFillRect(r, &panel);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(r, 205, 165, 36, 255);
    SDL_RenderDrawRect(r, &panel);
    SDL_SetRenderDrawColor(r, 140, 110, 22, 255);
    SDL_Rect inner = {panel.x + 3, panel.y + 3, panel.w - 6, panel.h - 6};
    SDL_RenderDrawRect(r, &inner);
    SDL_SetRenderDrawColor(r, 175, 140, 28, 255);
    SDL_RenderDrawLine(r, panel.x + 20, panel.y + 52, panel.x + panel.w - 20, panel.y + 52);

    char lvlStr[32];
    snprintf(lvlStr, sizeof(lvlStr), "── LEVEL %d ──", cfg->levelNum);
    TxtC(r, fL, lvlStr, WIDTH / 2, panel.y + 8, 252, 205, 36);
    TxtC(r, fL, cfg->name, WIDTH / 2, panel.y + 58, 255, 245, 210);
    TxtC(r, fS, cfg->subtitle, WIDTH / 2, panel.y + 98, 185, 165, 115);

    char killStr[64];
    snprintf(killStr, sizeof(killStr), "Slay %d dragons to advance", cfg->killsRequired);
    TxtC(r, fS, killStr, WIDTH / 2, panel.y + panel.h + 12, 255, 120, 60);

    if (bannerTick > 90 && (bannerTick / 22) % 2 == 0)
        TxtC(r, fS, "Press ENTER to begin!", WIDTH / 2, panel.y + panel.h + 34, 80, 220, 80);
}

// ─────────────────────────────────────────────────────────────────────────────
void HUD_DrawLevelClear(SDL_Renderer *r, TTF_Font *fL,
                        TTF_Font *fS, LevelConfig *cfg,
                        Player *p, int tick)
{
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 185);
    SDL_Rect ov = {0, 0, WIDTH, HEIGHT};
    SDL_RenderFillRect(r, &ov);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);

    SDL_SetRenderDrawColor(r, 252, 205, 36, 255);
    SDL_Rect tb = {WIDTH / 2 - 240, 120, 480, 68};
    SDL_RenderFillRect(r, &tb);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    TxtC(r, fL, "LEVEL CLEARED!", WIDTH / 2, 136, 30, 20, 5);

    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 5, 5, 20, 210);
    SDL_Rect sp = {WIDTH / 2 - 220, 205, 440, 165};
    SDL_RenderFillRect(r, &sp);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(r, 158, 128, 30, 178);
    SDL_RenderDrawRect(r, &sp);

    char n1[64];
    snprintf(n1, sizeof(n1), "Level %d: %s completed!", cfg->levelNum, cfg->name);
    TxtC(r, fS, n1, WIDTH / 2, 215, 252, 205, 46);

    char n2[64];
    snprintf(n2, sizeof(n2), "Dragons Slain : %d", p->kills);
    TxtC(r, fL, n2, WIDTH / 2, 245, 252, 155, 36);
    char n3[64];
    snprintf(n3, sizeof(n3), "Coins Collected : %d", p->coins);
    TxtC(r, fS, n3, WIDTH / 2, 278, 252, 215, 36);
    char n4[64];
    snprintf(n4, sizeof(n4), "Score : %d", p->score);
    TxtC(r, fL, n4, WIDTH / 2, 308, 255, 232, 55);

    if (cfg->levelNum < MAX_LEVELS)
    {
        if ((tick / 24) % 2 == 0)
            TxtC(r, fS, "Press ENTER for next level", WIDTH / 2, 385, 80, 225, 80);
    }
    else
    {
        TxtC(r, fS, "FINAL LEVEL! Press ENTER", WIDTH / 2, 385, 252, 205, 36);
    }
    TxtC(r, fS, "ESC = Main Menu", WIDTH / 2, 410, 200, 100, 100);
}

// ─────────────────────────────────────────────────────────────────────────────
void HUD_DrawPause(SDL_Renderer *r, TTF_Font *fL, TTF_Font *fS, int tick)
{
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 172);
    SDL_Rect ov = {0, 0, WIDTH, HEIGHT};
    SDL_RenderFillRect(r, &ov);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);

    SDL_SetRenderDrawColor(r, 205, 165, 36, 255);
    SDL_Rect pan = {WIDTH / 2 - 160, HEIGHT / 2 - 80, 320, 160};
    SDL_RenderFillRect(r, &pan);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    SDL_RenderDrawRect(r, &pan);
    TxtC(r, fL, "PAUSED", WIDTH / 2, HEIGHT / 2 - 60, 20, 15, 5);

    if ((tick / 25) % 2 == 0)
        TxtC(r, fS, "Press ESC to resume", WIDTH / 2, HEIGHT / 2 - 10, 30, 20, 5);
    TxtC(r, fS, "ENTER = Main Menu", WIDTH / 2, HEIGHT / 2 + 20, 30, 20, 5);
}

// ─────────────────────────────────────────────────────────────────────────────
void HUD_DrawWin(SDL_Renderer *r, TTF_Font *fL, TTF_Font *fS,
                 Player *p, int tick)
{
    // Gold overlay
    int row;
    for (row = 0; row < HEIGHT; row++)
    {
        float t = (float)row / HEIGHT;
        SDL_SetRenderDrawColor(r, (Uint8)(15 + t * 25), (Uint8)(12 + t * 18), (Uint8)(3 + t * 8), 255);
        SDL_RenderDrawLine(r, 0, row, WIDTH, row);
    }

    SDL_SetRenderDrawColor(r, 252, 210, 40, 255);
    SDL_Rect tb = {WIDTH / 2 - 260, 80, 520, 80};
    SDL_RenderFillRect(r, &tb);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    TxtC(r, fL, "DRAGON HUNTER - COMPLETE!", WIDTH / 2, 100, 20, 15, 5);

    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 5, 5, 15, 215);
    SDL_Rect sp = {WIDTH / 2 - 240, 178, 480, 220};
    SDL_RenderFillRect(r, &sp);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(r, 205, 165, 36, 178);
    SDL_RenderDrawRect(r, &sp);

    TxtC(r, fS, "~ ALL 10 LEVELS CONQUERED ~", WIDTH / 2, 188, 195, 158, 52);
    SDL_SetRenderDrawColor(r, 138, 108, 24, 255);
    SDL_RenderDrawLine(r, WIDTH / 2 - 210, 208, WIDTH / 2 + 210, 208);

    char n1[64];
    snprintf(n1, sizeof(n1), "Total Dragons Slain : %d", p->kills);
    TxtC(r, fL, n1, WIDTH / 2, 218, 252, 205, 36);
    char n2[64];
    snprintf(n2, sizeof(n2), "Coins Collected     : %d", p->coins);
    TxtC(r, fS, n2, WIDTH / 2, 255, 252, 215, 36);
    char n3[64];
    snprintf(n3, sizeof(n3), "FINAL SCORE         : %d", p->score);
    TxtC(r, fL, n3, WIDTH / 2, 285, 255, 232, 55);

    // Rank
    const char *rank;
    Uint8 rkR, rkG, rkB;
    if (p->score >= 50000)
    {
        rank = "*** LEGENDARY DRAGON LORD ***";
        rkR = 255;
        rkG = 210;
        rkB = 0;
    }
    else if (p->score >= 30000)
    {
        rank = "** MASTER DRAGONSLAYER **";
        rkR = 95;
        rkG = 215;
        rkB = 115;
    }
    else if (p->score >= 15000)
    {
        rank = "* BRAVE DRAGON KNIGHT *";
        rkR = 95;
        rkG = 155;
        rkB = 248;
    }
    else
    {
        rank = "APPRENTICE HUNTER";
        rkR = 195;
        rkG = 155;
        rkB = 195;
    }
    TxtC(r, fS, rank, WIDTH / 2, 322, rkR, rkG, rkB);

    SDL_SetRenderDrawColor(r, 138, 108, 24, 255);
    SDL_RenderDrawLine(r, WIDTH / 2 - 210, 348, WIDTH / 2 + 210, 348);

    if ((tick / 24) % 2 == 0)
    {
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(r, 46, 26, 5, 220);
        SDL_Rect sb = {WIDTH / 2 - 188, 360, 376, 48};
        SDL_RenderFillRect(r, &sb);
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(r, 198, 158, 30, 255);
        SDL_RenderDrawRect(r, &sb);
        TxtC(r, fL, "ENTER = Play Again", WIDTH / 2, 376, 252, 212, 55);
    }
    TxtC(r, fS, "ESC = Main Menu", WIDTH / 2, 418, 200, 100, 100);
}