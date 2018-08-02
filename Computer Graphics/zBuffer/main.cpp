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
#include "ZBuffer.hpp"
#include "Transfer.hpp"
#include "ObjAttribute.h"
#include <stdio.h>
#include <iostream>
#define WIDTH 500
#define HEIGHT 500
using namespace std;
int x_last,y_last;
ZBuffer myZbuffer;
//show wireFrame or zBuffer
bool wireFrameFlag = true;
Transfer transfer;
//show perspective view
bool perspectFlag = true;
//0: initial, 1: rotate, 2: translate, 3: scale
int commandType = 0;
/***************************************************************************/


void write_pixel(int x, int y, int z, double intensity)
/* Turn on the pixel found at x,y */
{
    glColor3f (intensity, intensity, intensity);
    glBegin(GL_POINTS);
    glVertex3i( x, y, z);
    glEnd();
}

/***************************************************************************/
//draw Line
void BresenhamLineSegment(CVertex v1, CVertex v2)
{
    float x1 = v1.x;
    float y1 = v1.y;
    float z1 = v1.z;
    float x2 = v2.x;
    float y2 = v2.y;
    float z2 = v2.z;
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
    
    if ((l >= m) && (l >= n)) {
        err_1 = dy2 - l;
        err_2 = dz2 - l;
        for (int i = 0; i < l; i++) {
            write_pixel(point[0], point[1], point[2], 1.0);
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
            write_pixel(point[0], point[1], point[2], 1.0);
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
            write_pixel(point[0], point[1], point[2], 1.0);
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


void init_window()
/* Clear the image area, and set up the coordinate system */
{
    
    /* Clear the window */
    glLoadIdentity();
    glOrtho(-(GLfloat)winX,(GLfloat)winX,-(GLfloat)winY,(GLfloat)winY,-(GLfloat)winZ, (GLfloat)winZ);
    myZbuffer.clearAll();
    myZbuffer.CreateClassTable();
}


void showWireFrame() {
    for(int i = 0;i < myZbuffer.getModel().faceCount; i++) {
        int vexIndex0 = myZbuffer.getModel().face[i].vertexIndex[0];
        int vexIndex1 = myZbuffer.getModel().face[i].vertexIndex[1];
        int vexIndex2 = myZbuffer.getModel().face[i].vertexIndex[2];
        //the index in the obj file must decrease 1 because the array starts from 0.
        CVertex vex0 = myZbuffer.getModel().vertex[vexIndex0-1];
        CVertex vex1 = myZbuffer.getModel().vertex[vexIndex1-1];
        CVertex vex2 = myZbuffer.getModel().vertex[vexIndex2-1];
        BresenhamLineSegment(vex0, vex1);
        BresenhamLineSegment(vex0, vex2);
        BresenhamLineSegment(vex1, vex2);
    }
    
}

void showZBuffer() {
    for(int y = winY; y >= -winY;y--)
    {
        myZbuffer.initBuffer();
        myZbuffer.addPolTable(y);
        if(myZbuffer.getActPolNum() > 0)
        {
            myZbuffer.updateActEdgeTable(y);
        }
        glRasterPos2i(-winX,y);
        glDrawPixels(winX*2,1,GL_RGB,GL_FLOAT,&myZbuffer.myColorBuffer[0][0]);
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
        showZBuffer();
    }
    glFlush();
    glutSwapBuffers();                                      // Draw Frame Buffer
}



/***************************************************************************/
void mouse(int button, int state, int x, int y)
{
    /* This function I finessed a bit, the value of the printed x,y should
     match the screen, also it remembers where the old value was to avoid multiple
     readings from the same mouse click.  This can cause problems when trying to
     start a line or curve where the last one ended */
    static int oldx = 0;
    static int oldy = 0;
    int mag;
    
    y *= -1;  //align y with mouse
    y += 500; //ignore
    mag = (oldx - x)*(oldx - x) + (oldy - y)*(oldy - y);
    if (mag > 20) {
        printf(" x,y is (%d,%d)\n", x,y);
    }
    oldx = x;
    oldy = y;
    x_last = x;
    y_last = y;
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
                        transfer.Rotate(myZbuffer.getModel().vertex, 10, 0, 0, myZbuffer.getModel().vertexCount);
                        commandType = 0;
                        myZbuffer.clearAll();
                        myZbuffer.CreateClassTable();
                        break;
                    case 2:
                        transfer.Translate(myZbuffer.getModel().vertex, 0, 10, 0, myZbuffer.getModel().vertexCount);
                        commandType = 0;
                        myZbuffer.clearAll();
                        myZbuffer.CreateClassTable();
                        break;
                    case 3:
                        transfer.Scale(myZbuffer.getModel().vertex, 0.9, 0.9, 0.9, myZbuffer.getModel().vertexCount);
                        commandType = 0;
                        myZbuffer.clearAll();
                        myZbuffer.CreateClassTable();
                        break;
                }
            } else {
                wireFrameFlag = !wireFrameFlag;
            }
            break;
        case 's':
            switch (commandType) {
                case 1:
                    transfer.Rotate(myZbuffer.getModel().vertex, -10, 0, 0, myZbuffer.getModel().vertexCount);
                    commandType = 0;
                    myZbuffer.clearAll();
                    myZbuffer.CreateClassTable();
                    break;
                case 2:
                    transfer.Translate(myZbuffer.getModel().vertex, 0, -10, 0, myZbuffer.getModel().vertexCount);
                    commandType = 0;
                    myZbuffer.clearAll();
                    myZbuffer.CreateClassTable();
                    break;
                case 3:
                    transfer.Scale(myZbuffer.getModel().vertex, 0.9, 0.9, 0.9, myZbuffer.getModel().vertexCount);
                    commandType = 0;
                    myZbuffer.clearAll();
                    myZbuffer.CreateClassTable();
                    break;
            }
            break;
        case 'a':
            switch (commandType) {
                case 1:
                    transfer.Rotate(myZbuffer.getModel().vertex, 0, 10, 0, myZbuffer.getModel().vertexCount);
                    commandType = 0;
                    myZbuffer.clearAll();
                    myZbuffer.CreateClassTable();
                    break;
                case 2:
                    transfer.Translate(myZbuffer.getModel().vertex, -10, 0, 0, myZbuffer.getModel().vertexCount);
                    commandType = 0;
                    myZbuffer.clearAll();
                    myZbuffer.CreateClassTable();
                    break;
                case 3:
                    transfer.Scale(myZbuffer.getModel().vertex, 1.1, 1.1, 1.1, myZbuffer.getModel().vertexCount);
                    commandType = 0;
                    myZbuffer.clearAll();
                    myZbuffer.CreateClassTable();
                    break;
            }
            break;
        case 'd':
            switch (commandType) {
                case 1:
                    transfer.Rotate(myZbuffer.getModel().vertex, 0, -10, 0, myZbuffer.getModel().vertexCount);
                    commandType = 0;
                    myZbuffer.clearAll();
                    myZbuffer.CreateClassTable();
                    break;
                case 2:
                    transfer.Translate(myZbuffer.getModel().vertex, 10, 0, 0, myZbuffer.getModel().vertexCount);
                    commandType = 0;
                    myZbuffer.clearAll();
                    myZbuffer.CreateClassTable();
                    break;
                case 3:
                    transfer.Scale(myZbuffer.getModel().vertex, 1.1, 1.1, 1.1, myZbuffer.getModel().vertexCount);
                    commandType = 0;
                    myZbuffer.clearAll();
                    myZbuffer.CreateClassTable();
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
        case 'v':
            CVertex cameraPoint;
            cameraPoint.x = cameraPoint.y = 0;
            cameraPoint.z = 100;
            transfer.Perspective(myZbuffer.getModel().vertex, cameraPoint, 1.01, myZbuffer.getModel().vertexCount,perspectFlag);
            perspectFlag = !perspectFlag;
            myZbuffer.clearAll();
            myZbuffer.CreateClassTable();
            break;
        default:
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


