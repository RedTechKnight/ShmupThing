#include "../headers/game.h"

Game shmup{Game()};
int main() {
  uint32_t last_update = 0;
  uint32_t update_interval = 5;
  shmup.game_running = true;
  while (shmup.game_running) {
    if ((SDL_GetTicks() - last_update) > update_interval) {
      shmup.start();
      shmup.init();
      shmup.update_interface(SDL_GetTicks() - last_update);
      shmup.update(SDL_GetTicks() - last_update);
      shmup.render();

      last_update = SDL_GetTicks();
    } else {
      SDL_Delay(1);
    }
  }
  return 0;
}
