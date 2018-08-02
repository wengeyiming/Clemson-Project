#ifndef SMARTSPRITE__H
#define SMARTSPRITE__H
#include <string>
#include "multisprite.h"


class SmartSprite : public MultiSprite {
public:
  SmartSprite(const std::string&, const Vector2f& pos, int w, int h);
  SmartSprite(const SmartSprite&);
  ~SmartSprite();

  void turnLeft() { toRight = false; }
  void turnRight() { toRight = true; }
  virtual void update(Uint32 ticks);
  void setPlayerPos(const Vector2f& p) { playerPos = p; }
private:
  enum MODE {NORMAL, EVADE};
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  MODE currentMode;
  float safeDistance;
  std::vector<Image*> goLeftImages;
  std::vector<Image*> goRightImages;

  void goLeft();
  void goRight();
  void goUp();
  void goDown();
  Vector2f makeVelocity(int, int);
  bool toRight = false;
  
};
#endif
