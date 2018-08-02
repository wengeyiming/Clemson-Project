

//
//                Programming Assignment #1
//
//                  Victor Zordan
//
//
//
/***************************************************************************/

/* Include needed files */
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>


#define WIDTH 500
#define HEIGHT 500


class Point
{
public:
    int x, y;
    void setxy(int _x, int _y) {
        x = _x;
        y = _y;
    }
};


static int pointNumber = 0;
const int maxPointNumber = 4;
const int animationSpeed = 100;
static Point points[maxPointNumber];
int x_last,y_last,minIndex;
bool chooseFlag = true;
bool animation = false;
Point oldPoint;
Point newPoint;

/***************************************************************************/

void init_window()
/* Clear the image area, and set up the coordinate system */
{
    
    /* Clear the window */
    glClearColor(0.0,0.0,0.0,0.0);
    glShadeModel(GL_SMOOTH);
    glOrtho(0,WIDTH,0,HEIGHT,-1.0,1.0);
}



/***************************************************************************/

void write_pixel(int x, int y, double intensity)
/* Turn on the pixel found at x,y */
{
    glColor3f (intensity, intensity, intensity);
    glBegin(GL_POINTS);
    glVertex3i( x, y, 0);
    glEnd();
}


/***************************************************************************/

void DDALineSegment(Point p1, Point p2)
{
    int x0 = p1.x;
    int y0 = p1.y;
    int x1 = p2.x;
    int y1 = p2.y;
    float x = 0.0;
    float y = 0.0;
    float m = 0.0;
    float dx = x1 - x0;
    float dy = y1 - y0;
    write_pixel(x0, y0, 1.0);
    if (dx != 0) {
        m = dy/dx;
        if (m <= 1 && m >= -1) {
            y = y0;
            for (x = x0; x <= x1; x++) {
                write_pixel(x, int(y+0.5), 1.0);
                y += m;
            }
        }
        else {
            m = 1/m;
            x = x0;
            for (y=y0; y<=y1; y++) {
                write_pixel(int(x+0.5), y, 1.0);
                x += m;
            }
        }
    }
    else {
        int x = x0;
        int y = 0;
        y = (y0 <= y1) ? y0 : y1;
        int d = fabs((double) (y0 - y1));
        while (d >= 0) {
            write_pixel(x, y, 1.0);
            y++;
            d--;
        }
    }
}

/***************************************************************************/


Point setBezier(Point p1, Point p2, Point p3, Point p4,double t) {
    Point p;
    double a1 = pow((1 - t), 3);
    double a2 = pow((1 - t), 2) * 3 * t;
    double a3 = 3 * t*t*(1 - t);
    double a4 = t*t*t;
    p.x = a1*p1.x + a2*p2.x + a3*p3.x + a4*p4.x;
    p.y = a1*p1.y + a2*p2.y + a3*p3.y + a4*p4.y;
    return p;
}

void playAnimation() {
    float oldPointX = oldPoint.x;
    float oldPointY = oldPoint.y;
    float newPointX = newPoint.x;
    float newPointY = newPoint.y;
    float k = (newPointY - oldPointY) / (newPointX - oldPointX);
    float b = ((newPointX * oldPointY) - (oldPointX * newPointY)) / (newPointX - oldPointX);
    float tempX = points[minIndex].x;
    float tempY = points[minIndex].y;
    //printf(" k,b is %f,%f\n", k,b);
    Point tempPoint;
    if(newPointX > oldPointX) {
        //printf("1\n");
        if(tempX + ((newPointX - oldPointX) / animationSpeed) < newPointX) {
            tempX = tempX + (newPointX - oldPointX) / animationSpeed;
            float tempY = k * tempX + b;
            tempPoint.setxy(tempX, tempY);
            points[minIndex] = tempPoint;
            //printf(" tempx,tempy is %f,%f\n", tempX,tempY);
        } else {
            animation = false;
            points[minIndex] = newPoint;
        }
    } else if(newPointX < oldPointX) {
        //printf("2\n");
        if(tempX - ((oldPointX - newPointX) / animationSpeed) > newPointX) {
            tempX = tempX - (oldPointX - newPointX) / animationSpeed;
            float tempY = k * tempX + b;
            tempPoint.setxy(tempX, tempY);
            points[minIndex] = tempPoint;
            //printf(" tempx,tempy is %f,%f\n", tempX,tempY);
        } else {
            animation = false;
            points[minIndex] = newPoint;
        }
    } else {
        //printf("3\n");
        if(newPointY < oldPointY) {
            if(tempY - ((oldPointY - newPointY) / animationSpeed) > newPointY) {
                tempY = tempY - (oldPointY - newPointY) / animationSpeed;
                tempPoint.setxy(newPointX, tempY);
                points[minIndex] = tempPoint;
            } else {
                animation = false;
                points[minIndex] = newPoint;
            }
        } else {
            if(tempY + ((newPointY - oldPointY) / animationSpeed) < newPointY) {
                tempY = tempY + (newPointY - oldPointY) / animationSpeed;
                tempPoint.setxy(newPointX, tempY);
                points[minIndex] = tempPoint;
            } else {
                animation = false;
                points[minIndex] = newPoint;
            }
        }
    }
    //printf(" tempPointx,tempPointy is %f,%f\n", tempPoint.x,tempPoint.y);
}




void display ( void )   // Create The Display Function
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       // Clear Screen
    
    write_pixel(x_last,y_last,1.0);//<-you can get rid of this call if you like
    // CALL YOUR CODE HERE
    if(pointNumber == maxPointNumber) {
        for(int i = 0; i < maxPointNumber; i++) {
            write_pixel(points[i].x, points[i].y, 1.0);
        }
        if(animation) {
            clock_t start = clock();
            while(clock() - start < 0.02 * CLOCKS_PER_SEC) ;
            playAnimation();
        }
        
        Point p_current = points[0];
        for (double t = 0.0; t <= 1.0; t += 0.001) {
            Point P = setBezier(points[0], points[1], points[2], points[3], t);
            DDALineSegment(p_current,P);
            p_current = P;
        }
    }
    glutSwapBuffers();                                      // Draw Frame Buffer
}

/***************************************************************************/
void mouse(int button, int state, int x, int y)
{
    /* This function I finessed a bit, the value of the printed x,y should
     match the screen, also it remembers where the old value was to avoid multiple
     readings from the same mouse click.  This can cause problems when trying to
     start a line or curve where the last one ended */
    if(state == GLUT_DOWN) {
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
        
        if(pointNumber < maxPointNumber) {
            points[pointNumber].setxy(x, y);
            pointNumber++;
        } else {
            if(chooseFlag) {
                minIndex = 0;
                int minDistinct = pow(x - points[0].x, 2) + pow(y - points[0].y, 2);
                for(int i = 1; i < maxPointNumber; i++) {
                    int distinct = pow(x - points[i].x, 2) + pow(y - points[i].y, 2);
                    if(distinct < minDistinct) {
                        minDistinct = distinct;
                        minIndex = i;
                    }
                }
                chooseFlag = false;
                oldPoint = points[minIndex];
                //printf(" minIndex is %d\n", minIndex);
            }
            else {
                newPoint.setxy(x, y);
                chooseFlag = true;
                animation = true;
            }
        }
    }
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
        default:
            break;
    }
}
/***************************************************************************/


int main (int argc, char *argv[])
{
    /* This main function sets up the main loop of the program and continues the
     loop until the end of the data is reached.  Then the window can be closed
     using the escape key.                        */
    
    glutInit            ( &argc, argv );
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize  ( 500,500 );
    glutCreateWindow    ( "Computer Graphics" );
    glutDisplayFunc     ( display );
    glutIdleFunc        ( display );
    glutMouseFunc       ( mouse );
    glutKeyboardFunc    ( keyboard );
    init_window();                           //create_window
    glutMainLoop        ( );                 // Initialize The Main Loop
    
}


