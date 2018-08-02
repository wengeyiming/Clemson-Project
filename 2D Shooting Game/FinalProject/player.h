#ifndef PLAYER__H
#define PLAYER__H

#include <iostream>
#include <string>
#include <list>
#include <cmath>
#include "multibullets.h"

class SmartSprite;
// In this example the player is derived from MultiSprite.
// However, there are many options.
class Player : public MultiSprite {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual ~Player() {};
  virtual void update(Uint32 ticks);
  virtual void draw() const;

  Player& operator=(const Player&);

  void right();
  void left();
  void up();
  void down();
  void stop();

  unsigned int bulletCount() const { return bullets.bulletCount(); }
  unsigned int freeCount() const { return bullets.freeCount(); }

  void shoot();

  void turnLeft() { toRight = false; }
  void turnRight() { toRight = true; }

  void collided() { collision = true; }
  void missed() { collision = false; }

  void attach( SmartSprite* o ) { observers.push_back(o); } 
  void detach( SmartSprite* o );

  bool bulletCollided(const Drawable* it)  { return bullets.collided(it); }

  void changeGodMode() { godMode = !godMode; }

  bool getGodMode() { return godMode; }
 
protected:
  std::list<SmartSprite*> observers;
private:
  bool collision;
  Vector2f initialVelocity;

  std::vector<Image*> goLeftImages;
  std::vector<Image*> goRightImages;
  bool toRight = true;

  std::string bulletName;
  MultiBullets bullets;
  float minBulletSpeed;
  float bulletInterval;
  float timeSinceLastBullet;

  bool godMode = false;
};
#endif
