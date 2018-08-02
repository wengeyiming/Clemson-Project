#ifndef POOLHUD__H
#define POOLHUD__H

#include <string>
#include <SDL.h>
#include "player.h"
#include "gamedata.h"


class PoolHud {
public:
  PoolHud(const Player&);
  void draw() const;
  void update();
  void toggle() { showPoolHud = !showPoolHud; }
  PoolHud(const PoolHud&) = delete;
  PoolHud& operator=(const PoolHud&) = delete;
private:

  const Player& player;
  SDL_Renderer* renderer;
  SDL_Rect hudFrame;
  bool showPoolHud;
};

#endif
