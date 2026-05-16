#include "../include/game.h"

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    Game g;

    if (!Game_Init(&g))
    {
        printf("Game init failed!\n");
        return 1;
    }

    Game_Run(&g);
    Game_Cleanup(&g);

    return 0;
}