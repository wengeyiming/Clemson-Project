

#include "Vector3f.hpp"


Vector3f::Vector3f(){
    
}

Vector3f::Vector3f(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3f Vector3f::operator+(const Vector3f v) {
    return Vector3f(x+v.x,y+v.y,z+v.z);
}

Vector3f Vector3f::operator-(const Vector3f v) {
    return Vector3f(x-v.x,y-v.y,z-v.z);
}

Vector3f Vector3f::operator*(float f){
    return Vector3f(x*f,y*f,z*f);
}

Vector3f Vector3f::operator/(float f){
    return Vector3f(x/f,y/f,z/f);
}

float Vector3f::vectorDotProduct(const Vector3f v) {
    return (x*v.x+y*v.y+z*v.z);
}

Vector3f Vector3f::vectorCrossProduct(const Vector3f v) {
    return Vector3f(((y * v.z) - (z * v.y)),((z * v.x) - (x * v.z)),((x * v.y) - (y * v.x)));
}

Vector3f Vector3f::abs() {
    return Vector3f(x>0?x:-x,y>0?y:-y,z>0?z:-z);
}

float Vector3f::vectorLength() {
    return (float)sqrt(x*x+y*y+z*z);
}

Vector3f Vector3f::normalizeVector() {
    return Vector3f(x/vectorLength(),y/vectorLength(),z/vectorLength());
}

float Vector3f::getDistanceBetweenTwoVector(const Vector3f v) {
    return sqrt((x-v.x)*(x-v.x)+(y-v.y)*(y-v.y)+(z-v.z)*(z-v.z));
}



