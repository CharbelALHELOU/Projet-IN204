#include "../include/board.hpp"

// H et W sont des nmbres de cases pas de pixels

Board::Board(int w, int h): W(w), H(h)
{
    srand(time(NULL));
    currentBlock = {{0,0},(Color) (rand()%6), (Kind) (rand()%nb_kinds)};
    currentBlock.setPosition({(W-currentBlock.getDimensions().x)/2, H+currentBlock.getDimensions().y});
    grid.clear();
    grid.reserve(H);
}

void Board::land(Block next)
{
    /*! \brief Lands the current block ad switch to an other one
    *  Adds lines to the grid if needed and add the current block to it, next takes the place of the landed block
    */
    int y = currentBlock.getPosistion().y;
    Point dims = currentBlock.getDimensions();
    int x = currentBlock.getPosistion().x;
    int c=(int)currentBlock.getColor();
    std::vector<Point> shape = currentBlock.getShape();
    
    currentBlock = next;
    
    int linesToAdd = y-grid.size()+1; // was H-y-grid.size()+1
    if (linesToAdd > 0)
    {
        std::vector<int> col(W,-1);
        for (int i = 0; i < linesToAdd; i++)
        {
            grid.push_back(col);
        }
    }
    for (unsigned i=0; i < shape.size(); i++)
    {
        grid[y-shape[i].y][shape[i].x +x]=c;//here too
        
    }
}



bool Board::advance()
{
    /*! \brief Moves the current block downward if possible
    * Returns true if the block have been moved and false if not (it should be landed)
    */
    int y = currentBlock.getPosistion().y;
    if ((y-currentBlock.getDimensions().y-1) < 0) return false;
    Block saveCurrentBlock= currentBlock;
    currentBlock.goDown(); 
    if (!moveBlock(0)) {currentBlock = saveCurrentBlock; return false;}
    return true;  
}


bool Board::moveBlock(int dx)
{
    /*! \brief Moves the current block left (-1) or right(1)
    * Returns true if the block have been moved and false if not
    * if dx =0 it return if current block position is free in the grid
    */
    int x = currentBlock.getPosistion().x;
    int y = currentBlock.getPosistion().y;
    if  ( (x + dx ) < 0 ) return false ;

    if ( (x+ currentBlock.getDimensions().x + dx) > (W-1) )  return false ;

    if ( (y-currentBlock.getDimensions().y) < (int)(grid.size()) )
    {
        std::vector<Point> shape = currentBlock.getShape();
        for (unsigned i = 0; i < shape.size(); i++)
        {
            if ((y-shape[i].y) < ((int)(grid.size()))) //here
                {
                    if (grid[y-shape[i].y][x+shape[i].x+dx] != -1) return false;
                }
        }
    }

    currentBlock.moveHorz(dx);
    return true;
}

void Board::rotate()
{
    /*! \brief Rotates current Block if possible
    */
    currentBlock.rotate();
    if (!this->moveBlock(0)) currentBlock.unrotate();
}

int Board::deleteLines (int start, unsigned numb)
{
    /*! \brief Deletes full lines between start and start+numb
    * Used to scan the lines of a landed block
    */
    int deleted =0 ;
    for (unsigned i=0; i<(numb+1);i++)
    {
        bool isFull=true;
        for (int j=0; j<W ; j++)
        {
            if ( grid[start-i][j] == -1) isFull=false;
        }
        if (isFull) {grid.erase(grid.begin()+start-i); deleted++;}
    }
    return deleted; 
}

bool Board::isFull()
{
    /*! \brief Checks if the board is full
    */
    if (grid.size() >= ((size_t) H)) return true;
    return false;
}


void Board::load(std::vector<std::vector<int>> b)
{
    /*! \brief Loads a grid 
    */
    grid.clear();
    grid = b;
}


