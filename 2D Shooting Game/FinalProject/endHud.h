#ifndef ENDHUD__H
#define ENDHUD__H

#include <SDL.h>
#include "gamedata.h"

class EndHud {
public:
  static EndHud& getInstance();
  void closeInstructions() const;
  void showVictoryInstructions(SDL_Renderer* ) const;
  void showLostInstructions(SDL_Renderer* ) const;
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
  EndHud();
  EndHud(const EndHud&);
  EndHud& operator=(const EndHud&);
};
#endif
