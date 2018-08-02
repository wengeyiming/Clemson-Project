#ifndef FORMALSPRITE__H
#define FORMALSPRITE__H
#include <string>
#include "drawable.h"

class FormalSprite : public Drawable {
public:
  FormalSprite(const std::string&);
  FormalSprite(const std::string&, const Vector2f& pos, const Vector2f& vel, 
         const Image*);
  FormalSprite(const FormalSprite&);

  ~FormalSprite() {}
  FormalSprite& operator=(const FormalSprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { return image; }
  virtual const SDL_Surface* getSurface() const { 
    return image->getSurface();
  }
  int getScaledWidth()  const { return getScale()*image->getWidth();  } 
  int getScaledHeight() const { return getScale()*image->getHeight(); } 
private:
  const Image * image;
  
protected:
  int worldWidth;
  int worldHeight;

  int getDistance(const FormalSprite*) const;
  Vector2f makeVelocity(int, int) const;
};
#endif
