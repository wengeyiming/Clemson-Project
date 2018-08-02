
#ifndef TSample_hpp
#define TSample_hpp

#include <stdio.h>
#include "Ray.hpp"
#include "Camera.hpp"
#include "RGBColor.hpp"
#include "Vector3f.hpp"
#include "Material.hpp"
#include "ObjAttribute.h"

class TSample {
public:
    TSample();
    TSample(CVertex cvertex1, CVertex cvertex2, CVertex cvertex3, Vector3f normals);
    RGBColor calculateColor(Camera camera, CVertex c);
    RGBColor smoothShadingColor(Camera camera, IntersectResult result);
    RGBColor bumpMappingColor(Camera camera, IntersectResult result);
    Vector3f getNormal();
    IntersectResult isIntersect(Ray ray);
    
private:
    RGBColor color;
    CVertex c1;
    CVertex c2;
    CVertex c3;
    Vector3f normal;
};



#endif /* TSample_hpp */
