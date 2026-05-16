#include "../include/menu.h"
#include "../include/draw.h"

void Menu_Draw(SDL_Renderer *r, TTF_Font *fT,
               TTF_Font *fL, TTF_Font *fS, int tick)
{
    // Background gradient
    int row;
    for (row = 0; row < HEIGHT; row++)
    {
        float t = (float)row / HEIGHT;
        SDL_SetRenderDrawColor(r, (Uint8)(4 + t * 13), (Uint8)(3 + t * 7), (Uint8)(15 + t * 26), 255);
        SDL_RenderDrawLine(r, 0, row, WIDTH, row);
    }

    // Stars
    srand(42);
    int s;
    for (s = 0; s < 100; s++)
    {
        int sx = rand() % WIDTH, sy = rand() % (HEIGHT * 60 / 100);
        float tw = sinf((float)tick * 0.046f + (float)s * 0.62f);
        SDL_SetRenderDrawColor(r, (Uint8)(105 + tw * 76), (Uint8)(105 + tw * 76), (Uint8)(142 + tw * 55), 255);
        SDL_RenderDrawPoint(r, sx, sy);
    }
    srand((unsigned)time(NULL));

    // Decorative dragons on menu
    Dragon pf;
    memset(&pf, 0, sizeof(Dragon));
    pf.x = 520;
    pf.y = 80;
    pf.hp = 4;
    pf.maxHp = 4;
    pf.alive = 1;
    pf.type = 0;
    pf.faceL = 1;
    DrawDragon(r, (int)pf.x, (int)pf.y, &pf, tick);

    Dragon pg;
    memset(&pg, 0, sizeof(Dragon));
    pg.x = 540;
    pg.y = 460;
    pg.hp = 6;
    pg.maxHp = 6;
    pg.alive = 1;
    pg.type = 1;
    pg.faceL = 0;
    pg.legAnim = (float)tick * 0.25f;
    DrawDragon(r, (int)pg.x, (int)pg.y, &pg, tick);

    DrawPlayer(r, 150, 478, 1, 0, 3, 0, tick, 1);

    // Title
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 13, 9, 34, 218);
    SDL_Rect tb = {WIDTH / 2 - 242, 75, 484, 82};
    SDL_RenderFillRect(r, &tb);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(r, 178, 142, 36, 202);
    SDL_RenderDrawRect(r, &tb);
    TxtC(r, fT ? fT : fL, "DRAGON HUNTER", WIDTH / 2, 91, 252, 205, 46);
    TxtC(r, fS, "~ Ancient Legends of the Sword - 10 Levels ~", WIDTH / 2, 135, 175, 150, 86);

    SDL_SetRenderDrawColor(r, 138, 108, 24, 255);
    SDL_RenderDrawLine(r, WIDTH / 2 - 182, 162, WIDTH / 2 + 182, 162);

    // Info panel
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 9, 7, 24, 205);
    SDL_Rect ip = {WIDTH / 2 - 235, 168, 470, 210};
    SDL_RenderFillRect(r, &ip);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(r, 138, 108, 24, 182);
    SDL_RenderDrawRect(r, &ip);

    TxtC(r, fS, "- HOW TO PLAY -", WIDTH / 2, 177, 195, 158, 52);
    SDL_SetRenderDrawColor(r, 100, 78, 14, 255);
    SDL_RenderDrawLine(r, WIDTH / 2 - 210, 194, WIDTH / 2 + 210, 194);
    TxtC(r, fS, "LEFT/RIGHT - Move      SPACE/UP - Double Jump!", WIDTH / 2, 202, 185, 175, 135);
    TxtC(r, fS, "Z - Swing Sword  (5 hits per sword! Use wisely)", WIDTH / 2, 222, 185, 175, 135);
    TxtC(r, fS, "Q - Fire Powerball    (ONE SHOT KILL! rare orbs)", WIDTH / 2, 242, 185, 175, 135);
    SDL_RenderDrawLine(r, WIDTH / 2 - 210, 262, WIDTH / 2 + 210, 262);
    TxtC(r, fS, "Slay flying wyverns + charging ground dragons!", WIDTH / 2, 270, 135, 195, 95);
    TxtC(r, fS, "Ground dragons CHARGE RED - dodge fast!", WIDTH / 2, 290, 215, 95, 55);
    TxtC(r, fS, "Collect GOLD COINS  |  Build KILL COMBOS", WIDTH / 2, 310, 252, 205, 36);
    TxtC(r, fS, "Swords are RARE - each lasts only 5 hits!", WIDTH / 2, 330, 252, 165, 36);
    SDL_RenderDrawLine(r, WIDTH / 2 - 210, 352, WIDTH / 2 + 210, 352);

    // Start button
    if ((tick / 22) % 2 == 0)
    {
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(r, 46, 26, 5, 228);
        SDL_Rect sb = {WIDTH / 2 - 190, 360, 380, 52};
        SDL_RenderFillRect(r, &sb);
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(r, 198, 158, 30, 255);
        SDL_RenderDrawRect(r, &sb);
        TxtC(r, fL ? fL : fS, "PRESS  ENTER  TO  BEGIN", WIDTH / 2, 379, 252, 212, 55);
    }
    TxtC(r, fS, "Dragon Hunter v6.0  |  10 Levels  |  Ancient Fantasy", WIDTH / 2, 425, 65, 55, 36);
}

// ─────────────────────────────────────────────────────────────────────────────
void GameOver_Draw(SDL_Renderer *r, TTF_Font *fT,
                   TTF_Font *fL, TTF_Font *fS,
                   Player *p, int level, int tick)
{
    // Dark red background
    int row;
    for (row = 0; row < HEIGHT; row++)
    {
        float t = (float)row / HEIGHT;
        SDL_SetRenderDrawColor(r, (Uint8)(15 + t * 9), (Uint8)(2 + t * 3), (Uint8)(2 + t * 5), 255);
        SDL_RenderDrawLine(r, 0, row, WIDTH, row);
    }

    // Blood drips
    srand(77);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    int di;
    for (di = 0; di < 22; di++)
    {
        int dx = rand() % WIDTH, dlen = 15 + rand() % 105, ddelay = rand() % 52;
        int curL = tick > ddelay ? (tick - ddelay) * 5 : 0;
        if (curL > dlen)
            curL = dlen;
        int dl;
        for (dl = 0; dl < curL; dl++)
        {
            SDL_SetRenderDrawColor(r, 148, 5, 5, (Uint8)(178 - dl * 178 / dlen));
            SDL_RenderDrawPoint(r, dx, dl);
        }
        if (curL == dlen)
        {
            SDL_SetRenderDrawColor(r, 168, 7, 7, 128);
            Circle(r, dx, curL + 4, 3);
        }
    }
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    srand((unsigned)time(NULL));

    // Decorative dragon
    Dragon pgov;
    memset(&pgov, 0, sizeof(Dragon));
    pgov.x = 465;
    pgov.y = 78;
    pgov.hp = 4;
    pgov.maxHp = 4;
    pgov.alive = 1;
    pgov.type = 0;
    pgov.faceL = 1;
    DrawDragon(r, (int)pgov.x, (int)pgov.y, &pgov, tick);

    // Skull
    int skX = WIDTH / 2, skY = 108;
    SDL_SetRenderDrawColor(r, 198, 195, 188, 255);
    Circle(r, skX, skY, 32);
    SDL_SetRenderDrawColor(r, 172, 168, 162, 255);
    Circle(r, skX - 9, skY + 10, 10);
    Circle(r, skX + 9, skY + 10, 10);
    SDL_Rect jaw3 = {skX - 14, skY + 18, 28, 13};
    SDL_RenderFillRect(r, &jaw3);
    SDL_SetRenderDrawColor(r, 14, 7, 7, 255);
    Thick(r, skX - 18, skY - 10, skX - 7, skY + 1, 2);
    Thick(r, skX - 7, skY - 10, skX - 18, skY + 1, 2);
    Thick(r, skX + 7, skY - 10, skX + 18, skY + 1, 2);
    Thick(r, skX + 18, skY - 10, skX + 7, skY + 1, 2);
    SDL_SetRenderDrawColor(r, 235, 228, 218, 255);
    int ti;
    for (ti = 0; ti < 6; ti++)
    {
        SDL_Rect tooth = {skX - 13 + ti * 5, skY + 24, 4, 8};
        SDL_RenderFillRect(r, &tooth);
    }

    // GAMEOVER panel
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 65, 3, 3, 225);
    SDL_Rect gop = {WIDTH / 2 - 248, 148, 496, 80};
    SDL_RenderFillRect(r, &gop);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(r, 198, 20, 20, 255);
    SDL_RenderDrawRect(r, &gop);
    TxtC(r, fT ? fT : fL, "THOU HAST FALLEN", WIDTH / 2, 166, 252, 55, 55);

    // Stats panel
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 10, 6, 16, 215);
    SDL_Rect sp = {WIDTH / 2 - 225, 242, 450, 195};
    SDL_RenderFillRect(r, &sp);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(r, 158, 128, 30, 178);
    SDL_RenderDrawRect(r, &sp);

    TxtC(r, fS, "~ HERO'S LEGACY ~", WIDTH / 2, 250, 195, 158, 50);
    SDL_SetRenderDrawColor(r, 138, 108, 24, 255);
    SDL_RenderDrawLine(r, WIDTH / 2 - 200, 268, WIDTH / 2 + 200, 268);

    char ln[64];
    snprintf(ln, sizeof(ln), "Reached Level  : %d / %d", level, MAX_LEVELS);
    TxtC(r, fS, ln, WIDTH / 2, 276, 200, 180, 120);
    char ks[64];
    snprintf(ks, sizeof(ks), "Dragons Slain  : %d", p->kills);
    TxtC(r, fL, ks, WIDTH / 2, 300, 252, 205, 36);
    char cs[64];
    snprintf(cs, sizeof(cs), "Coins Collected: %d", p->coins);
    TxtC(r, fS, cs, WIDTH / 2, 332, 252, 215, 36);
    char sc[64];
    snprintf(sc, sizeof(sc), "FINAL SCORE    : %d", p->score);
    TxtC(r, fL, sc, WIDTH / 2, 358, 255, 232, 55);

    // Rank
    const char *rank;
    Uint8 rkR, rkG, rkB;
    if (p->kills >= 40)
    {
        rank = "*** LEGENDARY DRAGON LORD ***";
        rkR = 255;
        rkG = 210;
        rkB = 0;
    }
    else if (p->kills >= 20)
    {
        rank = "** MASTER DRAGONSLAYER **";
        rkR = 95;
        rkG = 215;
        rkB = 115;
    }
    else if (p->kills >= 10)
    {
        rank = "* BRAVE DRAGON KNIGHT *";
        rkR = 95;
        rkG = 155;
        rkB = 248;
    }
    else if (p->kills >= 4)
    {
        rank = "APPRENTICE HUNTER";
        rkR = 195;
        rkG = 155;
        rkB = 195;
    }
    else
    {
        rank = "FALLEN ADVENTURER";
        rkR = 155;
        rkG = 115;
        rkB = 115;
    }
    TxtC(r, fS, rank, WIDTH / 2, 392, rkR, rkG, rkB);

    SDL_SetRenderDrawColor(r, 138, 108, 24, 255);
    SDL_RenderDrawLine(r, WIDTH / 2 - 200, 412, WIDTH / 2 + 200, 412);

    // Buttons
    if ((tick / 26) % 2 == 0)
    {
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(r, 26, 50, 12, 225);
        SDL_Rect rb = {WIDTH / 2 - 215, 420, 202, 46};
        SDL_RenderFillRect(r, &rb);
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(r, 75, 155, 36, 255);
        SDL_RenderDrawRect(r, &rb);
        TxtC(r, fS, "ENTER = TRY AGAIN", WIDTH / 2 - 114, 440, 75, 225, 75);
    }
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 50, 12, 12, 225);
    SDL_Rect qb = {WIDTH / 2 + 13, 420, 202, 46};
    SDL_RenderFillRect(r, &qb);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(r, 155, 50, 50, 255);
    SDL_RenderDrawRect(r, &qb);
    TxtC(r, fS, "ESC = MAIN MENU", WIDTH / 2 + 114, 440, 225, 95, 95);
    TxtC(r, fS, "\"Even heroes must rest... for a time.\"", WIDTH / 2, 475, 85, 72, 50);
}