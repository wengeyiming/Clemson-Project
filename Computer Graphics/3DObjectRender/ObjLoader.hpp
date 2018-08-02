

#ifndef ObjLoader_hpp
#define ObjLoader_hpp

#include <stdio.h>
#include "ObjAttribute.h"


#define winX 400
#define winY 400
#define winZ 400

class ObjLoader {
public:
    ObjLoader();
    
    //read objFile to the model
    void ImportObj(CModel *myModel, char* fileName);
    
    //read Vertex information
    void ReadVertexInfo();
    
    //read Face information
    void ReadFaceInfo();
    
    CModel getModel(){
        return my3DModel;
    }
    
    Vector3f getFaceNormal(CFace f);
    
    Vector3f getVertexNormal(CVertex v);
private:
    
    //the file pointer
    FILE *myFilePointer;
    
    //vector of vertex
    vector<CVertex>  myVertice;
    
    //vector of face
    vector<CFace>  myFace;
    
    CModel my3DModel;
    
    int polygonNum;
    int vertexNum;
};



#endif /* ObjLoader_hpp */
