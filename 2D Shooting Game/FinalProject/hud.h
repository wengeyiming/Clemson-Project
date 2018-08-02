#ifndef HUD__H
#define HUD__H

#include <SDL.h>
#include "gamedata.h"

class Hud {
public:
  static Hud& getInstance();
  void closeInstructions() const;
  void showInstructions(SDL_Renderer* ) const;
  void draw() const;
private:

  Uint8 red;
  Uint8 green;
  Uint8 blue;
  Uint8 alpha;
  int positionX;
  int positionY;
  int width;
  int height;
  SDL_Rect rect;
  Hud();
  Hud(const Hud&);
  Hud& operator=(const Hud&);
};
#endif
