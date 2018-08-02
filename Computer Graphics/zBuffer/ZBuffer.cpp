
#include "ZBuffer.hpp"
#include <cstring>

using namespace std;

#define MINDEPTH -99999.0f
float bigY=0,bigX=0;

ZBuffer::ZBuffer() {
    myLoadobj.ImportObj(&my3DModel, FILE_PATH);
    polygonNum=my3DModel.faceCount;
    vertexNum=my3DModel.vertexCount;
}


void ZBuffer::CreateClassTable() {
    CFace* pface=my3DModel.face;
    CVertex* pVertex=my3DModel.vertex;
    int xx,yy;
    for (int i=0;i < my3DModel.vertexCount;i++)
    {
        xx=pVertex[i].x > 0 ? pVertex[i].x : -pVertex[i].x;
        yy=pVertex[i].y > 0 ? pVertex[i].y : -pVertex[i].y;
        if(xx>bigX)bigX=xx;
        if(xx>bigY)bigY=yy;
    }
    ClassPolTable polTable;
    ClassEdgeTable sideTable;
    
    for(int i=0;i < polygonNum;i++)
    {
        polTable.id=i;
        //ax+by+cz+d=0;
        int vdex=pface[i].vertexIndex[0]-1;
        float vx1=pVertex[vdex].x,vy1=pVertex[vdex].y,vz1=pVertex[vdex].z;
        vdex=pface[i].vertexIndex[1]-1;
        float vx2=pVertex[vdex].x,vy2=pVertex[vdex].y,vz2=pVertex[vdex].z;
        vdex=pface[i].vertexIndex[2]-1;
        float vx3=pVertex[vdex].x,vy3=pVertex[vdex].y,vz3=pVertex[vdex].z;
        
        polTable.a=(vy1-vy2)*(vz1-vz3)-(vy1-vy3)*(vz1-vz2);
        polTable.b=(vz1-vz2)*(vx1-vx3)-(vx1-vx2)*(vz1-vz3);
        polTable.c=(vx1-vx2)*(vy1-vy3)-(vx1-vx3)*(vy1-vy2);
        
        //skip the situation that projection is a line
        if(polTable.c == 0)continue;
        
        float minY,maxY;
        int vdex1,vdex2,zvdex;
        float x1,x2;
        int y1,y2;
        int pos;
        vdex1=pface[i].vertexIndex[0]-1;
        y1=pVertex[vdex1].y;
        x1=pVertex[vdex1].x;
        
        minY=maxY=y1;
        zvdex=vdex1;
        for(int j=1;j<=pface[i].num;j++)
        {
            if(j==pface[i].num)
                //the last face's point
                vdex2=pface[i].vertexIndex[0]-1;
            else
                vdex2=pface[i].vertexIndex[j]-1;
            
            y2=pVertex[vdex2].y;
            x2=pVertex[vdex2].x;
            
            if(y2 > maxY){
                maxY=y2;
                zvdex=vdex2;
            }
            if(y2 == maxY && x2 < pVertex[zvdex].x) {
                zvdex=vdex2;
            }
            if(y2 < minY) minY=y2;
            if(y1 > y2)
            {
                sideTable.x=x1;
                sideTable.dx=(x2-x1)/(y1-y2);//dx=-1/k
                sideTable.dy=y1-y2;
                sideTable.id=i;
                pos=int(y1);
                if(pos >= -winY && pos < winY) {
                    myListClassEdgeTable[pos+winY].push_back(sideTable);
                }
            }
            else if(y2 > y1)
            {
                sideTable.x=x2;
                sideTable.dx=(x2-x1)/(y1-y2);//dx=-1/k
                sideTable.dy=y2-y1;
                sideTable.id=i;
                pos=int(y2);
                
                if(pos >= -winY && pos < winY) {
                    myListClassEdgeTable[pos+winY].push_back(sideTable);
                }
            }
            x1=x2;y1=y2;
        }
        
        polTable.dy=maxY-minY;
        polTable.d=pVertex[zvdex].z;
        pos=int(maxY);
        if(pos >= -winY && pos < winY) {
            myListClassPolTable[pos+winY].push_back(polTable);
        }
        
    }
}

void ZBuffer::initBuffer()
{
    memset(myColorBuffer,0.0,sizeof(float)*2*winX*3);
    for(int i = -winX; i <= winX; i++)
    {
        myDepthBuffer[i+winX] = MINDEPTH;
    }
}


void ZBuffer::clearAll()
{
    
    for(int i = -winY; i < winY; i++) {
        myListClassPolTable[i+winY].clear();
        myListClassEdgeTable[i+winY].clear();
    }
    myListActPolTable.clear();
    myListActEdgeTable.clear();
}


void ZBuffer::addPolTable(int curY) {
    vector<ClassPolTable> listclassPolTable = myListClassPolTable[curY+winY];
    vector<ClassEdgeTable> listclassSideTable = myListClassEdgeTable[curY+winY];
    
    for(int i = 0;i < listclassPolTable.size();i++)
    {
        int id=listclassPolTable[i].id;
        ActEdgeTable sideTable;
        int k1=-1,k2=-1;
        for (int j = 0;j < listclassSideTable.size();j++)
        {
            if(listclassSideTable[j].id==id&&k1==-1)
            {
                k1=j;
                continue;
            }
            if(listclassSideTable[j].id==id&&k2==-1&&k1!=-1)
            {
                k2=j;
                break;
            }
        }

        if(k1 == -1||k2 == -1)
            continue;

        if(listclassSideTable[k1].x < listclassSideTable[k2].x||
           (listclassSideTable[k1].x == listclassSideTable[k2].x && listclassSideTable[k1].dx < listclassSideTable[k2].dx))
        {
            sideTable.xl=listclassSideTable[k1].x;
            sideTable.dxl=listclassSideTable[k1].dx;
            sideTable.dyl=listclassSideTable[k1].dy;
            sideTable.xr=listclassSideTable[k2].x;
            sideTable.dxr=listclassSideTable[k2].dx;
            sideTable.dyr=listclassSideTable[k2].dy;
        }
        else
        {
            sideTable.xl=listclassSideTable[k2].x;
            sideTable.dxl=listclassSideTable[k2].dx;
            sideTable.dyl=listclassSideTable[k2].dy;
            sideTable.xr=listclassSideTable[k1].x;
            sideTable.dxr=listclassSideTable[k1].dx;
            sideTable.dyr=listclassSideTable[k1].dy;
        }

        sideTable.id=id;
        sideTable.zl=listclassPolTable[i].d;
        sideTable.dzx=-listclassPolTable[i].a/listclassPolTable[i].c;
        sideTable.dzy=listclassPolTable[i].b/listclassPolTable[i].c;
        myListActEdgeTable.push_back(sideTable);
        myListActPolTable.push_back(listclassPolTable[i]);        
    }
}

void ZBuffer::updateActPolTable()
{
    list<ActPolTable>::iterator it = myListActPolTable.begin();
    //for each polygon: dy = dy-1, when dy <=0,erase it from table
    while (it != myListActPolTable.end())
    {
        if ((*it).dy <= 0)
        {
            it=myListActPolTable.erase(it);
        }
        else
        {
            (*it).dy--;
            it++;
        }
    }
}

void ZBuffer::updateActEdgeTable(int curY)
{
    list<ActEdgeTable>::iterator it;
    //for each edge: dyl=dyl-1,dyr=dyr-1,xl=xl+dx,xr=xr+dxr,zl=zl+dzl*dxl+dzy
    for(it = myListActEdgeTable.begin();it != myListActEdgeTable.end();)
    {
        float xl,xr;
        float z,dzx;
        unsigned int id;
        CFace* pface=my3DModel.face;
        id=(*it).id;
        z=(*it).zl;
        xl=(*it).xl;
        dzx=(*it).dzx;
        xr=(*it).xr;
        for (int x = (int)(xl+0.5);x <= (int)(xr+0.5);x++)
        {
            if(z > myDepthBuffer[x+winX])
            {
                myDepthBuffer[x+winX] = z;
                myColorBuffer[x + winX][0]=pface[id].col[0];
                myColorBuffer[x + winX][1]=pface[id].col[1];
                myColorBuffer[x + winX][2]=pface[id].col[2];
            }
            z+=dzx;
        }
        (*it).dyl--;
        (*it).dyr--;
        (*it).xl+=(*it).dxl;
        (*it).xr+=(*it).dxr;
        (*it).zl+=(*it).dzx*(*it).dxl+(*it).dzy;
        
        //left edge and right edge are both scanned
        if((*it).dyl <= 0 && (*it).dyr <= 0)
        {
            it=myListActEdgeTable.erase(it);
            continue;;
        } else {
            //left edge is scanned
            if((*it).dyl <= 0)
            {
                vector<ClassEdgeTable>::iterator i;
                int x=(*it).xl;
                for (i = myListClassEdgeTable[curY-1+winY].begin();i != myListClassEdgeTable[curY-1+winY].end();i++)
                {
                    if((*i).id==(*it).id && int((*i).x)==x)
                        break;
                }
                (*it).dxl=(*i).dx;
                (*it).dyl=(*i).dy;
            }
            //right edge is scanned
            if((*it).dyr <= 0)
            {
                vector<ClassEdgeTable>::iterator i;
                int x=(*it).xr;
                for (i=myListClassEdgeTable[curY-1+winY].begin();i!=myListClassEdgeTable[curY-1+winY].end();i++)
                {
                    if((*i).id==(*it).id && int((*i).x)==x)
                        break;
                }
                (*it).dxr=(*i).dx;
                (*it).dyr=(*i).dy;
            }
        }
        it++;
    }
    //modify Active polygon table;
    updateActPolTable();
}