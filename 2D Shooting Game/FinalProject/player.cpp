#include <iostream>
#include <cmath>
#include "player.h"
#include "smartSprite.h"
#include "gamedata.h"
#include "imageFactory.h"


Player::Player( const std::string& name) :
  MultiSprite(name),
  observers(),
  collision(false),
  initialVelocity(getVelocity()),
  goLeftImages( ImageFactory::getInstance().getImages(name+"Left") ),
  goRightImages( ImageFactory::getInstance().getImages(name) ),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  bullets(bulletName),
  minBulletSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") ),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastBullet(0)
{ }

Player::Player(const Player& s) :
  MultiSprite(s), 
  observers(s.observers),
  collision(s.collision),
  initialVelocity(s.getVelocity()),
  goLeftImages(s.goLeftImages),
  goRightImages(s.goRightImages),
  bulletName(s.bulletName),
  bullets(s.bullets),
  minBulletSpeed(s.minBulletSpeed),
  bulletInterval(s.bulletInterval),
  timeSinceLastBullet(s.timeSinceLastBullet)
  { }

Player& Player::operator=(const Player& s) {
  MultiSprite::operator=(s);
  collision = s.collision;
  initialVelocity = s.initialVelocity;

  goLeftImages = s.goLeftImages;
  goRightImages = s.goRightImages;
  return *this;
}

void Player::stop() { 
  setVelocityX( 0.93*getVelocityX() );
  setVelocityY(0);
}

void Player::right() { 
  images = goRightImages;
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);

  }
} 
void Player::left()  { 
  images = goLeftImages;
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
  }
} 
void Player::up()    { 
  //std::cout << "upup!  " << getVelocityY() << std::endl;
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
  }
} 
void Player::down()  { 
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

void Player::shoot() { 
  if ( timeSinceLastBullet < bulletInterval ) {
    return;
  }


  Vector2f vel = getVelocity();
  float x;
  float y = getY() + getScaledHeight()/10;
  if(vel[0] > 0) {
    x = getX() + getScaledWidth();
    vel[0] += minBulletSpeed;
  } else if(vel[0] < 0){
    x = getX();
    vel[0] -= minBulletSpeed;  
  }

  bullets.shoot(Vector2f(x,y),vel);
  timeSinceLastBullet = 0;
}

void Player::draw() const { 
  MultiSprite::draw();
  bullets.draw();
}


void Player::update(Uint32 ticks) {
  if ( !collision ) { MultiSprite::update(ticks); }
  
  timeSinceLastBullet += ticks;
  bullets.update(ticks);
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
  }
  stop();
}



void Player::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

