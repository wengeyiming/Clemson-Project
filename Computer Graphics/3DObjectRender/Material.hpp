

#ifndef Material_hpp
#define Material_hpp

#include <stdio.h>
#include "RGBColor.hpp"
#include "Ray.hpp"
#include "Vector3f.hpp"


using namespace std;

static Vector3f lightDirection = Vector3f(0, 0, 1).normalizeVector();
//static RGBColor lightColor = RGBColor::white();

class Material {
public:
    Material();
    Material(RGBColor diff, RGBColor spec, float shine);
    virtual RGBColor intersect(Ray& ray, Vector3f& hitPosition, Vector3f& normal);
    
private:
    RGBColor ambient;
    RGBColor specular;
    RGBColor diffuse;
    float shiness;
};


#endif /* Material_hpp */
