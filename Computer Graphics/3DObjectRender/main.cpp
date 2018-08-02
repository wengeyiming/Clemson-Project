//
//		          Programming Assignment #1
//
//			        Victor Zordan
//
//
//
/***************************************************************************/

/* Include needed files */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>  
#include <vector>
#include <list>
#include <math.h>
#include "Transfer.hpp"
#include "ObjAttribute.h"
#include "ObjLoader.hpp"
#include <stdio.h>
#include <iostream>
#include "Camera.hpp"
#include "TSample.hpp"
#include "Ray.hpp"

using namespace std;
int x_last,y_last;

//transfer polygon
Transfer transfer;
//show perspective view
bool perspectFlag = true;
//show wireFrame
bool wireFrameFlag = true;
//show flag shadow
bool isFlagShaded = false;
//show smooth shadow
bool isSmoothShaded = true;

//0: initial, 1: rotate, 2: translate, 3: scale
int commandType = 0;

//Load Obj File
ObjLoader myObjLoader;


Camera camera;

/***************************************************************************/


void write_pixel(int x, int y, int z, vector<float> color)
/* Turn on the pixel found at x,y */
{
    glColor3f (color.at(0), color.at(1), color.at(2));
    glBegin(GL_POINTS);
    glVertex3i( x, y, z);
    glEnd();
}

/***************************************************************************/



//draw Line
void BresenhamLineSegment(CVertex v1, CVertex v2)
{
    float x1 = v1.vertexPoint.x;
    float y1 = v1.vertexPoint.y;
    float z1 = v1.vertexPoint.z;
    float x2 = v2.vertexPoint.x;
    float y2 = v2.vertexPoint.y;
    float z2 = v2.vertexPoint.z;
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;
    int point[3],x_inc,y_inc,z_inc,l,m,n,dx2,dy2,dz2,err_1,err_2;
    point[0] = x1;
    point[1] = y1;
    point[2] = z1;
    x_inc = (dx < 0) ? -1 : 1;
    l = (dx > 0 ? dx : -dx);
    y_inc = (dy < 0) ? -1 : 1;
    m = (dy > 0 ? dy : -dy);
    z_inc = (dz < 0) ? -1 : 1;
    n = (dz > 0 ? dz : -dz);
    dx2 = l << 1;
    dy2 = m << 1;
    dz2 = n << 1;
    
    vector<float> white(3,1.0);
    
    if ((l >= m) && (l >= n)) {
        err_1 = dy2 - l;
        err_2 = dz2 - l;
        for (int i = 0; i < l; i++) {
            write_pixel(point[0], point[1], point[2], white);
            if (err_1 > 0) {
                point[1] += y_inc;
                err_1 -= dx2;
            }
            if (err_2 > 0) {
                point[2] += z_inc;
                err_2 -= dx2;
            }
            err_1 += dy2;
            err_2 += dz2;
            point[0] += x_inc;
        }
    } else if ((m >= l) && (m >= n)) {
        err_1 = dx2 - m;
        err_2 = dz2 - m;
        for (int i = 0; i < m; i++) {
            write_pixel(point[0], point[1], point[2], white);
            if (err_1 > 0) {
                point[0] += x_inc;
                err_1 -= dy2;
            }
            if (err_2 > 0) {
                point[2] += z_inc;
                err_2 -= dy2;
            }
            err_1 += dx2;
            err_2 += dz2;
            point[1] += y_inc;
        }
    } else {
        err_1 = dy2 - n;
        err_2 = dx2 - n;
        for (int i = 0; i < n; i++) {
            write_pixel(point[0], point[1], point[2], white);
            if (err_1 > 0) {
                point[1] += y_inc;
                err_1 -= dz2;
            }
            if (err_2 > 0) {
                point[0] += x_inc;
                err_2 -= dz2;
            }
            err_1 += dy2;
            err_2 += dx2;
            point[2] += z_inc;
        }
    }
}


/***************************************************************************/

void showRender() {
    for(int y = -winY; y < winY; y++) {
        for(int x = -winX; x < winX; x++) {
            Ray ray(camera.generateRays(x, y, 0));
            int minDistance = 1000000;
            IntersectResult result;
            result.isHit = false;
            
            for(int i = 0; i < myObjLoader.getModel().faceCount; i++) {
                CFace face = myObjLoader.getModel().face[i];
                TSample sample(myObjLoader.getModel().vertex[face.vertexIndex[0] - 1], myObjLoader.getModel().vertex[face.vertexIndex[1] - 1], myObjLoader.getModel().vertex[face.vertexIndex[2] - 1],face.normal);
                IntersectResult r = sample.isIntersect(ray);
                if(r.isHit) {
                    r.faceIndex = i;
                    if(r.distance < minDistance) {
                        minDistance = r.distance;
                        result = r;
                    }
                }
            }
            
            if(result.isHit) {
                vector<float> color;
                if(isFlagShaded) {
                    float *col = myObjLoader.getModel().face[result.faceIndex].col;
                    for(int i = 0; i < 3; i++) {
                        color.push_back(col[i]);
                    }
                } else {
                    CFace face = myObjLoader.getModel().face[result.faceIndex];
                    TSample sample(myObjLoader.getModel().vertex[face.vertexIndex[0] - 1], myObjLoader.getModel().vertex[face.vertexIndex[1] - 1], myObjLoader.getModel().vertex[face.vertexIndex[2] - 1],face.normal);
                    RGBColor smoothColor;
                    if(isSmoothShaded) {
                        smoothColor = sample.smoothShadingColor(camera, result);
                        smoothColor.saturateColor();
                    } else {
                        smoothColor = sample.bumpMappingColor(camera, result);
                        smoothColor.saturateColor();
                    }
                    float *col = new float[3];
                    col[0] = smoothColor.r;
                    col[1] = smoothColor.g;
                    col[2] = smoothColor.b;
                    //cout << "r,g,b:  " << smoothColor.r <<"  "<< smoothColor.g << "  "<< smoothColor.b << endl;
                    for(int i = 0; i < 3; i++) {
                        color.push_back(col[i]);
                    }
                    //cout << col[0] << "," << col[1] << "," << col[2] << endl;
                }
                write_pixel(x, y, 0, color);
            }
        }
    }
}
/***************************************************************************/

Vector3f getFaceNormal(CFace f) {
   
    Vector3f v0 =  myObjLoader.getModel().vertex[f.vertexIndex[0] - 1].vertexPoint;
    Vector3f v1 =  myObjLoader.getModel().vertex[f.vertexIndex[1] - 1].vertexPoint;
    Vector3f v2 =  myObjLoader.getModel().vertex[f.vertexIndex[2] - 1].vertexPoint;
    return (v0.operator-(v1)).vectorCrossProduct((v0.operator-(v2))).normalizeVector();
}

Vector3f getVertexNormal(CVertex v) {
    Vector3f vertexNormal(0,0,0);
    float num = 0;
    for(int i=0; i < v.vertexFace.size(); i++) {
        vertexNormal = vertexNormal.operator+(myObjLoader.getModel().face[v.vertexFace.at(i)].normal);
        num+= myObjLoader.getModel().face[v.vertexFace.at(i)].normal.vectorLength();
    }
    
    if(num > 0) {
        return vertexNormal.operator/(num);
    } else {
        return vertexNormal;
    }
}


void reCalculateFaceNormal(){
    for(int i = 0; i < myObjLoader.getModel().faceCount; i++) {
        Vector3f faceNor = getFaceNormal(myObjLoader.getModel().face[i]);
        myObjLoader.getModel().face[i].normal = faceNor;
    }
}

void reCalculateVertexNor(){
    for(int i =0; i < myObjLoader.getModel().vertexCount; i++) {
        Vector3f veNor = getVertexNormal(myObjLoader.getModel().vertex[i]);
        myObjLoader.getModel().vertex[i].vertexNormal = veNor;
    }
}


void calculateFaceColor(){
    reCalculateFaceNormal();
    reCalculateVertexNor();
    for(int i = 0; i < myObjLoader.getModel().faceCount; i++) {
        CFace face = myObjLoader.getModel().face[i];
        TSample sample(myObjLoader.getModel().vertex[face.vertexIndex[0] - 1], myObjLoader.getModel().vertex[face.vertexIndex[1] - 1], myObjLoader.getModel().vertex[face.vertexIndex[2] - 1],face.normal);
        RGBColor color = sample.calculateColor(camera, myObjLoader.getModel().vertex[face.vertexIndex[0] - 1]);
        color.saturateColor();        
        myObjLoader.getModel().face[i].col[0] = color.r;
        myObjLoader.getModel().face[i].col[1] = color.g;
        myObjLoader.getModel().face[i].col[2] = color.b;
    }
}


/***************************************************************************/

void showWireFrame() {
    for(int i = 0;i < myObjLoader.getModel().faceCount; i++) {
        int vexIndex0 = myObjLoader.getModel().face[i].vertexIndex[0];
        int vexIndex1 = myObjLoader.getModel().face[i].vertexIndex[1];
        int vexIndex2 = myObjLoader.getModel().face[i].vertexIndex[2];
        //the index in the obj file must decrease 1 because the array starts from 0.
        CVertex vex0 = myObjLoader.getModel().vertex[vexIndex0-1];
        CVertex vex1 = myObjLoader.getModel().vertex[vexIndex1-1];
        CVertex vex2 = myObjLoader.getModel().vertex[vexIndex2-1];
        BresenhamLineSegment(vex0, vex1);
        BresenhamLineSegment(vex0, vex2);
        BresenhamLineSegment(vex1, vex2);
    }
    
}

//***************************************************************************/

void display (void)   // Create The Display Function
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	      // Clear Screen
    // CALL YOUR CODE HERE

    if(wireFrameFlag) {
        showWireFrame();
    } else {
        showRender();
    }
    glFlush();
    glutSwapBuffers();                                      // Draw Frame Buffer
}


//***************************************************************************/
void init_window()
/* Clear the image area, and set up the coordinate system */
{
    
    /* Clear the window */
    glLoadIdentity();
    camera = Camera(Vector3f(0.0, 0.0, 2*winZ));
    calculateFaceColor();
    
    glOrtho(-(GLfloat)winX,(GLfloat)winX,-(GLfloat)winY,(GLfloat)winY,-(GLfloat)winZ, (GLfloat)winZ);
}



/***************************************************************************/
void mouse(int button, int state, int x, int y)
{
    /* This function I finessed a bit, the value of the printed x,y should
     match the screen, also it remembers where the old value was to avoid multiple
     readings from the same mouse click.  This can cause problems when trying to
     start a line or curve where the last one ended */
    printf(" x,y is (%d,%d)\n", x,y);
}

/***************************************************************************/
void keyboard ( unsigned char key, int x, int y )  // Create Keyboard Function
{
    
    switch ( key ) {
        case 27:              // When Escape Is Pressed...
            exit ( 0 );   // Exit The Program
            break;
        case '1':             // stub for new screen
            printf("New screen\n");
            break;
        
        case 'w':
            if(commandType != 0) {
                switch (commandType) {
                    case 1:
                        transfer.Rotate(myObjLoader.getModel().vertex, 10, 0, 0, myObjLoader.getModel().vertexCount);
                        commandType = 0;
                        calculateFaceColor();
                        break;
                    case 2:
                        transfer.Translate(myObjLoader.getModel().vertex, 0, 10, 0, myObjLoader.getModel().vertexCount);
                        commandType = 0;
                        calculateFaceColor();
                        break;
                    case 3:
                        transfer.Scale(myObjLoader.getModel().vertex, 0.9, 0.9, 0.9, myObjLoader.getModel().vertexCount);
                        commandType = 0;
                        calculateFaceColor();
                        break;
                }
            } else {
                wireFrameFlag = !wireFrameFlag;
            }
            break;
        case 's':
            switch (commandType) {
                case 1:
                    transfer.Rotate(myObjLoader.getModel().vertex, -10, 0, 0, myObjLoader.getModel().vertexCount);
                    commandType = 0;
                    calculateFaceColor();
                    break;
                case 2:
                    transfer.Translate(myObjLoader.getModel().vertex, 0, -10, 0, myObjLoader.getModel().vertexCount);
                    commandType = 0;
                    calculateFaceColor();
                    break;
                case 3:
                    transfer.Scale(myObjLoader.getModel().vertex, 0.9, 0.9, 0.9, myObjLoader.getModel().vertexCount);
                    commandType = 0;
                    calculateFaceColor();
                    break;
            }
            break;
        case 'a':
            switch (commandType) {
                case 1:
                    transfer.Rotate(myObjLoader.getModel().vertex, 0, 10, 0, myObjLoader.getModel().vertexCount);
                    commandType = 0;
                    calculateFaceColor();
                    break;
                case 2:
                    transfer.Translate(myObjLoader.getModel().vertex, -10, 0, 0, myObjLoader.getModel().vertexCount);
                    commandType = 0;
                    calculateFaceColor();
                    break;
                case 3:
                    transfer.Scale(myObjLoader.getModel().vertex, 1.1, 1.1, 1.1, myObjLoader.getModel().vertexCount);
                    commandType = 0;
                    calculateFaceColor();
                    break;
            }
            break;
        case 'd':
            switch (commandType) {
                case 1:
                    transfer.Rotate(myObjLoader.getModel().vertex, 0, -10, 0, myObjLoader.getModel().vertexCount);
                    commandType = 0;
                    calculateFaceColor();
                    break;
                case 2:
                    transfer.Translate(myObjLoader.getModel().vertex, 10, 0, 0, myObjLoader.getModel().vertexCount);
                    commandType = 0;
                    calculateFaceColor();
                    break;
                case 3:
                    transfer.Scale(myObjLoader.getModel().vertex, 1.1, 1.1, 1.1, myObjLoader.getModel().vertexCount);
                    commandType = 0;
                    calculateFaceColor();
                    break;
            }
            break;
        case 'r':
            commandType = 1;
            break;
        case 't':
            commandType = 2;
            break;
        case 'e':
            commandType = 3;
            break;
        case 'x':
            isFlagShaded = !isFlagShaded;
            break;
        case 'b':
            isSmoothShaded = !isSmoothShaded;
            break;
        case 'v':
            Vector3f cameraPoint;
            transfer.Perspective(myObjLoader.getModel().vertex, cameraPoint, 1.01, myObjLoader.getModel().vertexCount,perspectFlag);
            perspectFlag = !perspectFlag;
            break;
    }
}




/***************************************************************************/
int main (int argc, char *argv[])
{
    /* This main function sets up the main loop of the program and continues the
     loop until the end of the data is reached.  Then the window can be closed
     using the escape key.						  */
    
    glutInit            ( &argc, argv );
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize  ( winX*2,winY*2 );
    glutCreateWindow    ( "CG Assn2" );
    glutDisplayFunc     ( display );
    glutIdleFunc	    ( display);
    glutMouseFunc       ( mouse );
    glutKeyboardFunc    ( keyboard );
    
    init_window();				             //create_window
    
    glutMainLoop        ( );                 // Initialize The Main Loop
}


