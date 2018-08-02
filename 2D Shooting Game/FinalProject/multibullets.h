#ifndef MULTIBULLETS__H
#define MULTIBULLETS__H

#include <list>
#include "multibullet.h"


class CollisionStrategy;

class MultiBullets {
  MultiBullets& operator=(const MultiBullets&)=delete;
public:
  MultiBullets(const std::string&);
  MultiBullets(const MultiBullets&);
  ~MultiBullets();

  void draw() const;
  void update(Uint32 ticks);
  void shoot(const Vector2f& pos, const Vector2f& objVel);

  unsigned int bulletCount() const { return bulletList.size(); }
  unsigned int freeCount() const { return freeList.size(); }
  bool shooting() { return !bulletList.empty(); }
  bool collided(const Drawable*) ; 

  
private:
  std::string name;
  Vector2f myVelocity;
  std::vector<Image*> bulletImages;
  std::list<MultiBullet> bulletList;
  std::list<MultiBullet> freeList; 
  CollisionStrategy* strategy;

};

#endif