#ifndef BLOCK_HPP
#define BLOCK_HPP
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <vector>
#include <iostream>
#include "basic_types.hpp"
#define total_kinds 8
#define nb_kinds 7



enum Color {RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, BLACK} ;
enum Kind {square, I, L, mirL, N, mirN, T, cross};

class Block
{
    public:

    void moveHorz(int dx);
    void goDown();
    void rotate();
    void print (int printSize, Point pos, sf::RenderWindow* window);

    Color getColor() {return color;}
    Kind getKind() {return kind;}
    std::vector<Point> getShape() {return shape;}
    Point getPosistion() {return position;}
    Point getDimensions() {return dims;}
    int getRoatation() {return rotation;}

    Block (Point pos, Color c, Kind k );
    Block (Point pos, Color c, Kind k, int rot);
    Block (): Block ({0,0}, RED, square) {}
    ~Block(){}

    void unrotate();
    std::vector <int> getData();

    void setPosition(Point newPos) {position=newPos;}
    
    protected:
    Point dims;
    Point position;
    std::vector<Point> shape;
    Color color;
    Kind kind;
    int rotation;
};

class Square: public Block
{
    Square(Point pos, Color c): Block(pos,c,square){}
    Square(Point pos, Color c,int rot): Block(pos,c,square,rot){}
    Square():Block(){}
    ~Square(){}
};

#endif