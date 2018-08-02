#include <iostream>
#include <cmath>

#include "chunk.h"

void Chunk::update(Uint32 ticks) { 
  float yincr = getVelocityY() * static_cast<float>(ticks) * 0.005;
  setY( getY()- yincr );
  float xincr = getVelocityX() * static_cast<float>(ticks) * 0.005;
  setX( getX()- xincr );
  distance += ( hypot(xincr, yincr) );
  //std::cout << maxDistance << std::endl;
  if (distance > maxDistance) tooFar = true;
}

