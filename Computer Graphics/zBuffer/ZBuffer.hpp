

#ifndef ZBuffer_hpp
#define ZBuffer_hpp

#include <stdio.h>
#include <list>
#include "ObjAttribute.h"
#include "ObjLoader.hpp"

#define winX 500
#define winY 500
#define winZ 500


struct Color
{
    float r,g,b;
};

struct PolygonTable
{
    float a,b,c,d; // coefficients of the plane where polygon is on: ax+by+cz+d=0;
    unsigned int id;//polygon id
    int dy;//the number of (reset) scanline spanned by polygon
    Color col;//polygon color
    
};

typedef PolygonTable ClassPolTable;
typedef PolygonTable ActPolTable;


struct ClassEdgeTable
{
    float x; //upper end of edge's x coordinate
    float dx;//coordinate difference between two scanline(-1/k)
    int dy;//the number of scanline spanned by polygon
    unsigned int id;//id of the polygon which edge belongs to
    
};

struct ActEdgeTable
{
    float xl,dxl;//left intersection's x coordinate and x coordinate difference between two scanline on the left intersection's edge
    int dyl;//decrease 1 each time when handle one scanline, initial valve is the number of scanline intersect with the left intersection's edge
    float xr,dxr;//right, same with xl and dxl
    int dyr;//right, same with xl and dxl
    float zl;//the polygon's depth value where left intersection on
    float dzx;//depth increase value where go one pixel to right along the scanline,   dzx = -a/c
    float dzy;//depth increase value where go one pixel to down along the y axis,  dzy = b/c
    unsigned int id;//polygon id
};

class ZBuffer
{
public:
    ZBuffer();
    
    //create polygon table and edge table
    void CreateClassTable();
    
    //initial frame_buffer and z-buffer
    void initBuffer();
    
    //add new polygon which is associated with current scanline to the active edge table
    void addPolTable(int);
    
    void updateActEdgeTable(int);
    
    void updateActPolTable();
    
    //clear all class tables and active tables
    void clearAll();
    
    int getActPolNum()
    {
        return (int)myListActPolTable.size();
    }
    
    float myColorBuffer[2*winX][3];
    
    CModel getModel(){
        return my3DModel;
    }
    
private:
    ObjLoader myLoadobj;//obj file loading
    
    CModel my3DModel;
    
    //Class Polygon Table List
    vector<ClassPolTable> myListClassPolTable[2*winY];    
    //Class Edge Table List
    vector<ClassEdgeTable> myListClassEdgeTable[2*winY];
    //Active Polygon Table List
    list<ActPolTable> myListActPolTable;
    //Active Edge Table List
    list<ActEdgeTable> myListActEdgeTable;
    
    int polygonNum;
    int vertexNum;
    float myDepthBuffer[2*winX];
};


#endif /* ZBuffer_hpp */
