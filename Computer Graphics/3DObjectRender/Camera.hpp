

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include "Ray.hpp"


class Camera {
public:
    Camera();
    Camera(const Vector3f& eyes);
    Ray generateRays(float x, float y, float z);
    void getCameraInfo();
private:
    Vector3f eyePosition;
};

#endif /* Camera_hpp */
