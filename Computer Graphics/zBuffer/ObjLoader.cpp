
#include "ObjLoader.hpp"
#include "stdlib.h"
#include <iostream>
#include <fstream>
#include <cstring>

//the number of max read chars each time
int charNumber = 200;

//the number of max polygon colors
int colorNumber = 6;

//get random color each time.
int getRandom() {
    srand((unsigned)time(0));
    return rand()%colorNumber;
}
int colorFlag = getRandom();


//change every vertex's coordinate to put the initial camera into the average point
vector<CVertex> setAveragePoint(vector<CVertex> vertexs) {
    float X_AVE = 0;
    float Y_AVE = 0;
    float Z_AVE = 0;
    for(int i=0; i<vertexs.size(); i++) {
        X_AVE += vertexs.at(i).x;
        Y_AVE += vertexs.at(i).y;
        Z_AVE += vertexs.at(i).z;
    }
    X_AVE /= vertexs.size();
    Y_AVE /= vertexs.size();
    Z_AVE /= vertexs.size();
    vector<CVertex> vertexVector;
    for(int i=0; i<vertexs.size(); i++) {
        CVertex vertex;
        vertex.x = vertexs.at(i).x - X_AVE;
        vertex.y = vertexs.at(i).y - Y_AVE;
        vertex.z = vertexs.at(i).z - Z_AVE;
        vertexVector.push_back(vertex);
    }
    return vertexVector;
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
    myVertice.clear();
    myFace.clear();
    fclose(myFilePointer);
}



void ObjLoader::ReadVertexInfo()
{
    CVertex vertex;
    fgetc(myFilePointer);
    fscanf(myFilePointer, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
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
    
    //this project support at most 5 vertexs in one face.
    if(faceVertexNum == 3) {
        if(normalFlag) {
        //read the file in specific model
        sscanf(line,"%d/%d/%d %d/%d/%d %d/%d/%d ",
           &face.vertexIndex[0],&face.textureIndex[0],&face.normalIndex[0],
           &face.vertexIndex[1],&face.textureIndex[1],&face.normalIndex[1],
           &face.vertexIndex[2],&face.textureIndex[2],&face.normalIndex[2]);
        } else {
            sscanf(line,"%d//%d %d//%d %d//%d ",
                   &face.vertexIndex[0],&face.normalIndex[0],
                   &face.vertexIndex[1],&face.normalIndex[1],
                   &face.vertexIndex[2],&face.normalIndex[2]);
        }
    } else if(faceVertexNum == 4){
        if(normalFlag) {
        sscanf(line,"%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d ",
            &face.vertexIndex[0],&face.textureIndex[0],&face.normalIndex[0],
            &face.vertexIndex[1],&face.textureIndex[1],&face.normalIndex[1],
            &face.vertexIndex[2],&face.textureIndex[2],&face.normalIndex[2],
            &face.vertexIndex[3],&face.textureIndex[3],&face.normalIndex[3]);
        } else {
            sscanf(line,"%d//%d %d//%d %d//%d %d//%d",
                   &face.vertexIndex[0],&face.normalIndex[0],
                   &face.vertexIndex[1],&face.normalIndex[1],
                   &face.vertexIndex[2],&face.normalIndex[2],
                   &face.vertexIndex[3],&face.normalIndex[3]);
        }
    } else if(faceVertexNum == 5) {
        if(normalFlag) {
            sscanf(line,"%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
                   &face.vertexIndex[0],&face.textureIndex[0],&face.normalIndex[0],
                   &face.vertexIndex[1],&face.textureIndex[1],&face.normalIndex[1],
                   &face.vertexIndex[2],&face.textureIndex[2],&face.normalIndex[2],
                   &face.vertexIndex[3],&face.textureIndex[3],&face.normalIndex[3],
                   &face.vertexIndex[4],&face.textureIndex[4],&face.normalIndex[4]);
        } else {
            sscanf(line,"%d//%d %d//%d %d//%d %d//%d %d//%d",
                   &face.vertexIndex[0],&face.normalIndex[0],
                   &face.vertexIndex[1],&face.normalIndex[1],
                   &face.vertexIndex[2],&face.normalIndex[2],
                   &face.vertexIndex[3],&face.normalIndex[3],
                   &face.vertexIndex[4],&face.normalIndex[4]);
        }
    } else {
    }
    face.num = faceVertexNum;
    
    //set the difference color to each polygon.
    switch (colorFlag) {
        case 0:
            face.col[0] = 1.0;
            face.col[1] = 0.0;
            face.col[2] = 0.0;
            colorFlag++;
            break;
        case 1:
            face.col[0] = 0.0;
            face.col[1] = 1.0;
            face.col[2] = 0.0;
            colorFlag++;
            break;
        case 2:
            face.col[0] = 0.0;
            face.col[1] = 0.0;
            face.col[2] = 1.0;
            colorFlag++;
            break;
        case 3:
            face.col[0] = 1.0;
            face.col[1] = 0.0;
            face.col[2] = 1.0;
            colorFlag++;
            break;
        case 4:
            face.col[0] = 1.0;
            face.col[1] = 1.0;
            face.col[2] = 0.0;
            colorFlag++;
            break;
        case 5:
            face.col[0] = 0.0;
            face.col[1] = 1.0;
            face.col[2] = 1.0;
            colorFlag=0;
            break;
    }
    myFace.push_back(face);
}





