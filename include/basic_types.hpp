#ifndef BASIC_TYPES_HPP
#define BASIC_TYPES_HPP

class Point
{
    public:
    int x,y;
    Point(): x(0), y(0) {}
    Point(int _x, int _y): x(_x), y(_y) {}
    Point(const Point &p) { x = p.x; y=p.y; }
    ~Point(){};
} typedef Point;


#endif