
#include "include/player.hpp"

void loadForms()
{
    /* load shapes from form.txt (add them to the cassical forms)*/
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
    loadForms(); /*
    form.clear();
    form.push_back({{10,10},{8,8}});*/
    /*std::vector <std::vector <int> > newF;
    newF.push_back({{0,0}});
    form = newF;*/
   // for (auto x : form) {for(auto p:x){std::cout << p.x << std::endl;}}
   // nb_kinds = 1;
   // loadForms(); // l'utilisateur peut mettre ses formes desire
     Player play(10,20,10,SOLO); // (Width, Height, starting Level,game mode)
    // if you want multi joueur change solo to p2,p3 or p4
    //play.random(); to ask the manager for a game
    //play.joinRoom(); to join your friends in a room
    //play.createRoom(); to create a room
    // if no method is called before run it will still display
    //play.random();
         play.run(); 
   

    //LocalGame local(10,20);
    //LocalGame loadedGame("icardo.txt");
    //loadedGame.save("saving.txt");

    return 0;
}


