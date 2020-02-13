#ifndef BOARD_HPP
#define BOARD_HPP


#include "block.hpp"

class Board
{
  /*! \brief The landed Blocks (that aren't blocks anymore) and the currently falling block
  */
  private:

  int W,H;
  Block currentBlock;
  std::vector< std::vector<int> > grid;


  public:

  void land(Block next);
  bool advance();
  bool moveBlock(int dx);
  void rotate();
  int deleteLines(int start, unsigned numb);
  bool isFull();
  void load(std::vector< std::vector<int> > b);

  void setCurrentBlock( Block b) {currentBlock=b;}
  Block getCurrentBlock() {return (currentBlock);}
  std::vector< std::vector<int> > getGrid() {return grid;}


  Board(int w, int h);
  Board(){}
};


#endif