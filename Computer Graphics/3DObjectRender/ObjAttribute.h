

#ifndef ObjAttribute_h
#define ObjAttribute_h
#include <vector>
#include "Vector3f.hpp"
using namespace std;

//file path
#define FILE_PATH "model1.obj"


struct CVertex {
public:
    Vector3f vertexPoint;
    Vector3f vertexNormal;
    vector<int> vertexFace;
};

struct CFace{
    int vertexIndex[3];//vertexIndex
    int normalIndex[3];//noramalIndex
    int textureIndex[3];//textureIndex
    float col[3]; // RGB color
    Vector3f normal;//face normal
    int num;
};


struct CModel {
    CVertex *vertex;
    CFace *face;
    int vertexCount,faceCount;
};

struct IntersectResult {
    Vector3f intersectPosition;
    Vector3f intersectNormal;
    int faceIndex;
    float distance;
    bool isHit;
    float u;
    float v;
    static inline IntersectResult initialIntersectResult() {return IntersectResult();}
};


#endif /* ObjAttribute_h */
