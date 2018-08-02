#ifndef BOSSSPRITE__H
#define BOSSSPRITE__H

#include <iostream>
#include <string>
#include <list>
#include <cmath>
#include "multibullets.h"

class BossSprite : public MultiSprite {
public:
  BossSprite(const std::string&);
  BossSprite(const BossSprite&);
  virtual ~BossSprite() {};
  virtual void update(Uint32 ticks);
  virtual void draw() const;

  BossSprite& operator=(const BossSprite&);

  unsigned int bulletCount() const { return bullets.bulletCount(); }
  unsigned int freeCount() const { return bullets.freeCount(); }

  void shoot();

  void decreaseLife();
  int getLife();

  bool bulletCollided(const Drawable* it)  { return bullets.collided(it); }
private:
  Vector2f initialVelocity;
  int life;
  std::string bulletName;
  MultiBullets bullets;
  float minBulletSpeed;
  float bulletInterval;
  float timeSinceLastBullet;
  bool isAlive = true;
};
#endif

