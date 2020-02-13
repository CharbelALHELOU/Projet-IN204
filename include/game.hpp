#ifndef GAME_HPP
#define GAME_HPP


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <thread>
#include "board.hpp"
#include "block.hpp"


class Game
{
protected:
    int W,H;
    int score; //actual player score;
    int scoretable[5] = {0, 40, 100, 300, 1200}; //table of level zero scores (scoretable(0) is bonus score when landed and no lines are deleted)
    int rectSize;
    int delay; //delay between two moves if by default at level 0 is 1000000 delay at level 15 is 100000
    int level;
    int linescleared=0; //counter of cleared lines per level
    Board gameBoard; // grid + currently falling block
    Block nextBlock; // next block !! could be a vector so we can keep some history and print more next blocks

    Point windowSize; // windows size
    std::string textureName = "pictures/bar2.jpg"; // bar to be printed at the side of the board
    sf::Texture texture;
    sf::Sprite sprite;
    Point nextBPos; // next block printing position so it can fit in the bar

    void playingScreen(sf::RenderWindow* window, Point position);
    void gameOverState(sf::RenderWindow* window, Point position);
    void updateScore(int nblines); //calculate score relatively to level and number of lines
    void incrementLevel();

public:

    void reset();
    bool isGameOver() { return (gameBoard.isFull());}
    bool advance(int dx, bool dy, bool rt);
    void print(sf::RenderWindow* window, Point position);

    void setTexture (std::string barName) {textureName= barName;texture.loadFromFile(barName);}
    Point getPrintData() {return windowSize;} //returns windows dimension
    int getScore() {return score;}
    void setLevel(int givenlevel);
    int getLevel() {return level;}

    int getDelay() { return delay;}
    Game(int w, int h); //create a new default Game 
    Game(int w, int h, int startlevel): Game(w,h) {
        setLevel(startlevel);
    }//create game with specific starting level
    Game(){}
    ~Game(){}
};



////// LOCAL GAME ////////////
class LocalGame: public Game
{
    /*! \brief Test of our first idea of how a local game should work not very useful 
    */
    public:

    void run(); 
    void save(std::string name);

    LocalGame(int w, int h): Game(w,h) {}
    LocalGame(int w, int h, int startlevel): Game(w,h,startlevel) {}
    LocalGame(const std::string& fileName);
    LocalGame(){}
    ~LocalGame(){}
};


////// NETWORK GAME ////////////
class NetworkGame: public Game
{
    /*! \brief A game that is able to exchange data (by loading an puting the data from and in a buffer)
    * The starting point of multiplayers mode
    */
    public:

    void getData(int* buffer);
    void loadData(int* buffer);

    NetworkGame(int w, int h): Game(w,h) {}
    NetworkGame(int w, int h, int startlevel): Game(w,h,startlevel) {} //specific level start
    NetworkGame(){}
    ~NetworkGame() {}
};



#endif