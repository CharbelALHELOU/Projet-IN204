#include "../include/block.hpp"

#include <iostream>
#include <algorithm>


// les formes possibles
std::vector<Point> form[total_kinds]=
{
    // first point is the rotation ref
    {{0,0},{0,1},{1,0},{1,1}},

    {{0,1},{0,0},{0,2},{0,3}},

    {{0,1},{0,0},{1,2},{0,2}},

    {{1,1},{1,0},{1,2},{0,2}},

    {{1,1},{0,1},{1,0},{0,2}},

    {{1,1},{0,0},{0,1},{1,2}},

    {{1,0},{0,0},{2,0},{1,1}},

    {{1,1},{0,1},{1,0},{1,2},{2,1}}
};



Block::Block (Point pos, Color c, Kind k ): position(pos), color(c), kind(k),rotation(0)
{
    /*! takes position color and kind
    */
    int minx= form[kind].size();
    int miny= form[kind].size();
    for (size_t i = 0; i < form[kind].size() ; i++)
    {
        shape.push_back(form[kind][i]);
        dims.x = std::max (form[kind][i].x , dims.x);
        minx = std::min (form[kind][i].x ,minx);
        miny = std::min (form[kind][i].y ,miny);
        dims.y = std::max (form[kind][i].y , dims.y);
    }
    if (minx!=0) 
    {
        dims.x-= minx;
        for (unsigned i=0; i<shape.size();i++) shape[i].x-=minx;
        position.x+=minx;
    }
    if (miny!=0) 
    {
        dims.y-= miny;
        for (unsigned i=0; i<shape.size();i++) shape[i].y-=miny;
        position.y+=miny;
    }
}


Block::Block (Point pos, Color c, Kind k, int rot): Block(pos,c,k)
{
    /*! \brief akes position color, kind and rotation 
    * Useful for data exchange in multiplayer mode
    */
    for (int j=0; j<rot; j++)
    {
        dims = {0,0};
        Point p=shape[0];
        int minx= shape.size();
        int miny= shape.size();
        for (size_t i=1; i < shape.size() ; i++)
            {
                int x = p.y +p.x - shape[i].y;
                int y = -p.x + p.y +shape[i].x ;
                shape[i].x = x;
                shape[i].y =y ;
                dims.x = std::max (x , dims.x);
                minx = std::min (x,minx);
                miny = std::min (y ,miny);
                dims.y = std::max (shape[i].y , dims.y);
            } 
        if (minx!=0) 
        {
            dims.x-= minx;
            for (unsigned i=0; i<shape.size();i++) shape[i].x-=minx;
        }
        if (miny!=0) 
        {
            dims.y-= miny;
            for (unsigned i=0; i<shape.size();i++) shape[i].y-=miny;
        }
    }
}


void Block::rotate()
{
    /*! \brief Rotate the block clockwise by taking the first pt of the form as ref
    */
    if (kind == square) return; // don't rotate if it s a square (just a detail can be removed)
    rotation = (rotation+1)%4; // increment rotation
    dims = {0,0};
    Point p=shape[0]; // set the first point in block as ref
    int minx= shape.size();
    int miny= shape.size();
    for (size_t i=1; i < shape.size() ; i++)
        {
            int x = p.y +p.x - shape[i].y;
            int y = -p.x + p.y +shape[i].x ;
            shape[i].x = x;
            shape[i].y =y ;
            dims.x = std::max (x , dims.x);
            minx = std::min (x,minx);
            miny = std::min (y ,miny);
            dims.y = std::max (shape[i].y , dims.y);
        } 
    // rearrange to respect the concept
    if (minx!=0) 
    {
        dims.x-= minx;
        for (unsigned i=0; i<shape.size();i++) shape[i].x-=minx;
        position.x+=minx;
    }
    if (miny!=0) 
    {
        dims.y-= miny;
        for (unsigned i=0; i<shape.size();i++) shape[i].y-=miny;
        position.y-=miny;
    }
}

void Block::unrotate()
{
    /*! \brief Rotate the block counter clockwise by taking the first pt of the form as ref
    */
    rotation = (rotation-1)%4;
    rotation %= 4;
    dims = {0,0};
    Point p=shape[0];
    int minx= shape.size();
    int miny= shape.size();
    for (size_t i=1; i < shape.size() ; i++)
        {
            int x = shape[i].y -p.y +p.x;
            int y = p.x + p.y -shape[i].x ;
            shape[i].x = x;
            shape[i].y =y ;
            dims.x = std::max (x , dims.x);
            minx = std::min (x,minx);
            miny = std::min (y ,miny);
            dims.y = std::max (shape[i].y , dims.y);
        } 
    if (minx!=0) 
    {
        dims.x-= minx;
        for (unsigned i=0; i<shape.size();i++) shape[i].x-=minx;
        position.x+=minx;
    }
    if (miny!=0) 
    {
        dims.y-= miny;
        for (unsigned i=0; i<shape.size();i++) shape[i].y-=miny;
        position.y-=miny;
    }
}


void Block::moveHorz ( int dx )
{
    /*! \brief Move the block horiz left =-1 right=1
    */
    position.x += dx;
}

void Block::goDown ()
{ 
    /*! \brief Make the block fall downward
    */
    position.y-=1;
    
}


std::vector <int> Block::getData()
{
    /*! \brief returns blocks data
    * useful for data exchange between games
    */
    std::vector<int> data ={position.x,position.y,color,kind,rotation};
    return(data);
}
