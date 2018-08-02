

#ifndef Transfer_hpp
#define Transfer_hpp

#include "ObjAttribute.h"


class Transfer {
public:
    void Scale(CVertex *vertex,float sx, float sy, float sz, int size);
    void Rotate(CVertex *vertex,float xDegree, float yDegree, float zDegree, int size);
    void Translate(CVertex *vertex,float tx, float ty, float tz, int size);
    void Perspective(CVertex* vertex,Vector3f cameraPoint, float zDepth, int size, bool isPerspect);
private:
    void MultipleMatrix(float* initalMatrix, float**transforMatrix);
};

#endif /* Transfer_hpp */
