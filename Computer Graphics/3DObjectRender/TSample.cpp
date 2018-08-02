
#include "TSample.hpp"

TSample::TSample() {
    
}

TSample::TSample(CVertex cvertex1, CVertex cvertex2, CVertex cvertex3, Vector3f normals) {
    c1 = cvertex1;
    c2 = cvertex2;
    c3 = cvertex3;
    normal = normals;
}

RGBColor TSample::calculateColor(Camera camera, CVertex c) {
    Ray ray = camera.generateRays(c1.vertexPoint.x, c1.vertexPoint.y, c1.vertexPoint.z);
    Material material(RGBColor::green(),RGBColor::white(), 5);
    RGBColor showColor =  material.intersect(ray, c1.vertexPoint, normal);
    return showColor;
}



RGBColor TSample::smoothShadingColor(Camera camera, IntersectResult result) {
    Vector3f cnormal1 = c1.vertexNormal;
    Vector3f cnormal2 = c2.vertexNormal;
    Vector3f cnormal3 = c3.vertexNormal;
    Vector3f p = result.intersectPosition;
    float u = result.u;
    float v = result.v;
    //Vector3f hitNormal = cnormal1.operator*(1-u-v) + cnormal2.operator*(u) + cnormal3.operator*(v);
    
    //cout << "u,v:  " << u << "," << v << ","  << endl;
    Vector3f hitNormal= cnormal1.operator*(u).operator+(cnormal2.operator*(v)).operator+(cnormal3.operator*(1-u-v));
    hitNormal.normalizeVector();
    //cout << "hitnormal:  " << hitNormal.x << "," << hitNormal.y << "," << hitNormal.z << endl;
    
    Ray ray = camera.generateRays(p.x, p.y, p.z);
    Material material(RGBColor::green(),RGBColor::white(), 5);
    RGBColor showColor =  material.intersect(ray, p, hitNormal);
    return showColor;
}


RGBColor TSample::bumpMappingColor(Camera camera, IntersectResult result) {
    Vector3f cnormal1 = c1.vertexNormal;
    Vector3f cnormal2 = c2.vertexNormal;
    Vector3f cnormal3 = c3.vertexNormal;
    Vector3f p = result.intersectPosition;
    float u = result.u;
    float v = result.v;
    Vector3f hitNormal= cnormal1.operator*(u).operator+(cnormal2.operator*(v)).operator+(cnormal3.operator*(1-u-v));
    hitNormal.normalizeVector();
    
    if(abs((int)p.x)% 5 <= 2 && abs((int)p.y)% 5 <= 2) {
        hitNormal = hitNormal.operator*(1.1);
    }
    Ray ray = camera.generateRays(p.x, p.y, p.z);
    Material material(RGBColor::green(),RGBColor::white(), 5);
    RGBColor showColor =  material.intersect(ray, p, hitNormal);
    return showColor;
}

IntersectResult TSample::isIntersect(Ray ray) {
    /*
    cout << c1.vertexNormal.x << "  " << c1.vertexNormal.y << "  " << c3.vertexNormal.z <<  "/  ";
    cout << c2.vertexNormal.x << "  " << c2.vertexNormal.y << "  " << c2.vertexNormal.z <<  "/  ";
    cout << c3.vertexNormal.x << "  " << c3.vertexNormal.y << "  " << c3.vertexNormal.z <<  endl;
    */
    IntersectResult result = IntersectResult::initialIntersectResult();
    float rayDirection = normal.vectorDotProduct(ray.getDirection());
    if(fabs(rayDirection) < 0.00001) {
        return result;
    }
    float D = -normal.vectorDotProduct(c1.vertexPoint);
    float t = -(normal.vectorDotProduct(ray.getStartPosition()) + D) / rayDirection;
    if (t < 0) return result;
    Vector3f P = ray.rayPoint(t);
    result.intersectPosition = P;
    Vector3f C;
    Vector3f edge0 = c2.vertexPoint.operator-(c1.vertexPoint);
    Vector3f vp0 = P.operator-(c1.vertexPoint);
    C = edge0.vectorCrossProduct(vp0);
    if(normal.vectorDotProduct(C) < 0) {
        return result;
    }
    Vector3f edge1 = c3.vertexPoint.operator-(c2.vertexPoint);
    Vector3f vp1 = P.operator-(c2.vertexPoint);
    C = edge1.vectorCrossProduct(vp1);
    if(normal.vectorDotProduct(C) < 0) {
        return result;
    }
    Vector3f edge2 = c1.vertexPoint.operator-(c3.vertexPoint);
    Vector3f vp2 = P.operator-(c3.vertexPoint);
    C = edge2.vectorCrossProduct(vp2);
    if(normal.vectorDotProduct(C) < 0) {
        return result;
    }
    /*
    Vector3f cnormal1 = c1.vertexNormal;
    float sum = cnormal1.vectorDotProduct(cnormal1);
    result.u = cnormal1.vectorDotProduct(vp0)/sum;
    result.v = cnormal1.vectorDotProduct(vp2)/sum;
    */
    
    Vector3f c12 = c1.vertexPoint.operator-(c2.vertexPoint);
    Vector3f c13 = c1.vertexPoint.operator-(c3.vertexPoint);
    Vector3f normalNotNormalized = c12.vectorCrossProduct(c13);
    
    float sum = normalNotNormalized.vectorDotProduct(normalNotNormalized);
    result.u = normalNotNormalized.vectorDotProduct(edge1.vectorCrossProduct(vp1))/sum;
    result.v = normalNotNormalized.vectorDotProduct(edge2.vectorCrossProduct(vp2))/sum;
    
    //cout << "normal:  " << normalNotNormalized.x << "," << normalNotNormalized.y << "," << normalNotNormalized.z << endl;
    //cout << "u,v,sum:  " << result.u << "," << result.v << "," << sum << endl;
    
    result.isHit = true;
    result.distance = t;
    result.intersectPosition= ray.rayPoint(result.distance);
    result.intersectNormal = normal.normalizeVector();
    return result;
}


