
#include "include/player.hpp"

void loadForms()
{
    /* load shapes from form.txt (add them to the classical forms)*/
    char x;
    std::ifstream inFile;
    inFile.open("form.txt");
    if (!inFile) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
    std::vector<Point> newForm;
    int i =0;
    Point toAdd;
    while (inFile >> x)
    {
        if (x=='/') { form.push_back(newForm); newForm.clear();}
        else
        {
            if (i == 1) 
            {
                toAdd.y = (x-'0');
                newForm.push_back(toAdd);
            }
            else
            {
                toAdd.x = (x-'0');
            }
            i = (i+1)%2;
        }
    }
    nb_kinds = form.size();
}

int main ( int nargs, char* argv[])
{   
    loadForms(); 
    while (1)
    {
        std::cout << " Insert nbre of players: \n (0 to quit)\n";
        int nb_players,W,H,level;
        std::cin >> nb_players;
        if (nb_players == 0) break;
        std::cout << "W: \n";
        std::cin >> W;
        std::cout << "H: \n";
        std::cin >> H;
        std::cout << "Level: \n";
        std::cin >> level;
        nb_kinds = form.size();
        Player play(W,H,level, (mode) (nb_players-1));
        if (nb_players > 1)
        {
            nb_kinds=7; // everyone should have the same forms
            std::cout << "what mode:\n 1 create room\n 2 join room\n 3 play random\n";
            int m;
            std::cin >> m;
            std::string ipOfServer;
            switch (m)
            {
            case 1:
                play.createRoom();
                break;
            case 2:
                std::cout << "Server IP:\n";
                std::cin >> ipOfServer;
                play.setServerIp(ipOfServer);
                play.joinRoom();
                break;
            case 3:
                play.setServerIp(""); //insert here the ip of the manager
                play.random();
                break;
            default:
                std::cout << "WRONG!";
                break;
            }
        }
        play.run();
        std::cout << "WANNA PLAY AGAIN \n";
    }
    std::cout << "GOODBYE!\n";
    return 0;
}


