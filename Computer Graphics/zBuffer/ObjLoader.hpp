

#ifndef ObjLoader_hpp
#define ObjLoader_hpp

#include <stdio.h>
#include "ObjAttribute.h"




class ObjLoader {
public:
    //read objFile to the model
    void ImportObj(CModel *myModel, char* fileName);
    
    //read Vertex information
    void ReadVertexInfo();
    
    //read Face information
    void ReadFaceInfo();
    
    
private:
    
    //the file pointer
    FILE *myFilePointer;
    
    //vector of vertex
    vector<CVertex>  myVertice;
    
    //vector of face
    vector<CFace>  myFace;
};



#endif /* ObjLoader_hpp */
