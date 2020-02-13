#include "../include/game.hpp"


sf::Color Colors[8] = {sf::Color(212, 41, 41) , sf::Color(38, 223, 63) , sf::Color(38, 88, 223) , sf::Color(38, 205, 223) , sf::Color(223, 38, 131) , sf::Color(223, 223, 38), sf::Color(255, 255, 255), sf::Color(21, 21, 21, 200)};

Game::Game(int w, int h): W(w), H(h)
    {
        rectSize = 15;
        nextBPos = {W+3,3};
        score =0;
        level=0;
        delay = 1000000 -(level*60000); //delay at level 0
        gameBoard = Board {w,h}; 
        nextBlock = {nextBPos,RED,I};
        if (!texture.loadFromFile(textureName)) std::cout << "NOT LOADED\n";
        rectSize = texture.getSize().y / (H+1);
        windowSize = {(int)(W*rectSize+texture.getSize().x),(int)(texture.getSize().y-texture.getSize().y % (H+1))};
    }



void Game::updateScore(int nblines)
{
    /*! \brief Update the score fonction depending on the level
    */
    if(nblines>4){
        std::cout<<"error with number of deleted lines (greater than 4)"<<std::endl;
        return;
    }

    linescleared+=nblines;
    // increment level every 4 lines cleared
    if(linescleared>=4){
        incrementLevel();
        linescleared=linescleared%4;
    }
    //FIXME
    score+=scoretable[nblines]*(level+1); //score is proportionnal to the game level
}

void Game::incrementLevel()
{
    /*! \brief Increment the levels after a certain number of lines deleted
    */
    if(level>=15){}
    else{
        level+=1;
        //we should reduce delay by 60000 at each level up
        delay-=60000;
        }
        //check if negative ->error
}

void Game::setLevel(int givenlevel)
{
    /*! \brief Sets level at givern level
    */
    //check boundaries of givenlevel
    level=givenlevel;
    delay = 1000000 -(givenlevel*60000); //delay at level 0

}

bool Game::advance(int dx, bool dy, bool rt)
{
    /* \brief Advance the game according to dx: horiz movt dy : true for go down and rt: true to rotate
    */
    if (gameBoard.isFull()) return false;
    if (rt) gameBoard.rotate();
    if (dx !=0 ) gameBoard.moveBlock(dx);
    if (dy)
    {
        if (!gameBoard.advance()) //landed
            {

                srand(time(NULL));
                //score+=20; 
                int y=gameBoard.getCurrentBlock().getPosistion().y;
                unsigned  numb= gameBoard.getCurrentBlock().getDimensions().y;
                nextBlock.setPosition({W/2-nextBlock.getDimensions().x/2,H+nextBlock.getDimensions().y});
                gameBoard.land(nextBlock);
                nextBlock = {nextBPos,(Color)(rand()%6),(Kind)(rand()%nb_kinds)};
                //score += gameBoard.deleteLines(y,numb)*100;
                updateScore( gameBoard.deleteLines(y,numb) ); //considering at level zero. //FIXME
            }
    }
    return true;

}

void Game::reset()
{
    /*! \brief Reset the game
    */
    srand(time(NULL));
    gameBoard = {W,H};
}

void Game::playingScreen(sf::RenderWindow* window, Point position)
{   
    /*! \brief Prints the playing screen
    */
    sprite.setTexture(texture);
     //print sidebar
    sprite.setPosition((W)*rectSize+position.x,position.y);
    window->draw(sprite);
    float border=1.1;
    std::vector< std::vector<int> > grid = gameBoard.getGrid();
    
    sf::RectangleShape s({(float) (W*rectSize),(float) rectSize});;
    sf::RectangleShape shadowsR({rectSize-border,rectSize-border}) ;

    //floor rectangle
    s.setFillColor(sf::Color(40,40,40)); //dark gray
    s.setPosition({(float) position.x,(float) (H*rectSize+position.y)});
    window->draw(s);

    s.setSize({rectSize-border,rectSize-border});

    shadowsR.setOutlineThickness(1);
    shadowsR.setOutlineColor(sf::Color(0,0,0,50)); //Dark outline
    s.setOutlineThickness(1);
    s.setOutlineColor(sf::Color(31, 31, 31)); //Dark outline

    Block currentBlock = gameBoard.getCurrentBlock();
    Point p= currentBlock.getPosistion();
    while (gameBoard.advance()) {} // move the block
    Point pAfter= gameBoard.getCurrentBlock().getPosistion(); // and get the shadow position
    gameBoard.setCurrentBlock(currentBlock); // restore old block
    sf::Color currentColor = Colors[currentBlock.getColor()];
    s.setFillColor(currentColor);
    shadowsR.setFillColor(sf::Color(currentColor.r,currentColor.g,currentColor.b,90));

    //print current Block and its shadow
    for (unsigned i = 0; i < currentBlock.getShape().size(); i++)
        {
            s.setPosition(position.x + (p.x+ currentBlock.getShape()[i].x)*rectSize+1,position.y+ (H- p.y + currentBlock.getShape()[i].y-1)*rectSize);
            shadowsR.setPosition(position.x + (pAfter.x+ currentBlock.getShape()[i].x)*rectSize+1,position.y+ (H- pAfter.y + currentBlock.getShape()[i].y-1)*rectSize);
            window->draw(shadowsR);
            window->draw(s);
        }

    //print score text
    sf::Font font;
    sf::Text scoretxt;

    if(!font.loadFromFile("fonts/VeraSeBd.ttf")) std::cout << "error loading font\n";
    scoretxt.setFont(font);
    scoretxt.setString(std::to_string(score));
    scoretxt.setCharacterSize(20);
    scoretxt.setPosition(position.x+W*rectSize+20, position.y+210); //score position on sidebar
    window->draw(scoretxt);

    //print level text
    sf::Text leveltxt;
    leveltxt.setFont(font);
    leveltxt.setString(std::to_string(level));
    leveltxt.setCharacterSize(20);
    leveltxt.setPosition(position.x+W*rectSize+20, position.y+275);
    window->draw(leveltxt);

    //print the grid 
    for (size_t i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < W; j++)
            {   
                if (grid[i][j] != -1)
                {
                    s.setFillColor(Colors[grid[i][j]]);
                    s.setPosition({(float)(position.x+j*rectSize+1),(float)(position.y+(H-i-1)*rectSize)});       
                    window->draw(s);
                }
                
            }            
        }
    
    //print next block
    Point dim = nextBlock.getDimensions();
    float rectNext = 20; //size of next block at the side of the board 
    s.setSize({rectNext-border, rectNext-border});
    std::vector<Point> nextShape = nextBlock.getShape();
    Point pNext = nextBlock.getPosistion();
    s.setFillColor(Colors[nextBlock.getColor()]);
    for (unsigned i = 0; i < nextBlock.getShape().size(); i++)
        {
            s.setPosition(position.x+W*rectSize+50+nextShape[i].x*rectNext,position.y+(2+nextShape[i].y)*rectNext);
            window->draw(s);
        }
}




void Game::gameOverState(sf::RenderWindow* window, Point position)
{
    /*! \brief Prints a game over screen
    */
    //print sidebar
    sprite.setPosition((W)*rectSize+position.x,position.y);
    window->draw(sprite);

    //create game over text
    sf::Font font;
    sf::Text gameover;
    if(!font.loadFromFile("fonts/VeraSeBd.ttf")) std::cout << "error loading font\n";
    gameover.setFont(font);
    gameover.setString("GAME OVER!");
    gameover.setCharacterSize(15);
    gameover.setFillColor(sf::Color(231,91,91));
    gameover.setPosition(position.x+W*rectSize+19, position.y+40);
    window->draw(gameover);
    
    // print score text in red
    sf::Text scoretxt;

    if(!font.loadFromFile("fonts/VeraSeBd.ttf")) std::cout << "error loading font\n";
    scoretxt.setFont(font);
    scoretxt.setFillColor(sf::Color(231,91,91)); //red color
    scoretxt.setString(std::to_string(score));
    scoretxt.setCharacterSize(20);
    scoretxt.setPosition(position.x+W*rectSize+20, position.y+210);
    window->draw(scoretxt);

    //print level text in red
    sf::Text leveltxt;
    leveltxt.setFont(font);
    scoretxt.setFillColor(sf::Color(231,91,91)); //red color
    leveltxt.setString(std::to_string(level));
    leveltxt.setCharacterSize(20);
    leveltxt.setPosition(position.x+W*rectSize+20, position.y+275);
    window->draw(leveltxt);

    //grid turns gray
    float border = 1.1;
    sf::RectangleShape s ({(float) (W*rectSize), (float) rectSize});
    s.setFillColor(sf::Color(40,40,40)); //dark gray
    s.setPosition({(float) position.x, (float) (H*rectSize+position.y)});
    window->draw(s);

    s.setSize({rectSize-border,rectSize-border});

    s.setOutlineThickness(1);
    s.setOutlineColor(sf::Color(31, 31, 31)); //Dark outline
    std::vector<std::vector<int>> grid = gameBoard.getGrid(); 
    s.setFillColor(sf::Color(70,70,70));
    for (size_t i = 0; i < (grid.size()); i++)
    {
        for (int j = 0; j < W; j++)
        {         
            if (grid[i][j] != -1)
            {
                s.setPosition({(float)(position.x+j*rectSize+1), (float)(position.y+(H-i-1)*rectSize)});       
                window->draw(s);
            }
            
        }            
    }
    
}



void Game::print(sf::RenderWindow* window, Point position)
{
    /*! \brief Prints the game on a given window at a ceratin position (in pixels)
    */
    if (isGameOver()) gameOverState(window,position);
    else playingScreen(window,position);
    
}






void networkGame::getData(int* buffer)
{
    /*! \brief Put the data of the game in a buffer
    * Aims to send the data through a network
    */
    if (gameBoard.isFull()) {buffer[0]=-1; return;}
    buffer[0] = W;
    buffer[1]=H;
    buffer[2] = (int) gameBoard.getGrid().size();
    buffer[3] = score;
    int i=4;
    //TODO: one loop can do the job
    for (auto x : gameBoard.getCurrentBlock().getData()) {buffer[i] = x;i++;}
    for (auto x : nextBlock.getData()) {buffer[i] = x;i++;}
    for (auto z : gameBoard.getGrid())
    {
        for (auto y:z) {buffer[i]=y; i++;}
    }
}

void networkGame::loadData(int* buffer)
{
    /*! \brief Load data from a given buffer
    * Allows us to load data received on a network
    */
    W= buffer[0];
    H= buffer[1];
    int gridSize= buffer[2];
    score = buffer[3];
    gameBoard.setCurrentBlock({{buffer[4],buffer[5]},(Color) buffer[6], (Kind) buffer[7], buffer[8]});
    nextBlock = {{buffer[9],buffer[10]},(Color) buffer[11], (Kind) buffer[12],buffer[13]};
    std::vector < std::vector <int> > newGrid;
    for (int i=0; i < (gridSize*W); i+=W)
        {
            std::vector <int> row;
            for (int j=0; j<W;j++)
                {
                    row.push_back(buffer[14+i+j]);
                }
            newGrid.push_back(row);
        }
    
    gameBoard.load(newGrid);
}


////////////////////////////////////////////////////////////////////////////////////////


localGame::localGame (const std::string& fileName)
{// TODO : Check if the file is indeed a Game file
    /*! \brief Load a game from a local file
    */
    int x;
    int i=0;
    std::vector<std::vector<int>> loadingGrid;
    Point currentBlockData;
    std::ifstream inFile;
    inFile.open("./saved games/" + fileName);
    if (!inFile) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
    while (inFile >> x)
    {
        if (i==0) W=x;
        else if (i==1) H=x;
        else if (i==2) score=x;
        else if (i==3) currentBlockData.x = x; // load currentBlock color
        else if (i==4) currentBlockData.y = x; // load currentBlock kind
        else {
            std::vector<int> loadingLine;
            for (int j=0; j<W; j++)
            {
                loadingLine.push_back(x%10-1) ;
                x /= 10;
            }
            loadingGrid.push_back(loadingLine);
        }
        i++;
    }
    inFile.close();
    gameBoard = {W,H};
    gameBoard.setCurrentBlock({{W/2,-10},(Color) currentBlockData.x, (Kind) currentBlockData.y});
    int positionY = gameBoard.getCurrentBlock().getDimensions().y;
    gameBoard.setCurrentBlock({{W/2,H+positionY},(Color) currentBlockData.x, (Kind) currentBlockData.y});
    gameBoard.load(loadingGrid);
    nextBlock = {{0,0},(Color)(rand()%6),(Kind)(rand()%nb_kinds)};
}



void localGame::save(std::string name)
{
    /*! \brief Save the game in a file localy
    */
    std::ofstream outFile;
    outFile.open("saved games/"+name+".txt");
    outFile << W << std::endl;
    outFile << H << std::endl;
    outFile << score << std::endl;
    outFile << gameBoard.getCurrentBlock().getColor() << std::endl;
    outFile << gameBoard.getCurrentBlock().getKind() << std::endl;
    for (size_t i=0; i< gameBoard.getGrid().size(); i++)
    {
        for (int j=W; j>0; j--)
        {  
            outFile << (gameBoard.getGrid()[i][j-1]+1);
        }
        outFile << "\n";
    }
    outFile.close();
}



void localGame::run()
{
    /*! \brief Runs a Local game
    * First idea just to test (no levels same delay )
    */
	sf::Clock clock;
	sf::RenderWindow window (sf::VideoMode((W)*rectSize+texture.getSize().x,texture.getSize().y*texture.getSize().y % (H+1)), "Tetris");
	sf::Event event;
	while ( window.isOpen() )
	{
		int elapsed = clock.getElapsedTime().asMicroseconds();
		int dx=0;
		bool rt=false;
		bool dy=false;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {window.close();}
			if (event.type == sf::Event::KeyPressed)
					{
					if (event.key.code == sf::Keyboard::Left) dx=-1;
					if (event.key.code == sf::Keyboard::Right) dx=1;
					if (event.key.code == sf::Keyboard::Down) {dy=true; clock.restart();}
					if (event.key.code == sf::Keyboard::Up) rt= true;
					}
		}
		if (elapsed > delay ) {advance(0,true,false); clock.restart();dy=false;}
        advance(dx,dy,rt);
		window.clear(sf::Color::White);
        print(&window,{0,0});
		window.display();
	}
}


