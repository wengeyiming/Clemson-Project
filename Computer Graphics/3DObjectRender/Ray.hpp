
#ifndef Ray_hpp
#define Ray_hpp

#include <stdio.h>
#include "Vector3f.hpp"
using namespace std;

class Ray {
public:
    Ray();
    
    Ray(Vector3f start, Vector3f dir);
    
    Vector3f getDirection();
    
    void setDirection(Vector3f dir);
    
    Vector3f getStartPosition();
    
    void setStartPosition(Vector3f start);
    
    Vector3f rayPoint(float t);
    
    
private:
    Vector3f direction;
    Vector3f startPosition;
};

#endif /* Ray_hpp */
