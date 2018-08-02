#include <iostream>
#include <cmath>
#include "multibullet.h"
#include "imageFactory.h"

MultiBullet::MultiBullet ( const std::string& name, const Vector2f& pos, const Vector2f& vel) :
	MultiSprite(name, pos, vel, ImageFactory::getInstance().getImages(name)),
	distance(0),
	maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")),
	tooFar(false)
 {}



MultiBullet::MultiBullet( const MultiBullet& mb) : 
	MultiSprite(mb),
	distance(mb.distance),
	maxDistance(mb.maxDistance),
	tooFar(mb.tooFar)
 {}

void MultiBullet::reset() {
	tooFar = false;
	distance = 0;
}

void MultiBullet::update(Uint32 ticks) { 
  Vector2f pos = getPosition();
  MultiSprite::update(ticks);
  if( getY() < 0 || getY()+getScaledHeight() > worldHeight ) {
  	tooFar = true;
  } if( getX() < 0 || getX()+getScaledWidth() > worldWidth ) {
  	tooFar = true;
  } 

  distance += ( hypot(getX()-pos[0], getY()-pos[1]) );
  if(distance > maxDistance) {
  	tooFar = true;
  }
}

