
#include "Ray.hpp"


Ray::Ray() {
    
}

Ray::Ray(Vector3f start, Vector3f dir) {
    startPosition = start;
    direction = dir;
}

Vector3f Ray::getDirection() {
    return direction;
}

void Ray::setDirection(Vector3f dir) {
    direction = dir;
}

Vector3f Ray::getStartPosition() {
    return startPosition;
}

void Ray::setStartPosition(Vector3f start) {
    startPosition = start;
}

Vector3f Ray::rayPoint(float t) {
    return startPosition + direction*t;
}