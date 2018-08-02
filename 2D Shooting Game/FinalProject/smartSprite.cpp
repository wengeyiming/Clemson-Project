#include <cmath>
#include <random>
#include <functional>
#include "smartSprite.h"
#include "gamedata.h"
#include "renderContext.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

void SmartSprite::goLeft()  { 
  setVelocityX( -fabs(getVelocityX()) ); 
  images = goLeftImages; 
}
void SmartSprite::goRight() { 
  setVelocityX( fabs(getVelocityX()) );  
  images = goRightImages;
}
void SmartSprite::goUp()    { setVelocityY( -fabs(getVelocityY()) ); }
void SmartSprite::goDown()  { setVelocityY( fabs(getVelocityY()) );  }

Vector2f SmartSprite::makeVelocity(int vx, int vy)  {
  float newvx = Gamedata::getInstance().getRandFloat(vx-50,vx+50);;
  float newvy = Gamedata::getInstance().getRandFloat(vy-50,vy+50);;
  newvx *= [](){ if(rand()%2) return -1; else return 1; }();
  newvy *= [](){ if(rand()%2) return -1; else return 1; }();

  if(newvx > 0) { turnRight(); } else { turnLeft(); }
  return Vector2f(newvx, newvy);
}


SmartSprite::SmartSprite(const std::string& name, const Vector2f& pos, 
  int w, int h) :
  MultiSprite(name),
  playerPos(pos),
  playerWidth(w),
  playerHeight(h),
  currentMode(NORMAL),
  safeDistance(Gamedata::getInstance().getXmlFloat(name+"/safeDistance")),
  goLeftImages( ImageFactory::getInstance().getImages(name+"Left") ),
  goRightImages( ImageFactory::getInstance().getImages(name) )
{
  Vector2f initialVelocity = makeVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
  Gamedata::getInstance().getXmlInt(name+"/speedY"));
  setVelocityX(initialVelocity[0]);
  setVelocityY(initialVelocity[1]);

  if(toRight) { images = goRightImages; }
  else { images= goLeftImages; }
}


SmartSprite::SmartSprite(const SmartSprite& s) : 
  MultiSprite(s),
  playerPos(s.playerPos),
  playerWidth(s.playerWidth),
  playerHeight(s.playerHeight),
  currentMode(s.currentMode),
  safeDistance(s.safeDistance),
  goLeftImages(s.goLeftImages),
  goRightImages(s.goRightImages)
{
  if(toRight) { images = goRightImages; }
  else { images= goLeftImages; }
}


SmartSprite::~SmartSprite( ) { 
}


void SmartSprite::update(Uint32 ticks) { 
  MultiSprite::update(ticks);
  float x= getX()+getImage()->getWidth()/2;
  float y= getY()+getImage()->getHeight()/2;
  float ex= playerPos[0]+playerWidth/2;
  float ey= playerPos[1]+playerHeight/2;
  float distanceToEnemy = ::distance( x, y, ex, ey );
  if  ( currentMode == NORMAL ) {
    if(distanceToEnemy < safeDistance) currentMode = EVADE;
  }
  else if  ( currentMode == EVADE ) {
    if(distanceToEnemy > safeDistance) currentMode=NORMAL;
    else {
      if ( x < ex ) goLeft();
      if ( x > ex ) goRight();
      if ( y < ey ) goUp();
      if ( y > ey ) goDown();
    }
  }

  if ( getX() < 0) {
     goRight();
  }
  if ( getX() > worldWidth-getScaledWidth()) {
     goLeft();
  }  

}
