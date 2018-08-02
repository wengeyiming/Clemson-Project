
#include "Camera.hpp"

#include <math.h>

Camera::Camera(){
    
}

Camera::Camera(const Vector3f& eyes) {
    eyePosition = eyes;
}


Ray Camera::generateRays(float x, float y, float z) {
    Vector3f direction(x,y,z);
    Vector3f vec = (direction.operator-(eyePosition)).normalizeVector();
    return Ray(eyePosition, vec);
}

void Camera::getCameraInfo() {
    cout << "eyePosition: " << eyePosition.x << eyePosition.y << eyePosition.z;
}