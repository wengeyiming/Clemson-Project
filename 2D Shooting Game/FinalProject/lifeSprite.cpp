#include <cmath>
#include <random>
#include <functional>
#include "lifeSprite.h"
#include "gamedata.h"
#include "renderContext.h"


LifeSprite::LifeSprite(const std::string& name, const Vector2f& pos, const Vector2f& vel, const Vector2f& pPos,
  int w, int h) :
  FormalSprite(name, pos, vel, RenderContext::getInstance()->getImage(name)),
  playerPos(pPos),
  playerWidth(w),
  playerHeight(h)
{}



void LifeSprite::update(Uint32 ticks, const Vector2f& pos, int number) { 
  FormalSprite::update(ticks);
  playerPos[0] = pos[0];
  playerPos[1] = pos[1];
  float ex= playerPos[0];
  float ey= playerPos[1]-playerHeight/2;
  setPosition(Vector2f(ex+number*getScaledWidth(), ey));
}

