#include "Game.h"
/*
TODO:
- Improve logic in projectile handler class
*/
Game shmup{Game()};
int main()
{
    uint32_t lastUpdate = 0;
    uint32_t updateInterval = 5;
    shmup.mGameRunning = true;
    while(shmup.mGameRunning)
    {
        if((SDL_GetTicks() - lastUpdate) > updateInterval)
        {
            shmup.start();
            shmup.init();
            shmup.updateInterface(SDL_GetTicks() - lastUpdate);
            shmup.update(SDL_GetTicks() - lastUpdate);
            shmup.renderInterface();
            shmup.render();

            lastUpdate = SDL_GetTicks();
        }
        else
        {
            SDL_Delay(1);
        }
    }
    return 0;
}
