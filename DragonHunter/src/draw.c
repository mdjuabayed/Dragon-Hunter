#include "../include/draw.h"
#include <math.h>

// ═══════════════════════════════════════════════════════════════
// PRIMITIVES
// ═══════════════════════════════════════════════════════════════

void Circle(SDL_Renderer *r, int cx, int cy, int rad)
{
    int dy, dx2;
    for (dy = -rad; dy <= rad; dy++)
        for (dx2 = -rad; dx2 <= rad; dx2++)
            if (dx2 * dx2 + dy * dy <= rad * rad)
                SDL_RenderDrawPoint(r, cx + dx2, cy + dy);
}

void Thick(SDL_Renderer *r, int x1, int y1,
           int x2, int y2, int t)
{
    int i;
    for (i = -t / 2; i <= t / 2; i++)
        if (abs(x2 - x1) >= abs(y2 - y1))
            SDL_RenderDrawLine(r, x1, y1 + i, x2, y2 + i);
        else
            SDL_RenderDrawLine(r, x1 + i, y1, x2 + i, y2);
}

void Tri(SDL_Renderer *r, int x1, int y1,
         int x2, int y2, int x3, int y3)
{
    int xs[3] = {x1, x2, x3}, ys[3] = {y1, y2, y3};
    int minY = y1 < y2 ? y1 : y2;
    if (y3 < minY)
        minY = y3;
    int maxY = y1 > y2 ? y1 : y2;
    if (y3 > maxY)
        maxY = y3;
    int y;
    for (y = minY; y <= maxY; y++)
    {
        int xl[2], xi = 0, e;
        for (e = 0; e < 3 && xi < 2; e++)
        {
            int n = (e + 1) % 3;
            if ((ys[e] <= y && y < ys[n]) || (ys[n] <= y && y < ys[e]))
                if (ys[n] != ys[e])
                    xl[xi++] = xs[e] + (int)((float)(y - ys[e]) / (ys[n] - ys[e]) * (xs[n] - xs[e]));
        }
        if (xi == 2)
        {
            if (xl[0] > xl[1])
            {
                int t2 = xl[0];
                xl[0] = xl[1];
                xl[1] = t2;
            }
            SDL_RenderDrawLine(r, xl[0], y, xl[1], y);
        }
    }
}

void HBar(SDL_Renderer *r, int x, int y, int w, int h,
          int cur, int max2, Uint8 R, Uint8 G, Uint8 B)
{
    if (max2 <= 0)
        return;
    SDL_SetRenderDrawColor(r, 20, 20, 20, 255);
    SDL_Rect bg = {x, y, w, h};
    SDL_RenderFillRect(r, &bg);
    int f = (cur > 0) ? (int)((float)cur / max2 * w) : 0;
    if (f > w)
        f = w;
    SDL_SetRenderDrawColor(r, R / 2, G / 2, B / 2, 255);
    SDL_Rect b1 = {x, y + h / 2, f, h / 2};
    SDL_RenderFillRect(r, &b1);
    SDL_SetRenderDrawColor(r, R, G, B, 255);
    SDL_Rect b2 = {x, y, f, h / 2};
    SDL_RenderFillRect(r, &b2);
    SDL_SetRenderDrawColor(r, 150, 120, 50, 255);
    SDL_RenderDrawRect(r, &bg);
}

// ═══════════════════════════════════════════════════════════════
// TEXT
// ═══════════════════════════════════════════════════════════════

TTF_Font *LoadFont(const char **pp, int sz)
{
    int i;
    for (i = 0; pp[i]; i++)
    {
        TTF_Font *f = TTF_OpenFont(pp[i], sz);
        if (f)
            return f;
    }
    return NULL;
}

void Txt(SDL_Renderer *r, TTF_Font *f, const char *t,
         int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
    if (!f || !t || !t[0])
        return;
    SDL_Color c = {R, G, B, 255};
    SDL_Surface *s = TTF_RenderText_Blended(f, t, c);
    if (!s)
        return;
    SDL_Texture *tx = SDL_CreateTextureFromSurface(r, s);
    SDL_Rect d = {x, y, s->w, s->h};
    SDL_FreeSurface(s);
    if (tx)
    {
        SDL_RenderCopy(r, tx, NULL, &d);
        SDL_DestroyTexture(tx);
    }
}

void TxtS(SDL_Renderer *r, TTF_Font *f, const char *t,
          int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
    Txt(r, f, t, x + 2, y + 2, 0, 0, 0);
    Txt(r, f, t, x, y, R, G, B);
}

void TxtC(SDL_Renderer *r, TTF_Font *f, const char *t,
          int cx, int y, Uint8 R, Uint8 G, Uint8 B)
{
    if (!f || !t || !t[0])
        return;
    int w2, h2;
    TTF_SizeText(f, t, &w2, &h2);
    TxtS(r, f, t, cx - w2 / 2, y, R, G, B);
}

// ═══════════════════════════════════════════════════════════════
// BACKGROUND
// ═══════════════════════════════════════════════════════════════

void DrawBG(SDL_Renderer *r, int camX, int tick,
            Uint8 skyR, Uint8 skyG, Uint8 skyB)
{
    // Gradient sky
    int row;
    for (row = 0; row < HEIGHT; row++)
    {
        float t = (float)row / HEIGHT;
        SDL_SetRenderDrawColor(r,
                               (Uint8)(skyR + t * 15),
                               (Uint8)(skyG + t * 9),
                               (Uint8)(skyB + t * 24), 255);
        SDL_RenderDrawLine(r, 0, row, WIDTH, row);
    }

    // Moon
    SDL_SetRenderDrawColor(r, 240, 238, 205, 255);
    Circle(r, 680, 85, 42);
    SDL_SetRenderDrawColor(r, skyR + 2, skyG + 2, skyB + 14, 255);
    Circle(r, 660, 76, 38);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 238, 235, 195, 18);
    Circle(r, 680, 85, 58);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);

    // Stars
    srand(42);
    int s;
    for (s = 0; s < 110; s++)
    {
        int sx = ((rand() % (WIDTH * 5)) - (camX / (4 + s % 3))) % WIDTH;
        int sy = rand() % (HEIGHT * 56 / 100);
        if (sx < 0)
            sx += WIDTH;
        float tw = sinf((float)tick * 0.042f + (float)s * 0.62f);
        Uint8 br = (Uint8)(102 + tw * 80);
        SDL_SetRenderDrawColor(r, br, br, (Uint8)(br + 36), 255);
        SDL_RenderDrawPoint(r, sx, sy);
        if (s % 3 == 0)
            SDL_RenderDrawPoint(r, sx + 1, sy);
    }
    srand((unsigned)time(NULL));

    // Far mountains
    SDL_SetRenderDrawColor(r, 16, 12, 34, 255);
    {
        int mp[] = {0, 425, 85, 305, 185, 390, 288, 270, 390, 350,
                    488, 260, 588, 345, 688, 278, 790, 365, 900, 425};
        int mi;
        for (mi = 0; mi < 9; mi++)
        {
            int ox = (camX / 8) % WIDTH;
            int mx1 = mp[mi * 2] - ox, my1 = mp[mi * 2 + 1];
            int mx2 = mp[(mi + 1) * 2] - ox, my2 = mp[(mi + 1) * 2 + 1];
            int fx;
            for (fx = mx1; fx <= mx2; fx++)
            {
                if (mx2 == mx1)
                    continue;
                int fy = my1 + (int)((float)(fx - mx1) / (mx2 - mx1) * (my2 - my1));
                SDL_RenderDrawLine(r, fx, fy, fx, HEIGHT);
            }
        }
    }

    // Near mountains
    SDL_SetRenderDrawColor(r, 22, 18, 46, 255);
    {
        int mp[] = {0, 455, 105, 368, 208, 435, 312, 340, 414, 410,
                    515, 328, 615, 398, 715, 342, 818, 410, 920, 455};
        int mi;
        for (mi = 0; mi < 9; mi++)
        {
            int ox = (camX / 5) % WIDTH;
            int mx1 = mp[mi * 2] - ox, my1 = mp[mi * 2 + 1];
            int mx2 = mp[(mi + 1) * 2] - ox, my2 = mp[(mi + 1) * 2 + 1];
            int fx;
            for (fx = mx1; fx <= mx2; fx++)
            {
                if (mx2 == mx1)
                    continue;
                int fy = my1 + (int)((float)(fx - mx1) / (mx2 - mx1) * (my2 - my1));
                SDL_RenderDrawLine(r, fx, fy, fx, HEIGHT);
            }
        }
    }

    // Dead trees
    srand(88);
    int ti;
    for (ti = 0; ti < 16; ti++)
    {
        int tx = ((rand() % (WIDTH * 5)) - (camX / 2)) % (WIDTH + 92);
        if (tx < -32)
            tx += WIDTH + 124;
        int th2 = 42 + rand() % 62, ty = HEIGHT - 56 - th2;
        SDL_SetRenderDrawColor(r, 20, 32, 14, 255);
        Thick(r, tx, ty + th2, tx + (rand() % 6 - 3), ty, 3 + rand() % 3);
        int bi2;
        for (bi2 = 0; bi2 < 3; bi2++)
        {
            int bx = tx + (rand() % 14 - 7), by2 = ty + th2 / 5 + bi2 * (th2 / 4);
            int bex = bx + (rand() % 26 - 13), bey = by2 - (rand() % 16 + 7);
            Thick(r, bx, by2, bex, bey, 1 + rand() % 2);
        }
        SDL_SetRenderDrawColor(r, 12, 22, 8, 255);
        Circle(r, tx, ty, 14 + rand() % 9);
        SDL_SetRenderDrawColor(r, 16, 28, 10, 255);
        Circle(r, tx - 8, ty + 6, 10 + rand() % 6);
        Circle(r, tx + 8, ty + 6, 10 + rand() % 6);
    }
    srand((unsigned)time(NULL));
}

// ═══════════════════════════════════════════════════════════════
// PLATFORM
// ═══════════════════════════════════════════════════════════════

void DrawPlat(SDL_Renderer *r, Platform p, int camX, int tick)
{
    (void)tick;
    int x = p.x - camX, y2 = p.y;

    // Shadow
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 72);
    SDL_Rect sh = {x + 5, y2 + 5, p.w, p.h};
    SDL_RenderFillRect(r, &sh);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);

    if (p.type == 0)
    {
        // Ground/dirt
        SDL_SetRenderDrawColor(r, 50, 34, 14, 255);
        SDL_Rect gr = {x, y2, p.w, p.h};
        SDL_RenderFillRect(r, &gr);
        SDL_SetRenderDrawColor(r, 38, 82, 22, 255);
        SDL_Rect gl = {x, y2, p.w, 11};
        SDL_RenderFillRect(r, &gl);
        SDL_SetRenderDrawColor(r, 55, 112, 30, 255);
        SDL_Rect gl2 = {x, y2, p.w, 4};
        SDL_RenderFillRect(r, &gl2);
        SDL_SetRenderDrawColor(r, 75, 142, 38, 255);
        int gi;
        for (gi = 4; gi < p.w; gi += 11)
        {
            SDL_RenderDrawLine(r, x + gi, y2, x + gi - 2, y2 - 8);
            SDL_RenderDrawLine(r, x + gi + 5, y2, x + gi + 7, y2 - 7);
        }
    }
    else
    {
        // Stone castle
        SDL_SetRenderDrawColor(r, 55, 47, 74, 255);
        SDL_Rect st = {x, y2, p.w, p.h};
        SDL_RenderFillRect(r, &st);
        SDL_SetRenderDrawColor(r, 68, 60, 90, 255);
        int bi;
        for (bi = 0; bi < p.w; bi += 34)
        {
            SDL_Rect bl = {x + bi, y2, 32, p.h - 2};
            SDL_RenderFillRect(r, &bl);
        }
        SDL_SetRenderDrawColor(r, 88, 80, 112, 255);
        SDL_Rect ts = {x, y2, p.w, 5};
        SDL_RenderFillRect(r, &ts);
        SDL_SetRenderDrawColor(r, 130, 118, 162, 255);
        SDL_RenderDrawLine(r, x, y2, x + p.w, y2);
        SDL_SetRenderDrawColor(r, 40, 78, 32, 255);
        int mi;
        for (mi = 0; mi < p.w; mi += 22)
        {
            SDL_Rect ms = {x + mi, y2, 12, 4};
            SDL_RenderFillRect(r, &ms);
        }
        SDL_SetRenderDrawColor(r, 44, 38, 60, 255);
        int si;
        for (si = 8; si < p.w; si += 16)
        {
            int sl = 4 + (si % 3) * 4;
            Thick(r, x + si, y2 + p.h, x + si, y2 + p.h + sl, 2);
        }
    }
}

// ═══════════════════════════════════════════════════════════════
// PLAYER
// ═══════════════════════════════════════════════════════════════

void DrawPlayer(SDL_Renderer *r, int x, int y,
                int hasSword, int swTimer,
                int swordHits, int dmg,
                int tick, int fR)
{
    int flash = (dmg > 0 && (dmg / 4) % 2);
    int cx = x + 18;
    Uint8 tR = flash ? 220 : 42, tG = flash ? 0 : 105, tB = flash ? 0 : 35;
    Uint8 aR = flash ? 180 : 75, aG = flash ? 0 : 85, aB = flash ? 0 : 95;
    Uint8 sR = flash ? 255 : 215, sG = flash ? 180 : 183, sB = flash ? 180 : 145;

    float wk = (float)sinf(tick * 0.28f);
    int lsw = (int)(wk * 6);

    // Shadow
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 50);
    SDL_Rect sh = {cx - 14, y + 52, 28, 5};
    SDL_RenderFillRect(r, &sh);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);

    // Boots
    SDL_SetRenderDrawColor(r, 58, 36, 14, 255);
    SDL_Rect bL = {cx - 12 + lsw, y + 44, 12, 9};
    SDL_RenderFillRect(r, &bL);
    SDL_Rect bR2 = {cx - lsw, y + 44, 12, 9};
    SDL_RenderFillRect(r, &bR2);
    SDL_SetRenderDrawColor(r, 85, 52, 22, 255);
    SDL_Rect bLh = {cx - 12 + lsw, y + 44, 12, 3};
    SDL_RenderFillRect(r, &bLh);
    SDL_Rect bRh = {cx - lsw, y + 44, 12, 3};
    SDL_RenderFillRect(r, &bRh);

    // Legs
    SDL_SetRenderDrawColor(r, 155, 142, 95, 255);
    Thick(r, cx - 6, y + 30, cx - 6 + lsw, y + 44, 4);
    Thick(r, cx + 5, y + 30, cx + 5 - lsw, y + 44, 4);
    SDL_SetRenderDrawColor(r, aR, aG, aB, 255);
    Circle(r, cx - 6 + lsw / 2, (y + 30 + y + 44) / 2, 5);
    Circle(r, cx + 5 - lsw / 2, (y + 30 + y + 44) / 2, 5);

    // Skirt/armour bottom
    SDL_SetRenderDrawColor(r,
                           (Uint8)(tR > 10 ? tR - 10 : 0),
                           (Uint8)(tG > 8 ? tG - 8 : 0),
                           (Uint8)(tB > 6 ? tB - 6 : 0), 255);
    int i;
    for (i = 0; i < 9; i++)
    {
        int tw2 = 15 - i / 2;
        SDL_RenderDrawLine(r, cx - tw2 / 2, y + 28 + i, cx + tw2 / 2, y + 28 + i);
    }
    SDL_SetRenderDrawColor(r, 175, 140, 30, 255);
    SDL_RenderDrawLine(r, cx - 7, y + 28, cx + 7, y + 28);

    // Body
    SDL_SetRenderDrawColor(r, tR, tG, tB, 255);
    SDL_Rect bod = {cx - 10, y + 14, 20, 16};
    SDL_RenderFillRect(r, &bod);
    SDL_SetRenderDrawColor(r, aR, aG, aB, 255);
    SDL_Rect cp = {cx - 9, y + 14, 18, 14};
    SDL_RenderFillRect(r, &cp);
    SDL_SetRenderDrawColor(r,
                           (Uint8)(aR + 35 < 255 ? aR + 35 : 255),
                           (Uint8)(aG + 28 < 255 ? aG + 28 : 255),
                           (Uint8)(aB + 28 < 255 ? aB + 28 : 255), 255);
    SDL_RenderDrawLine(r, cx - 8, y + 15, cx + 8, y + 15);
    SDL_SetRenderDrawColor(r,
                           (Uint8)(aR > 15 ? aR - 15 : 0),
                           (Uint8)(aG > 12 ? aG - 12 : 0),
                           (Uint8)(aB > 12 ? aB - 12 : 0), 255);
    SDL_RenderDrawLine(r, cx, y + 14, cx, y + 28);

    // Pauldrons
    SDL_SetRenderDrawColor(r,
                           (Uint8)(aR + 8 < 255 ? aR + 8 : 255),
                           (Uint8)(aG + 8 < 255 ? aG + 8 : 255),
                           (Uint8)(aB + 8 < 255 ? aB + 8 : 255), 255);
    SDL_Rect pL = {cx - 14, y + 13, 7, 6};
    SDL_RenderFillRect(r, &pL);
    SDL_Rect pR2 = {cx + 7, y + 13, 7, 6};
    SDL_RenderFillRect(r, &pR2);
    SDL_SetRenderDrawColor(r, 175, 140, 30, 255);
    SDL_RenderDrawLine(r, cx - 14, y + 13, cx - 7, y + 13);
    SDL_RenderDrawLine(r, cx + 7, y + 13, cx + 14, y + 13);

    // Belt
    SDL_SetRenderDrawColor(r, 55, 35, 12, 255);
    SDL_Rect belt = {cx - 10, y + 27, 20, 5};
    SDL_RenderFillRect(r, &belt);
    SDL_SetRenderDrawColor(r, 195, 155, 35, 255);
    SDL_Rect bk = {cx - 3, y + 28, 6, 3};
    SDL_RenderFillRect(r, &bk);

    // Shield
    {
        int shx = cx + (fR ? -16 : 16), shy = y + 18;
        SDL_SetRenderDrawColor(r, 48, 75, 155, 255);
        Circle(r, shx, shy + 6, 8);
        SDL_SetRenderDrawColor(r, 38, 60, 130, 255);
        Circle(r, shx, shy + 6, 6);
        SDL_SetRenderDrawColor(r, 195, 155, 35, 255);
        SDL_RenderDrawLine(r, shx, shy + 2, shx, shy + 10);
        SDL_RenderDrawLine(r, shx - 4, shy + 6, shx + 4, shy + 6);
        SDL_SetRenderDrawColor(r, 175, 135, 28, 255);
        Circle(r, shx, shy + 6, 2);
    }

    // Arms
    float aa = sinf((float)tick * 0.28f + 3.14f);
    int lax = cx - 10, lay = y + 15, rax = cx + 10, ray = y + 15;
    int lax2, lay2, rax2, ray2;
    if (hasSword && swTimer > 0)
    {
        float sa = -(float)swTimer / 28.0f * 3.14f * 0.9f;
        rax2 = rax + (int)(cosf(sa) * 16) * (fR ? 1 : -1);
        ray2 = ray + (int)(sinf(sa) * 16);
        lax2 = lax + (fR ? -5 : 5);
        lay2 = lay + 11;
    }
    else
    {
        int asw = (int)(aa * 7);
        lax2 = lax + (fR ? -4 : 4);
        lay2 = lay + 11 + asw / 2;
        rax2 = rax + (fR ? 4 : -4);
        ray2 = ray + 11 - asw / 2;
    }
    SDL_SetRenderDrawColor(r,
                           (Uint8)(tR > 8 ? tR - 8 : 0),
                           (Uint8)(tG > 6 ? tG - 6 : 0),
                           (Uint8)(tB > 5 ? tB - 5 : 0), 255);
    Thick(r, lax, lay, lax2, lay2, 4);
    Thick(r, rax, ray, rax2, ray2, 4);
    SDL_SetRenderDrawColor(r, 68, 44, 16, 255);
    Circle(r, lax2, lay2, 3);
    Circle(r, rax2, ray2, 3);

    // Sword
    if (hasSword)
    {
        // Color sword based on remaining hits
        // 5 hits = blue, 3 hits = yellow, 1 hit = red
        Uint8 sbR, sbG, sbB;
        if (swordHits >= 4)
        {
            sbR = 195;
            sbG = 210;
            sbB = 238;
        }
        else if (swordHits >= 2)
        {
            sbR = 238;
            sbG = 215;
            sbB = 80;
        }
        else
        {
            sbR = 255;
            sbG = 80;
            sbB = 80;
        }

        float sa = (swTimer > 0) ? -(float)swTimer / 28.0f * 3.14f * 0.9f : 0.1f;
        int sd = fR ? 1 : -1;
        int bx2 = rax2 + (int)(cosf(sa) * 32) * sd;
        int by2 = ray2 + (int)(sinf(sa - 0.22f) * 32);

        if (swTimer > 0)
        {
            SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
            int g;
            for (g = 10; g >= 2; g -= 2)
            {
                SDL_SetRenderDrawColor(r, sbR, sbG, sbB, (Uint8)(22 + g * 6));
                SDL_RenderDrawLine(r, rax2 - g / 3, ray2, bx2 - g / 3, by2);
                SDL_RenderDrawLine(r, rax2 + g / 3, ray2, bx2 + g / 3, by2);
            }
            SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
        }

        // Sword indicator dots (hits remaining)
        int hi2;
        for (hi2 = 0; hi2 < swordHits; hi2++)
        {
            SDL_SetRenderDrawColor(r, sbR, sbG, sbB, 255);
            Circle(r, cx - 8 + (hi2 * 5), y - 5, 3);
        }

        SDL_SetRenderDrawColor(r, sbR, sbG, sbB, 255);
        Thick(r, rax2, ray2, bx2, by2, 3);
        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderDrawLine(r, rax2, ray2, (rax2 + bx2) / 2, (ray2 + by2) / 2);
        SDL_SetRenderDrawColor(r, 165, 128, 25, 255);
        Thick(r, rax2 - 7 * sd, ray2 + 4, rax2 + 7 * sd, ray2 - 4, 4);
        SDL_SetRenderDrawColor(r, 78, 178, 255, 255);
        Circle(r, rax2, ray2, 3);

        int gi;
        for (gi = 0; gi < 9; gi++)
        {
            SDL_SetRenderDrawColor(r, gi % 2 ? 68 : 95, gi % 2 ? 38 : 55, gi % 2 ? 12 : 18, 255);
            float gt = (float)gi / 9;
            SDL_Rect gp = {(int)(rax + (gt * (rax2 - rax))) - 2,
                           (int)(ray + (gt * (ray2 - ray))) - 1, 4, 2};
            SDL_RenderFillRect(r, &gp);
        }
        SDL_SetRenderDrawColor(r, 175, 135, 25, 255);
        Circle(r, rax, ray, 4);
    }

    // Head
    {
        int hcx = cx + (fR ? 2 : -2), hcy = y + 8;
        SDL_SetRenderDrawColor(r, sR, sG, sB, 255);
        SDL_Rect nk = {hcx - 3, hcy + 9, 7, 6};
        SDL_RenderFillRect(r, &nk);
        SDL_SetRenderDrawColor(r, aR, aG, aB, 255);
        SDL_Rect gg = {hcx - 5, hcy + 12, 10, 5};
        SDL_RenderFillRect(r, &gg);
        SDL_SetRenderDrawColor(r, sR, sG, sB, 255);
        Circle(r, hcx, hcy + 2, 10);

        // Hair lines
        SDL_SetRenderDrawColor(r,
                               (Uint8)(sR > 22 ? sR - 22 : 0),
                               (Uint8)(sG > 20 ? sG - 20 : 0),
                               (Uint8)(sB > 15 ? sB - 15 : 0), 255);
        if (fR)
        {
            SDL_RenderDrawLine(r, hcx - 9, hcy - 1, hcx - 14, hcy - 9);
            SDL_RenderDrawLine(r, hcx - 9, hcy + 0, hcx - 13, hcy - 8);
        }
        else
        {
            SDL_RenderDrawLine(r, hcx + 9, hcy - 1, hcx + 14, hcy - 9);
            SDL_RenderDrawLine(r, hcx + 9, hcy + 0, hcx + 13, hcy - 8);
        }

        int eo = fR ? 3 : -3;
        SDL_SetRenderDrawColor(r, 235, 230, 215, 255);
        Circle(r, hcx + eo, hcy, 3);
        SDL_SetRenderDrawColor(r, 55, 95, 155, 255);
        Circle(r, hcx + eo, hcy, 2);
        SDL_SetRenderDrawColor(r, 8, 8, 8, 255);
        SDL_RenderDrawPoint(r, hcx + eo + (fR ? 1 : -1), hcy);
        SDL_SetRenderDrawColor(r, 155, 95, 72, 255);
        SDL_RenderDrawLine(r, hcx - 2, hcy + 4, hcx + 2, hcy + 4);

        // Helmet hair
        SDL_SetRenderDrawColor(r, aR, aG, aB, 255);
        int hi;
        for (hi = 0; hi < 13; hi++)
        {
            int hw = 10 - (int)(hi * 0.48f);
            if (hw < 2)
                hw = 2;
            SDL_RenderDrawLine(r, hcx - hw, hcy - 2 - hi, hcx + hw, hcy - 2 - hi);
        }
        SDL_SetRenderDrawColor(r,
                               (Uint8)(aR > 8 ? aR - 8 : 0),
                               (Uint8)(aG > 8 ? aG - 8 : 0),
                               (Uint8)(aB > 8 ? aB - 8 : 0), 255);
        SDL_Rect ns = {hcx - 2, hcy - 2, 4, 7};
        SDL_RenderFillRect(r, &ns);
        SDL_Rect cgL = {hcx - 12, hcy - 1, 4, 8};
        SDL_RenderFillRect(r, &cgL);
        SDL_Rect cgR2 = {hcx + 8, hcy - 1, 4, 8};
        SDL_RenderFillRect(r, &cgR2);
        SDL_SetRenderDrawColor(r, 175, 138, 28, 255);
        SDL_RenderDrawLine(r, hcx - 11, hcy - 2, hcx + 11, hcy - 2);

        // Plume
        SDL_SetRenderDrawColor(r, 175, 28, 28, 255);
        int pi;
        for (pi = 0; pi < 6; pi++)
        {
            float pa = sinf((float)tick * 0.15f + (float)pi * 0.38f) * 2.5f;
            Thick(r, hcx - 2 + pi, hcy - 13, hcx - 2 + pi + (int)pa, hcy - 22, 2);
        }
        SDL_SetRenderDrawColor(r, 188, 158, 48, 255);
        if (fR)
            SDL_RenderDrawLine(r, hcx - 9, hcy - 1, hcx - 12, hcy + 3);
        else
            SDL_RenderDrawLine(r, hcx + 9, hcy - 1, hcx + 12, hcy + 3);
    }
}

// ═══════════════════════════════════════════════════════════════
// DRAGON
// ═══════════════════════════════════════════════════════════════

void DrawDragon(SDL_Renderer *r, int screenX, int screenY,
                Dragon *d, int tick)
{
    // Reuse the full DrawDragon from demo (same code, just called here)
    int x = screenX, y = screenY;
    int w = (d->type == 0) ? 80 : 76;
    int h = (d->type == 0) ? 76 : 68;
    int cx = x + w / 2, cy = y + h / 2;
    int wd = d->faceL ? -1 : 1;

    if (d->type == 0)
    {
        float wf = sinf((float)tick * 0.18f);
        int wsX = cx, wsY = cy - h / 6;
        int welx = wsX + wd * 34, wely = wsY - 20 + (int)(wf * 18);
        int wt1x = wsX + wd * 58, wt1y = wsY - 54 + (int)(wf * 26);
        int wt2x = wsX + wd * 62, wt2y = wsY - 10 + (int)(wf * 16);
        int wt3x = wsX + wd * 52, wt3y = wsY + 26 + (int)(wf * 12);

        // Wing membrane glow
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        int wi;
        for (wi = 0; wi <= 18; wi++)
        {
            float t = (float)wi / 18;
            int tx1 = (int)(wsX * (1 - t) + wt1x * t), ty1 = (int)(wsY * (1 - t) + wt1y * t);
            int tx3 = (int)(wsX * (1 - t) + wt3x * t), ty3 = (int)(wsY * (1 - t) + wt3y * t);
            Uint8 mr = (Uint8)(145 + wi * 4), mg = (Uint8)(8 + wi * 2);
            SDL_SetRenderDrawColor(r, mr, mg, 8, (Uint8)(190 - wi * 8));
            SDL_RenderDrawLine(r, tx1, ty1, tx3, ty3);
        }
        // Inner wing highlight
        for (wi = 0; wi <= 8; wi++)
        {
            float t = (float)wi / 8 * 0.5f;
            int tx1 = (int)(wsX * (1 - t) + wt1x * t), ty1 = (int)(wsY * (1 - t) + wt1y * t);
            int tx3 = (int)(wsX * (1 - t) + wt3x * t), ty3 = (int)(wsY * (1 - t) + wt3y * t);
            SDL_SetRenderDrawColor(r, 255, 100, 20, (Uint8)(60 - wi * 7));
            SDL_RenderDrawLine(r, tx1, ty1, tx3, ty3);
        }
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);

        // Wing bones - darker crimson
        SDL_SetRenderDrawColor(r, 210, 48, 18, 255);
        Thick(r, wsX, wsY, welx, wely, 5);
        Thick(r, welx, wely, wt1x, wt1y, 3);
        Thick(r, welx, wely, wt2x, wt2y, 3);
        Thick(r, welx, wely, wt3x, wt3y, 3);
        // Wing tips
        SDL_SetRenderDrawColor(r, 248, 68, 12, 255);
        Circle(r, wt1x, wt1y, 4);
        Circle(r, wt2x, wt2y, 3);
        Circle(r, wt3x, wt3y, 3);

        // Tail - richer gradient with spines
        float tw0 = sinf((float)tick * 0.14f) * 12.0f;
        float tw1 = sinf((float)tick * 0.14f + 0.6f) * 18.0f;
        float tw2 = sinf((float)tick * 0.14f + 1.3f) * 24.0f;
        int tbx = cx - wd * (w / 3), tby = cy + h / 4;
        int t1x = tbx - wd * 24 + (int)(tw0 / 4), t1y = tby + 16;
        int t2x = tbx - wd * 48 + (int)(tw0), t2y = tby + 12 + (int)(tw1 / 2);
        int t3x = tbx - wd * 70 + (int)(tw1), t3y = tby + 6 + (int)(tw2 / 2);
        int t4x = tbx - wd * 88 + (int)(tw2), t4y = tby + (int)(tw2);
        SDL_SetRenderDrawColor(r, 195, 42, 12, 255);
        Thick(r, tbx, tby, t1x, t1y, 14);
        SDL_SetRenderDrawColor(r, 175, 34, 10, 255);
        Thick(r, t1x, t1y, t2x, t2y, 11);
        SDL_SetRenderDrawColor(r, 152, 28, 8, 255);
        Thick(r, t2x, t2y, t3x, t3y, 8);
        SDL_SetRenderDrawColor(r, 128, 22, 6, 255);
        Thick(r, t3x, t3y, t4x, t4y, 5);
        // Tail spike - bright orange
        SDL_SetRenderDrawColor(r, 255, 105, 15, 255);
        Tri(r, t4x - wd * 16, t4y, t4x + wd * 2, t4y - 12, t4x + wd * 2, t4y + 9);
        // Tail dorsal spines
        SDL_SetRenderDrawColor(r, 245, 80, 20, 255);
        Tri(r, tbx, tby - 2, tbx - wd * 4, tby - 12, tbx + wd * 4, tby - 2);
        Tri(r, t1x, t1y - 2, t1x - wd * 3, t1y - 9, t1x + wd * 3, t1y - 2);

        // Body - deeper reds with scale highlights
        SDL_SetRenderDrawColor(r, 145, 25, 8, 255);
        Circle(r, cx, cy, w / 3 + 4);
        Circle(r, cx - wd * 4, cy - 3, w / 4 + 2);
        SDL_SetRenderDrawColor(r, 195, 52, 22, 255);
        Circle(r, cx - wd * 9, cy - h / 8, w / 5 + 1);
        // Body highlight
        SDL_SetRenderDrawColor(r, 235, 72, 35, 255);
        Circle(r, cx - wd * 11, cy - h / 6, w / 8);
        // Scale dots
        SDL_SetRenderDrawColor(r, 115, 18, 5, 255);
        Circle(r, cx + wd * 4, cy + 4, 5);
        Circle(r, cx - wd * 2, cy + 8, 4);
        Circle(r, cx + wd * 8, cy + 10, 3);
        // Belly lighter shade
        SDL_SetRenderDrawColor(r, 215, 88, 42, 255);
        Circle(r, cx, cy + h / 5, w / 6);

        // Dorsal spines on back
        SDL_SetRenderDrawColor(r, 255, 90, 25, 255);
        Tri(r, cx - wd * 2, cy - w / 3 - 2, cx - wd * 8, cy - w / 3 - 14, cx + wd * 4, cy - w / 3 - 2);
        Tri(r, cx + wd * 6, cy - w / 3 + 2, cx + wd * 2, cy - w / 3 - 10, cx + wd * 10, cy - w / 3 + 2);

        // Neck+Head - thicker, more defined
        int nbx = cx + wd * (w / 4), nby = y + h / 3 + 3;
        int ntx = cx + wd * (w / 2 + 9), nty = y + h / 5;
        SDL_SetRenderDrawColor(r, 148, 28, 8, 255);
        Thick(r, nbx, nby, ntx, nty, 17);
        // Neck highlight
        SDL_SetRenderDrawColor(r, 205, 58, 25, 255);
        Thick(r, nbx + wd * 2, nby - 2, ntx + wd * 2, nty - 2, 6);

        int hdx = ntx + wd * 16, hdy = nty - 3;
        // Head - layered for depth
        SDL_SetRenderDrawColor(r, 135, 22, 6, 255);
        Circle(r, hdx, hdy + 6, 18);
        SDL_SetRenderDrawColor(r, 195, 45, 18, 255);
        Circle(r, hdx, hdy + 4, 15);
        // Snout/jaw
        SDL_SetRenderDrawColor(r, 168, 38, 14, 255);
        Circle(r, hdx + wd * 8, hdy + 8, 10);
        // Nostril
        SDL_SetRenderDrawColor(r, 88, 10, 4, 255);
        Circle(r, hdx + wd * 10, hdy + 6, 3);

        // Horns - bright bone color
        SDL_SetRenderDrawColor(r, 235, 215, 155, 255);
        Thick(r, hdx - wd * 2, hdy - 6, hdx - wd * 8, hdy - 22, 4);
        Thick(r, hdx + wd * 2, hdy - 8, hdx + wd * 5, hdy - 20, 3);
        // Horn tips
        SDL_SetRenderDrawColor(r, 255, 245, 195, 255);
        Circle(r, hdx - wd * 8, hdy - 22, 3);
        Circle(r, hdx + wd * 5, hdy - 20, 2);

        // Eye - larger, more menacing
        int eyx = hdx + wd * 6, eyy = hdy + 1;
        SDL_SetRenderDrawColor(r, 55, 5, 5, 255);
        Circle(r, eyx, eyy, 11);
        SDL_SetRenderDrawColor(r, 255, 215, 0, 255);
        Circle(r, eyx, eyy, 8);
        SDL_SetRenderDrawColor(r, 235, 155, 0, 255);
        Circle(r, eyx, eyy, 5);
        SDL_SetRenderDrawColor(r, 5, 2, 0, 255);
        Thick(r, eyx, eyy - 6, eyx, eyy + 6, 3);
        // Eye glow pulse
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        float eg = (sinf((float)tick * 0.15f) + 1.0f) * 0.5f;
        SDL_SetRenderDrawColor(r, 255, 185, 0, (Uint8)(80 + eg * 80));
        Circle(r, eyx, eyy, 14);
        SDL_SetRenderDrawColor(r, 255, 100, 0, (Uint8)(30 + eg * 40));
        Circle(r, eyx, eyy, 18);
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);

        // Flame breath - larger and more dramatic
        float ba = (sinf((float)tick * 0.13f) + 1.0f) * 0.5f;
        if (ba > 0.28f)
        {
            int mx = hdx + wd * 26, my = hdy + 11;
            SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
            int fi;
            // Outer flame (wide)
            for (fi = 0; fi < 20; fi++)
            {
                float fa = (float)fi / 20.0f * 1.4f - 0.7f;
                int fl = (int)(28 + ba * 38);
                int fx2 = mx + (int)(cosf(fa) * fl) * wd;
                int fy2 = my + (int)(sinf(fa) * fl * 0.55f);
                SDL_SetRenderDrawColor(r, 255, (Uint8)(30 + fi * 8), 0, (Uint8)(140 - fi * 6));
                Thick(r, mx, my, fx2, fy2, 2);
            }
            // Inner flame (bright)
            for (fi = 0; fi < 10; fi++)
            {
                float fa = (float)fi / 10.0f * 0.7f - 0.35f;
                int fl = (int)(18 + ba * 28);
                int fx2 = mx + (int)(cosf(fa) * fl) * wd;
                int fy2 = my + (int)(sinf(fa) * fl * 0.4f);
                SDL_SetRenderDrawColor(r, 255, (Uint8)(160 + fi * 8), 30, (Uint8)(200 - fi * 15));
                Thick(r, mx, my, fx2, fy2, 3);
            }
            // Flame core
            SDL_SetRenderDrawColor(r, 255, 245, 120, 210);
            Circle(r, mx + wd * 6, my, 8);
            SDL_SetRenderDrawColor(r, 255, 255, 210, 155);
            Circle(r, mx + wd * 4, my, 4);
            SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
        }
        HBar(r, cx - 36, y - 24, 72, 10, d->hp, d->maxHp, 255, 45, 45);
    }
    else
    {
        // Ground dragon
        float la = d->legAnim;

        if (d->charging)
        {
            SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
            int pulseA = (int)(sinf((float)tick * 0.4f) * 70 + 100);
            SDL_SetRenderDrawColor(r, 80, 255, 40, (Uint8)pulseA);
            SDL_Rect cg = {cx - w / 2 - 10, y - 10, w + 20, h + 20};
            SDL_RenderFillRect(r, &cg);
            SDL_SetRenderDrawColor(r, 200, 255, 80, (Uint8)(pulseA / 2));
            SDL_Rect cg2 = {cx - w / 2 - 5, y - 5, w + 10, h + 10};
            SDL_RenderFillRect(r, &cg2);
            SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
        }

        // Tail - richer greens with spines
        float tw1g = sinf((float)tick * 0.16f + 0.4f) * 15.0f;
        float tw2g = sinf((float)tick * 0.16f + 1.0f) * 22.0f;
        float tw3g = sinf((float)tick * 0.16f + 1.7f) * 28.0f;
        int tbxg = cx - wd * (w / 3 + 3), tbyg = cy + h / 4 - 3;
        int gt1x = tbxg - wd * 22 + (int)(tw1g / 4), gt1y = tbyg + 11;
        int gt2x = tbxg - wd * 44 + (int)(tw1g), gt2y = tbyg + 8 + (int)(tw2g / 3);
        int gt3x = tbxg - wd * 64 + (int)(tw2g), gt3y = tbyg + 5 + (int)(tw2g);
        int gt4x = tbxg - wd * 80 + (int)(tw3g), gt4y = tbyg + (int)(tw3g);
        SDL_SetRenderDrawColor(r, 62, 158, 18, 255);
        Thick(r, tbxg, tbyg, gt1x, gt1y, 13);
        SDL_SetRenderDrawColor(r, 52, 138, 14, 255);
        Thick(r, gt1x, gt1y, gt2x, gt2y, 10);
        SDL_SetRenderDrawColor(r, 42, 118, 10, 255);
        Thick(r, gt2x, gt2y, gt3x, gt3y, 7);
        SDL_SetRenderDrawColor(r, 32, 98, 8, 255);
        Thick(r, gt3x, gt3y, gt4x, gt4y, 4);
        // Tail spike
        SDL_SetRenderDrawColor(r, 185, 238, 42, 255);
        Tri(r, gt4x - wd * 14, gt4y, gt4x + wd * 2, gt4y - 10, gt4x + wd * 2, gt4y + 8);
        // Tail dorsal spines
        SDL_SetRenderDrawColor(r, 155, 220, 28, 255);
        Tri(r, tbxg, tbyg - 2, tbxg - wd * 4, tbyg - 13, tbxg + wd * 4, tbyg - 2);
        Tri(r, gt1x, gt1y - 2, gt1x - wd * 3, gt1y - 10, gt1x + wd * 3, gt1y - 2);

        // Legs - thicker, more muscular
        SDL_SetRenderDrawColor(r, 55, 148, 14, 255);
        int frx = cx + wd * 10, fry = y + h * 6 / 10;
        int fkx = (int)(frx + wd * 6 + sinf(la) * 12), fky = fry + 22;
        int ffx = (int)(fkx + sinf(la) * 6), ffy = y + h;
        Thick(r, frx, fry, fkx, fky, 11);
        SDL_SetRenderDrawColor(r, 42, 128, 10, 255);
        Thick(r, fkx, fky, ffx, ffy, 9);
        SDL_SetRenderDrawColor(r, 75, 172, 22, 255);
        Circle(r, fkx, fky, 7);

        SDL_SetRenderDrawColor(r, 55, 148, 14, 255);
        int flx2 = cx + wd * 5, fly2 = y + h * 6 / 10;
        int fkx2 = (int)(flx2 + wd * 4 - sinf(la) * 12), fky2 = fly2 + 22;
        int ffx2 = (int)(fkx2 - sinf(la) * 6), ffy2 = y + h;
        Thick(r, flx2, fly2, fkx2, fky2, 11);
        SDL_SetRenderDrawColor(r, 42, 128, 10, 255);
        Thick(r, fkx2, fky2, ffx2, ffy2, 9);
        SDL_SetRenderDrawColor(r, 75, 172, 22, 255);
        Circle(r, fkx2, fky2, 7);

        SDL_SetRenderDrawColor(r, 55, 148, 14, 255);
        int hlx = cx - wd * 8, hly = y + h * 6 / 10 + 3;
        int hkx = (int)(hlx - wd * 5 - sinf(la) * 10), hky = hly + 22;
        int hfx = (int)(hkx - sinf(la) * 5), hfy = y + h;
        Thick(r, hlx, hly, hkx, hky, 11);
        SDL_SetRenderDrawColor(r, 42, 128, 10, 255);
        Thick(r, hkx, hky, hfx, hfy, 9);
        SDL_SetRenderDrawColor(r, 75, 172, 22, 255);
        Circle(r, hkx, hky, 7);

        SDL_SetRenderDrawColor(r, 55, 148, 14, 255);
        int hlx2 = cx - wd * 3, hly2 = y + h * 6 / 10 + 3;
        int hkx2 = (int)(hlx2 - wd * 3 + sinf(la) * 10), hky2 = hly2 + 22;
        int hfx2 = (int)(hkx2 + sinf(la) * 5), hfy2 = y + h;
        Thick(r, hlx2, hly2, hkx2, hky2, 11);
        SDL_SetRenderDrawColor(r, 42, 128, 10, 255);
        Thick(r, hkx2, hky2, hfx2, hfy2, 9);
        SDL_SetRenderDrawColor(r, 75, 172, 22, 255);
        Circle(r, hkx2, hky2, 7);

        // Claws - sharper, longer
        SDL_SetRenderDrawColor(r, 215, 208, 172, 255);
        SDL_RenderDrawLine(r, ffx, ffy, ffx + wd * 10, ffy + 5);
        SDL_RenderDrawLine(r, ffx, ffy, ffx + wd * 6, ffy + 9);
        SDL_RenderDrawLine(r, ffx, ffy, ffx + wd * 2, ffy + 8);
        SDL_RenderDrawLine(r, ffx2, ffy2, ffx2 - wd * 10, ffy2 + 5);
        SDL_RenderDrawLine(r, ffx2, ffy2, ffx2 - wd * 6, ffy2 + 9);
        SDL_RenderDrawLine(r, ffx2, ffy2, ffx2 - wd * 2, ffy2 + 8);
        SDL_RenderDrawLine(r, hfx, hfy, hfx + wd * 10, hfy + 5);
        SDL_RenderDrawLine(r, hfx, hfy, hfx + wd * 6, hfy + 9);
        SDL_RenderDrawLine(r, hfx2, hfy2, hfx2 - wd * 10, hfy2 + 5);
        SDL_RenderDrawLine(r, hfx2, hfy2, hfx2 - wd * 6, hfy2 + 9);

        // Body - deeper greens with scale detail
        SDL_SetRenderDrawColor(r, 52, 118, 14, 255);
        Circle(r, cx, cy, w / 3 + 5);
        Circle(r, cx - wd * 5, cy - 3, w / 4 + 2);
        SDL_SetRenderDrawColor(r, 78, 158, 22, 255);
        Circle(r, cx - wd * 9, cy - 7, w / 5 + 2);
        // Body highlight
        SDL_SetRenderDrawColor(r, 108, 188, 38, 255);
        Circle(r, cx - wd * 12, cy - h / 6, w / 8);
        // Scale dots
        SDL_SetRenderDrawColor(r, 38, 95, 10, 255);
        Circle(r, cx + wd * 4, cy + 5, 5);
        Circle(r, cx - wd * 3, cy + 9, 4);
        Circle(r, cx + wd * 8, cy + 12, 3);
        // Belly plates
        SDL_SetRenderDrawColor(r, 155, 195, 52, 255);
        Circle(r, cx, cy + h / 5 + 2, w / 7);
        SDL_SetRenderDrawColor(r, 138, 178, 44, 255);
        Circle(r, cx + wd * 5, cy + h / 4, w / 9);

        // Dorsal armor plates
        SDL_SetRenderDrawColor(r, 52, 205, 28, 255);
        Tri(r, cx - wd * 2, cy - w / 3 - 2, cx - wd * 10, cy - w / 3 - 18, cx + wd * 6, cy - w / 3 - 2);
        SDL_SetRenderDrawColor(r, 42, 185, 22, 255);
        Tri(r, cx + wd * 6, cy - w / 3 + 2, cx + wd * 2, cy - w / 3 - 14, cx + wd * 10, cy - w / 3 + 2);
        SDL_SetRenderDrawColor(r, 32, 162, 18, 255);
        Tri(r, cx + wd * 12, cy - w / 3 + 5, cx + wd * 9, cy - w / 3 - 8, cx + wd * 15, cy - w / 3 + 5);

        // Neck+Head
        int gnbx = cx + wd * (w / 3 + 1), gnby = y + h / 3 + 1;
        int gntx = cx + wd * (w / 2 + 13), gnty = y + h / 5 - 2;
        SDL_SetRenderDrawColor(r, 52, 120, 14, 255);
        Thick(r, gnbx, gnby, gntx, gnty, 16);
        // Neck highlight
        SDL_SetRenderDrawColor(r, 88, 165, 28, 255);
        Thick(r, gnbx + wd * 2, gnby - 2, gntx + wd * 2, gnty - 2, 5);

        int ghdx = gntx + wd * 17, ghdy = gnty - 4;
        // Head layers
        SDL_SetRenderDrawColor(r, 42, 108, 10, 255);
        Circle(r, ghdx, ghdy + 5, 18);
        SDL_SetRenderDrawColor(r, 65, 145, 18, 255);
        Circle(r, ghdx, ghdy + 4, 15);
        // Snout
        SDL_SetRenderDrawColor(r, 58, 132, 16, 255);
        Circle(r, ghdx + wd * 9, ghdy + 9, 11);
        Circle(r, ghdx + wd * 7, ghdy + 8, 9);
        // Nostril
        SDL_SetRenderDrawColor(r, 22, 65, 6, 255);
        Circle(r, ghdx + wd * 12, ghdy + 7, 3);

        // Horns - curved, dramatic
        SDL_SetRenderDrawColor(r, 205, 188, 128, 255);
        Thick(r, ghdx - wd * 2, ghdy - 5, ghdx - wd * 10, ghdy - 20, 4);
        Thick(r, ghdx + wd * 2, ghdy - 7, ghdx + wd * 6, ghdy - 18, 3);
        // Horn tips
        SDL_SetRenderDrawColor(r, 245, 232, 175, 255);
        Circle(r, ghdx - wd * 10, ghdy - 20, 3);
        Circle(r, ghdx + wd * 6, ghdy - 18, 2);

        // Eye
        int geyx = ghdx + wd * 6, geyy = ghdy;
        SDL_SetRenderDrawColor(r, 22, 72, 5, 255);
        Circle(r, geyx, geyy, 9);
        SDL_SetRenderDrawColor(r, 228, 188, 0, 255);
        Circle(r, geyx, geyy, 7);
        SDL_SetRenderDrawColor(r, 192, 148, 0, 255);
        Circle(r, geyx, geyy, 4);
        SDL_SetRenderDrawColor(r, 4, 3, 0, 255);
        Thick(r, geyx, geyy - 5, geyx, geyy + 5, 2);
        // Eye glow
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        float geg = (sinf((float)tick * 0.14f) + 1.0f) * 0.5f;
        SDL_SetRenderDrawColor(r, 185, 235, 0, (Uint8)(70 + geg * 70));
        Circle(r, geyx, geyy, 12);
        SDL_SetRenderDrawColor(r, 100, 200, 0, (Uint8)(30 + geg * 35));
        Circle(r, geyx, geyy, 16);
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);

        // Flame breath - larger green fire
        float ba2 = (sinf((float)tick * 0.11f + 1.5f) + 1.0f) * 0.5f;
        if (ba2 > 0.32f)
        {
            int gmx = ghdx + wd * 26, gmy = ghdy + 10;
            SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
            int fi;
            // Outer flame
            for (fi = 0; fi < 18; fi++)
            {
                float fa = (float)fi / 18.0f * 1.2f - 0.6f;
                int fl = (int)(22 + ba2 * 32);
                int gfx2 = gmx + (int)(cosf(fa) * fl) * wd;
                int gfy2 = gmy + (int)(sinf(fa) * fl * 0.52f);
                SDL_SetRenderDrawColor(r, (Uint8)(fi * 8), 255, (Uint8)(fi * 6), (Uint8)(150 - fi * 7));
                Thick(r, gmx, gmy, gfx2, gfy2, 2);
            }
            // Inner flame - bright lime
            for (fi = 0; fi < 8; fi++)
            {
                float fa = (float)fi / 8.0f * 0.6f - 0.3f;
                int fl = (int)(14 + ba2 * 22);
                int gfx2 = gmx + (int)(cosf(fa) * fl) * wd;
                int gfy2 = gmy + (int)(sinf(fa) * fl * 0.4f);
                SDL_SetRenderDrawColor(r, 155, 255, 50, (Uint8)(200 - fi * 18));
                Thick(r, gmx, gmy, gfx2, gfy2, 3);
            }
            // Core
            SDL_SetRenderDrawColor(r, 220, 255, 120, 210);
            Circle(r, gmx + wd * 7, gmy, 7);
            SDL_SetRenderDrawColor(r, 245, 255, 210, 165);
            Circle(r, gmx + wd * 5, gmy, 3);
            SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
        }
        HBar(r, cx - 32, y - 20, 64, 9, d->hp, d->maxHp, 88, 215, 28);
    }
}

// ═══════════════════════════════════════════════════════════════
// PICKUPS + COINS
// ═══════════════════════════════════════════════════════════════

void DrawSwordPick(SDL_Renderer *r, float x, float y, int tick, int camX)
{
    float bob = sinf((float)tick * 0.1f) * 3.5f;
    int px = (int)x - camX, py = (int)(y + bob);
    float gl = (sinf((float)tick * 0.13f) + 1.0f) * 0.5f;
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 185, 165, 255, (Uint8)(42 + gl * 52));
    SDL_Rect hl = {px - 7, py - 7, 34, 50};
    SDL_RenderFillRect(r, &hl);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(r, 212, 225, 252, 255);
    Thick(r, px + 9, py, px + 9, py + 22, 3);
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    Thick(r, px + 9, py, px + 9, py + 9, 1);
    SDL_SetRenderDrawColor(r, 192, 155, 30, 255);
    SDL_Rect gd = {px + 2, py + 22, 17, 5};
    SDL_RenderFillRect(r, &gd);
    int gi;
    for (gi = 0; gi < 13; gi++)
    {
        SDL_SetRenderDrawColor(r, gi % 2 ? 102 : 72, gi % 2 ? 60 : 40, gi % 2 ? 18 : 10, 255);
        SDL_Rect gp = {px + 6, py + 27 + gi, 6, 1};
        SDL_RenderFillRect(r, &gp);
    }
    SDL_SetRenderDrawColor(r, 192, 152, 30, 255);
    Circle(r, px + 9, py + 41, 5);
}

void DrawPowerPick(SDL_Renderer *r, float x, float y,
                   int tick, int camX, int idx)
{
    float bob = sinf((float)tick * 0.11f + (float)idx * 1.1f) * 3.5f;
    int px = (int)x - camX, py = (int)(y + bob);
    float gl = (sinf((float)tick * 0.16f + idx) + 1.0f) * 0.5f;
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    int ri;
    for (ri = 22; ri >= 10; ri -= 4)
    {
        SDL_SetRenderDrawColor(r, 175, 90, 252, (Uint8)(10 + gl * 20));
        Circle(r, px, py, ri);
    }
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    int i;
    for (i = 0; i < 10; i++)
    {
        float a1 = (float)i * 3.14159f / 5 - 1.5708f;
        float a2 = (float)(i + 1) * 3.14159f / 5 - 1.5708f;
        int r1 = i % 2 == 0 ? 14 : 6, r2 = (i + 1) % 2 == 0 ? 14 : 6;
        SDL_SetRenderDrawColor(r, 152, 72, 245, 255);
        SDL_RenderDrawLine(r, px + (int)(cosf(a1) * r1), py + (int)(sinf(a1) * r1),
                           px + (int)(cosf(a2) * r2), py + (int)(sinf(a2) * r2));
    }
    SDL_SetRenderDrawColor(r, 215, 152, 252, 255);
    Circle(r, px, py, 6);
    SDL_SetRenderDrawColor(r, 252, 235, 252, 255);
    Circle(r, px, py, 3);
}

void DrawHealthPick(SDL_Renderer *r, float x, float y,
                    int tick, int camX)
{
    float bob = sinf((float)tick * 0.12f) * 3.0f;
    int px = (int)x - camX, py = (int)(y + bob);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    float gl = (sinf((float)tick * 0.15f) + 1.0f) * 0.5f;
    SDL_SetRenderDrawColor(r, 255, 80, 80, (Uint8)(40 + gl * 50));
    Circle(r, px, py, 20);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(r, 220, 40, 40, 255);
    SDL_Rect h1 = {px - 3, py - 10, 6, 20};
    SDL_RenderFillRect(r, &h1);
    SDL_Rect h2 = {px - 10, py - 3, 20, 6};
    SDL_RenderFillRect(r, &h2);
    SDL_SetRenderDrawColor(r, 255, 150, 150, 255);
    SDL_Rect h3 = {px - 2, py - 9, 3, 8};
    SDL_RenderFillRect(r, &h3);
}

void DrawCoinObj(SDL_Renderer *r, float x, float y,
                 int tick, int camX)
{
    float bob = sinf((float)tick * 0.14f) * 3.0f;
    int px = (int)x - camX, py = (int)(y + bob);
    SDL_SetRenderDrawColor(r, 255, 208, 32, 255);
    Circle(r, px, py, 7);
    SDL_SetRenderDrawColor(r, 255, 232, 100, 255);
    Circle(r, px, py, 5);
    SDL_SetRenderDrawColor(r, 255, 245, 160, 255);
    SDL_RenderDrawPoint(r, px - 1, py - 1);
    SDL_SetRenderDrawColor(r, 205, 158, 10, 255);
    int a;
    for (a = 0; a < 360; a += 45)
    {
        float an = (float)a * 3.14f / 180;
        SDL_RenderDrawPoint(r, px + (int)(cosf(an) * 7), py + (int)(sinf(an) * 7));
    }
}

void DrawPowerball(SDL_Renderer *r, float x, float y,
                   float vx, int camX, int tick)
{
    (void)tick;
    int bx = (int)x - camX, by = (int)y;
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    int tr;
    for (tr = 1; tr <= 5; tr++)
    {
        SDL_SetRenderDrawColor(r, 148, 55, 245, (Uint8)(55 - tr * 9));
        int tsz = 16 - tr * 2;
        if (tsz < 4)
            tsz = 4;
        int trOff = (int)(vx < 0 ? tr * 9 : -tr * 9);
        SDL_Rect tR = {bx + trOff - tsz / 2, by - tsz / 2, tsz, tsz};
        SDL_RenderFillRect(r, &tR);
    }
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    int si;
    for (si = 0; si < 10; si++)
    {
        float a1 = (float)si * 3.14159f / 5 - 1.5708f;
        float a2 = (float)(si + 1) * 3.14159f / 5 - 1.5708f;
        int r1 = si % 2 == 0 ? 14 : 6, r2 = (si + 1) % 2 == 0 ? 14 : 6;
        SDL_SetRenderDrawColor(r, 168, 85, 252, 255);
        SDL_RenderDrawLine(r, bx + 12 + (int)(cosf(a1) * r1), by + 12 + (int)(sinf(a1) * r1),
                           bx + 12 + (int)(cosf(a2) * r2), by + 12 + (int)(sinf(a2) * r2));
    }
    SDL_SetRenderDrawColor(r, 218, 158, 252, 255);
    Circle(r, bx + 12, by + 12, 7);
    SDL_SetRenderDrawColor(r, 252, 238, 252, 255);
    Circle(r, bx + 12, by + 12, 3);
}