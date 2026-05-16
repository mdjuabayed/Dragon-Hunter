#include "../include/platform.h"

void Platform_InitLevel(Platform plats[], int level, int camX)
{
    (void)level;

    // Ground - always index 0
    plats[0].x = camX - 300;
    plats[0].y = GROUND_Y;
    plats[0].w = WIDTH + 600;
    plats[0].h = 50;
    plats[0].type = 0;

    // Fixed starter platforms
    static const int pd[][4] = {
        {200, 460, 130, 1}, {390, 400, 120, 1}, {560, 460, 110, 1}, {720, 380, 130, 1}, {900, 450, 120, 1}, {1060, 370, 110, 1}, {1220, 440, 130, 1}, {1400, 360, 120, 1}, {1560, 440, 110, 1}, {1730, 370, 130, 1}, {1900, 450, 120, 1}, {2060, 360, 110, 1}, {2230, 430, 130, 1}, {2400, 355, 120, 1}, {2560, 445, 110, 1}};
    int i;
    for (i = 0; i < MAX_PLATFORMS - 1; i++)
    {
        plats[i + 1].x = pd[i][0];
        plats[i + 1].y = pd[i][1];
        plats[i + 1].w = pd[i][2];
        plats[i + 1].h = 20;
        plats[i + 1].type = pd[i][3];
    }
}

void Platform_Recycle(Platform plats[], int camX)
{
    // Keep ground updated
    plats[0].x = camX - 300;
    plats[0].w = WIDTH + 600;

    int i, j;
    for (i = 1; i < MAX_PLATFORMS; i++)
    {
        if (plats[i].x + plats[i].w >= camX - 200)
            continue;

        // Find rightmost platform
        int maxX = 0;
        for (j = 1; j < MAX_PLATFORMS; j++)
            if (plats[j].x > maxX)
                maxX = plats[j].x;

        int prevY = GROUND_Y - 60;
        for (j = 1; j < MAX_PLATFORMS; j++)
            if (plats[j].x == maxX)
                prevY = plats[j].y;

        int newGap = 155 + rand() % 70;
        int newY = prevY + (rand() % 3 - 1) * 55;
        if (newY < 260)
            newY = 260;
        if (newY > GROUND_Y - 65)
            newY = GROUND_Y - 65;

        plats[i].x = maxX + newGap;
        plats[i].y = newY;
        plats[i].w = 100 + rand() % 85;
        plats[i].h = 20;
        plats[i].type = 1;
    }
}