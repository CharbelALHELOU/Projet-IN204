
#include "include/player.hpp"


int main ( int nargs, char* argv[])
{
    Player play(10,20,10,P2); // (Width, Height, starting Level)
    // if you want multi joueur change solo to p2,p3 or p4
    //play.random(); to ask the manager for a game
    //play.joinRoom(); to join your friends in a room
    //play.createRoom(); to create a room
    // if no method is called before run it will still display
    play.run(); 

    //LocalGame local(10,20);
    //LocalGame loadedGame("icardo.txt");
    //loadedGame.save("saving.txt");

    return 0;
}


