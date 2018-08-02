
#include "Material.hpp"

Material::Material() {
    
}

Material::Material(RGBColor diff, RGBColor spec, float shine){
    specular = spec;
    diffuse = diff;
    shiness = shine;
}

RGBColor Material::intersect(Ray &ray, Vector3f& hitPosition, Vector3f& normal) {
    
    //const float k=1e-4;
    //cout << "hitPosition:  " << hitPosition.x << endl;
    //cout << "normal:  " << normal.x << endl;
    float NdotL = normal.vectorDotProduct(lightDirection);
    Vector3f v = (lightDirection.operator-(ray.getDirection())).normalizeVector();
    float NdotH = normal.vectorDotProduct(v);
    RGBColor diff = diffuse.mulColor(abs(NdotL));
    RGBColor spec = specular.mulColor(pow(abs(NdotH),shiness));
    return diff.addColor(spec);
}