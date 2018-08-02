
#include "ObjLoader.hpp"
#include "stdlib.h"
#include <iostream>
#include <fstream>
#include <cstring>

//the number of max read chars each time
int charNumber = 200;


ObjLoader::ObjLoader(){
    ImportObj(&my3DModel, FILE_PATH);
    polygonNum=my3DModel.faceCount;
    vertexNum=my3DModel.vertexCount;
}


//change every vertex's coordinate to put the initial camera into the average point
vector<CVertex> setAveragePoint(vector<CVertex> vertexs) {
    float X_AVE = 0;
    float Y_AVE = 0;
    float Z_AVE = 0;
    for(int i=0; i<vertexs.size(); i++) {
        X_AVE += vertexs.at(i).vertexPoint.x;
        Y_AVE += vertexs.at(i).vertexPoint.y;
        Z_AVE += vertexs.at(i).vertexPoint.z;
    }
    X_AVE /= vertexs.size();
    Y_AVE /= vertexs.size();
    Z_AVE /= vertexs.size();
    vector<CVertex> vertexVector;
    for(int i=0; i<vertexs.size(); i++) {
        CVertex vertex;
        vertex.vertexPoint.x = vertexs.at(i).vertexPoint.x - X_AVE;
        vertex.vertexPoint.y = vertexs.at(i).vertexPoint.y - Y_AVE;
        vertex.vertexPoint.z = vertexs.at(i).vertexPoint.z - Z_AVE;
        vertexVector.push_back(vertex);
    }
    return vertexVector;
}


Vector3f ObjLoader::getFaceNormal(CFace f) {
    Vector3f v0 = myVertice.at(f.vertexIndex[0] - 1).vertexPoint;
    Vector3f v1 = myVertice.at(f.vertexIndex[1] - 1).vertexPoint;
    Vector3f v2 = myVertice.at(f.vertexIndex[2] - 1).vertexPoint;
    return (v0.operator-(v1)).vectorCrossProduct((v0.operator-(v2))).normalizeVector();
}

Vector3f ObjLoader::getVertexNormal(CVertex v) {
    Vector3f vertexNormal(0,0,0);
    float num = 0;
    for(int i=0; i < v.vertexFace.size(); i++) {
        vertexNormal = vertexNormal.operator+(myFace.at(v.vertexFace.at(i)).normal);
        num+= myFace.at(v.vertexFace.at(i)).normal.vectorLength();
    }
    
    if(num > 0) {
        return vertexNormal.operator/(num);
    } else {
        return vertexNormal;
    }
}


//read OBJ file and import information into model
void ObjLoader::ImportObj(CModel *myModel, char* fileName) {
    char line[charNumber];
    myFilePointer = fopen(fileName, "r");
    if(!myFilePointer) {
        cout << "Something Went Wrong When Opening Objfiles" << endl;
        return;
    }
    
    while(!feof(myFilePointer))
    {
        fscanf(myFilePointer,"%s",line);
        if(strcmp(line,"v") == 0) {
            ReadVertexInfo();
        } else if(strcmp(line,"f") == 0) {
            ReadFaceInfo();
        } else {
            fgets(line, 100, myFilePointer);
        }
    }
    //fill vertex and face infomation into model.
    myVertice = setAveragePoint(myVertice);
    myModel->vertexCount = (int)myVertice.size();
    myModel->faceCount = (int)myFace.size();
    if(myModel->vertexCount > 0) {
        myModel->vertex = new CVertex [myModel->vertexCount];
        for(int i = 0; i < myModel->vertexCount; i++)
        {
            myModel->vertex[i] = myVertice[i];
        }
    }
    if(myModel->faceCount > 0)
    {
       
        myModel->face = new CFace [myModel->faceCount];
        for(int i = 0; i < myModel->faceCount; i++)
        {
            myModel->face[i] = myFace[i];
        }
    }
    
    for(int i = 0; i < myFace.size(); i++) {
        myModel->face[i] = myFace.at(i);
        myModel->face[i].normal = getFaceNormal(myFace.at(i));
        myModel->vertex[myFace.at(i).vertexIndex[0]-1].vertexFace.push_back(i);
        myModel->vertex[myFace.at(i).vertexIndex[1]-1].vertexFace.push_back(i);
        myModel->vertex[myFace.at(i).vertexIndex[2]-1].vertexFace.push_back(i);
    }
    
    for(int i = 0; i < myVertice.size(); i++) {
        myModel->vertex[i].vertexNormal = getVertexNormal(myModel->vertex[i]);
    }
    myVertice.clear();
    myFace.clear();
    fclose(myFilePointer);
}


void ObjLoader::ReadVertexInfo()
{
    CVertex vertex;
    fgetc(myFilePointer);
    fscanf(myFilePointer, "%f %f %f", &vertex.vertexPoint.x, &vertex.vertexPoint.y, &vertex.vertexPoint.z);
    myVertice.push_back(vertex);
}


void ObjLoader::ReadFaceInfo()
{
    CFace face;
    char line[charNumber];
    fgetc(myFilePointer);
    fgets(line, charNumber, myFilePointer);
    int faceVertexNum = 1;
    bool normalFlag = true;
    

    //get face number and normalFlag handles this situation with out normal in the obj file, like f: 1//2
    for(int i=0;i<strlen(line)-1;i++) {
        if(line[i] == ' ' && line[i+1] != ' ') {
            faceVertexNum++;
        }
        if(line[i] == '/' && line[i+1] == '/') {
            normalFlag = false;
        }
    }
    
    vector<string> faceArray;
    char* faceInfo = new char[20];
    int j = 0;
    for(int i=0;i<strlen(line);i++) {
        if(line[i] != ' ' && line[i] != '\n') {
            faceInfo[j] = line[i];
            j++;
        } else if(line[i - 1] != ' '){
            string face = string(faceInfo);
            faceArray.push_back(face);
            j = 0;
            faceInfo = new char[30];
        }
    }
    if(faceVertexNum >= 3) {
        if(faceVertexNum == 3) {
            sscanf(line,"%d/%d/%d %d/%d/%d %d/%d/%d ",
                   &face.vertexIndex[0],&face.textureIndex[0],&face.normalIndex[0],
                   &face.vertexIndex[1],&face.textureIndex[1],&face.normalIndex[1],
                   &face.vertexIndex[2],&face.textureIndex[2],&face.normalIndex[2]);
            myFace.push_back(face);
        } else {
            for(int i = 0; i < faceArray.size() - 2; i++) {
                const char *faceInfo0 = faceArray.at(0).c_str();
                sscanf(faceInfo0, "%d/%d/%d",
                       &face.vertexIndex[0],&face.textureIndex[0],&face.normalIndex[0]);
                const char *faceInfo1 = faceArray.at(i+1).c_str();
                sscanf(faceInfo1, "%d/%d/%d",
                       &face.vertexIndex[1],&face.textureIndex[1],&face.normalIndex[1]);
                const char *faceInfo2 = faceArray.at(i+2).c_str();
                sscanf(faceInfo2, "%d/%d/%d",
                       &face.vertexIndex[2],&face.textureIndex[2],&face.normalIndex[2]);
                myFace.push_back(face);
            }
        }
    }
    //cout << myFace.size() << endl;
}




