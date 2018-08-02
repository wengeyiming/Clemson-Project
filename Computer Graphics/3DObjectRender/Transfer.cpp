
#include "Transfer.hpp"
#include <math.h>

#include<iostream>
using namespace std;

#define MTM_SIZE 4
#define PI 3.14159265354




void Transfer::Scale(CVertex* vertex, float sx, float sy, float sz, int size) {
    float s_matrix[MTM_SIZE][MTM_SIZE]
    = { { sx, 0, 0, 0 },
        { 0, sy, 0, 0 },
        { 0, 0, sz, 0 },
        { 0, 0, 0, 1 } };
    for(int i = 0; i < size; i++) {
        float matrix[4] = {vertex[i].vertexPoint.x, vertex[i].vertexPoint.y, vertex[i].vertexPoint.z, 1};
        MultipleMatrix(matrix, (float**)s_matrix);
        vertex[i].vertexPoint.x = matrix[0];
        vertex[i].vertexPoint.y = matrix[1];
        vertex[i].vertexPoint.z = matrix[2];
    }
    
}


void Transfer::Translate(CVertex* vertex, float tx, float ty, float tz, int size) {
    float t_matrix[MTM_SIZE][MTM_SIZE]
    = { { 1, 0, 0, tx },
        { 0, 1, 0, ty },
        { 0, 0, 1, tz },
        { 0, 0, 0, 1 } };
    for(int i = 0; i < size; i++) {
        float matrix[4] = {vertex[i].vertexPoint.x, vertex[i].vertexPoint.y, vertex[i].vertexPoint.z, 1};
        MultipleMatrix(matrix, (float**)t_matrix);
        vertex[i].vertexPoint.x = matrix[0];
        vertex[i].vertexPoint.y = matrix[1];
        vertex[i].vertexPoint.z = matrix[2];
    }
}


void Transfer::Rotate(CVertex* vertex, float xDegree, float yDegree, float zDegree, int size) {
    if(xDegree == 0 && yDegree == 0 && zDegree == 0) {
        return;
    }
    if(xDegree != 0) {
        float rad = xDegree * PI / 180.0;
        float r_matrix[MTM_SIZE][MTM_SIZE]
        = { { 1, 0, 0, 0 },
            { 0, (float)cos(rad), (float)-sin(rad), 0 },
            { 0, (float)sin(rad), (float)cos(rad), 0 },
            { 0, 0, 0, 1 } };
        for(int i = 0; i < size; i++) {
            float matrix[4] = {vertex[i].vertexPoint.x, vertex[i].vertexPoint.y, vertex[i].vertexPoint.z, 1};
            MultipleMatrix(matrix, (float**)r_matrix);
            vertex[i].vertexPoint.x = matrix[0];
            vertex[i].vertexPoint.y = matrix[1];
            vertex[i].vertexPoint.z = matrix[2];
        }
    }
    if(yDegree != 0) {
        float rad = yDegree * PI / 180.0;
        //notice that symbols before sin are opposite
        float r_matrix[MTM_SIZE][MTM_SIZE]
        = { { (float)cos(rad), 0, (float)sin(rad), 0 },
            { 0, 1, 0, 0 },
            { (float)-sin(rad), 0, (float)cos(rad), 0 },
            { 0, 0, 0, 1 } };

        for(int i = 0; i < size; i++) {
            float matrix[4] = {vertex[i].vertexPoint.x, vertex[i].vertexPoint.y, vertex[i].vertexPoint.z, 1};
            MultipleMatrix(matrix, (float**)r_matrix);
            vertex[i].vertexPoint.x = matrix[0];
            vertex[i].vertexPoint.y = matrix[1];
            vertex[i].vertexPoint.z = matrix[2];
        }
    }
    if(zDegree != 0) {
        float rad = zDegree * PI / 180.0;
        float r_matrix[MTM_SIZE][MTM_SIZE]
        = { { (float)cos(rad), (float)-sin(rad), 0, 0 },
            { (float)sin(rad), (float)cos(rad), 0, 0 },
            { 0, 0, 1, 0 },
            { 0, 0, 0, 1 } };
        for(int i = 0; i < size; i++) {
            float matrix[4] = {vertex[i].vertexPoint.x, vertex[i].vertexPoint.y, vertex[i].vertexPoint.z, 1};
            MultipleMatrix(matrix, (float**)r_matrix);
            vertex[i].vertexPoint.x = matrix[0];
            vertex[i].vertexPoint.y = matrix[1];
            vertex[i].vertexPoint.z = matrix[2];
        }
    }
}

void Transfer::Perspective(CVertex *vertex, Vector3f cameraPoint, float zDepth, int size, bool isPerspect) {
    if(isPerspect) {
        float c_matrix[MTM_SIZE][MTM_SIZE]
        = { { 1, 0, 0, cameraPoint.x},
            { 0, 1, 0, cameraPoint.y},
            { 0, 0, 1, cameraPoint.z},
            { 0, 0, 0, 1} };
        for(int i = 0; i < size; i++) {
            float matrix[4] = {vertex[i].vertexPoint.x, vertex[i].vertexPoint.y, vertex[i].vertexPoint.z, 1};
            MultipleMatrix(matrix, (float**)c_matrix);
            vertex[i].vertexPoint.x = matrix[0]*zDepth/matrix[2];
            vertex[i].vertexPoint.y = matrix[1]*zDepth/matrix[2];
            vertex[i].vertexPoint.z = matrix[2];
        }
    } else {
        float c_matrix[MTM_SIZE][MTM_SIZE]
        = { { 1, 0, 0, -cameraPoint.x},
            { 0, 1, 0, -cameraPoint.y},
            { 0, 0, 1, -cameraPoint.z},
            { 0, 0, 0, 1} };
        for(int i = 0; i < size; i++) {
            vertex[i].vertexPoint.x = vertex[i].vertexPoint.x*vertex[i].vertexPoint.z/zDepth;
            vertex[i].vertexPoint.y = vertex[i].vertexPoint.y*vertex[i].vertexPoint.z/zDepth;
            vertex[i].vertexPoint.z = vertex[i].vertexPoint.z;
            float matrix[4] = {vertex[i].vertexPoint.x, vertex[i].vertexPoint.y, vertex[i].vertexPoint.z, 1};
            MultipleMatrix(matrix, (float**)c_matrix);
            vertex[i].vertexPoint.x = matrix[0];
            vertex[i].vertexPoint.y = matrix[1];
            vertex[i].vertexPoint.z = matrix[2];
        }
    }
}


void Transfer::MultipleMatrix(float *initalMatrix, float **transforMatrix) {
    float result[4];
    for(int i = 0; i < 4; i++) {
        float a = (float)*((float *) transforMatrix + i * 4 + 0) * initalMatrix[0];
        float b = (float)*((float *) transforMatrix + i * 4 + 1) * initalMatrix[1];
        float c = (float)*((float *) transforMatrix + i * 4 + 2) * initalMatrix[2];
        float d = (float)*((float *) transforMatrix + i * 4 + 3) * initalMatrix[3];
        result[i] = a + b + c + d;
    }
    for(int i = 0; i < 4; i++) {
        initalMatrix[i] = result[i];
    }
}