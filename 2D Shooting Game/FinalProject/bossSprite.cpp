#include <iostream>
#include <cmath>
#include "bossSprite.h"
#include "gamedata.h"
#include "imageFactory.h"


BossSprite::BossSprite( const std::string& name) :
  MultiSprite(name),
  initialVelocity(getVelocity()),
  life(Gamedata::getInstance().getXmlInt(name+"/life")),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bossBullet") ),
  bullets(bulletName),
  minBulletSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") ),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastBullet(0)
{ }

BossSprite::BossSprite(const BossSprite& s) :
  MultiSprite(s), 
  initialVelocity(s.getVelocity()),
  life(s.life),
  bulletName(s.bulletName),
  bullets(s.bullets),
  minBulletSpeed(s.minBulletSpeed),
  bulletInterval(s.bulletInterval),
  timeSinceLastBullet(s.timeSinceLastBullet)
  { }

BossSprite& BossSprite::operator=(const BossSprite& s) {
  MultiSprite::operator=(s);
  initialVelocity = s.initialVelocity;
  return *this;
}

void BossSprite::decreaseLife() { 
  life--;
  if(life <=0 ) {
    setVelocityX(400);
    setVelocityY(0);
  }
}

int BossSprite::getLife() { 
  return life;
}


void BossSprite::shoot() { 
    if ( timeSinceLastBullet < bulletInterval) {
      return;
    }

    if(rand()%30==0) {
    Vector2f vel = getVelocity();
    float x;
    float y = getY() + getScaledHeight()/10;
    if(vel[0] > 0) {
      x = getX() + getScaledWidth();
      vel[0] += minBulletSpeed;
    } else if(vel[0] <= 0){
      x = getX();
      vel[0] -= minBulletSpeed;  
      vel[1] = rand()%100;
    }

    bullets.shoot(Vector2f(x,y),vel);
    timeSinceLastBullet = 0;
  }

}

void BossSprite::draw() const { 
  MultiSprite::draw();
  bullets.draw();
}


void BossSprite::update(Uint32 ticks) {

  advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }
  timeSinceLastBullet += ticks;
  bullets.update(ticks);
}

