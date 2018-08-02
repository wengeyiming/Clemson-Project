

#ifndef ObjAttribute_h
#define ObjAttribute_h
#include <vector>
using namespace std;

//the max vertex number of one face.
#define MAX_VERTEX_OF_FACE 5
#define FILE_PATH "model1.obj"


class CVertex {
public:
    float x, y, z;
};

struct CFace{
    int vertexIndex[MAX_VERTEX_OF_FACE];//vertexIndex
    int normalIndex[MAX_VERTEX_OF_FACE];//noramalIndex
    int textureIndex[MAX_VERTEX_OF_FACE];//textureIndex
    float col[3]; // RGB color
    int num;
};


struct CModel {
    CVertex *vertex;
    CFace *face;
    int vertexCount,faceCount;
};



#endif /* ObjAttribute_h */
