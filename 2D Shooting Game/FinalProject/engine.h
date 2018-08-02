#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "poolhud.h"
#include "endHud.h"

class CollisionStrategy;
class SmartSprite;
class Player;
class LifeSprite;
class MultiSprite;
class BossSprite;
class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
private:
  const RenderContext* rc;
  const IoMod& io;
  const Hud& hud;
  const EndHud& endHud;

  Clock& clock;

  SDL_Renderer * const renderer;
  World background;
  World foreground;
  Viewport& viewport;
  Player* player;
  BossSprite* boss;
  PoolHud poolhud;
  std::vector<SmartSprite*> spritesVector;
  std::vector<CollisionStrategy*> strategiesVector;
  std::vector<LifeSprite*> lifeVector;
  std::vector<MultiSprite*> coinVector;
  std::vector<SmartSprite*> backgroundVector;
  int currentStrategy = 0;
  bool isLost = false;
  bool isWin = false;
  bool collision;
  bool makeVideo;
  void draw() const;
  void update(Uint32);

  void checkForCollisions();
};
