#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <vector>
#include <map>
#include <string.h>
#include <iostream>
#include <SFML/Network.hpp>

class Manager 
{
    private:
    std::map <int,std::string> waitingList; 
    sf::UdpSocket socket;
    unsigned short port = 8080;

    public:

    void run();
    Manager();
    ~Manager(){}
};




#endif