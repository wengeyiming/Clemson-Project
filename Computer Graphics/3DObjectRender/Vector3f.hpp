
#ifndef Vector3f_hpp
#define Vector3f_hpp

#include <stdio.h>
#include <iostream>
#include <cmath>

using namespace std;


#define MIN(x,y) (x)>(y)?y:x;
#define MAX(x,y) (x)>(y)?x:y;

class Vector3f {
public:
    float x;
    float y;
    float z;
    
    Vector3f();
    Vector3f(float x, float y, float z);
    
    Vector3f operator+(const Vector3f v);
    Vector3f operator-(const Vector3f v);
    Vector3f operator*(float f);
    Vector3f operator/(float f);
    
    float vectorDotProduct(const Vector3f v);
    
    Vector3f vectorCrossProduct(const Vector3f v);
    
    Vector3f abs();
    
    float vectorLength();
    
    Vector3f normalizeVector();
    
    float getDistanceBetweenTwoVector(const Vector3f v);
    
    
};


#endif /* Vector3f_hpp */
