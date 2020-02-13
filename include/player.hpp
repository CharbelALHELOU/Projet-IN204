#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <SFML/Network.hpp>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <thread> 
#include "game.hpp"

#define SA struct sockaddr 


enum status {SERVER, CLIENT, NEITHER};
enum mode {SOLO, P2, P3, P4};


class Player
{
    protected:
    int playerID;
    std::vector <networkGame> games ;
    std::vector <int> exitedPlayer;
    int W,H;
    status myStatus;
    int sizeBuffer;
    sf::IpAddress serverIP = "147.250.8.143" ;
    std::map<sf::IpAddress,int> otherPlayersIP;
    sf::UdpSocket socket;
    unsigned short port=8080;
    

    public:

    void random();
    void joinRoom();
    void createRoom();
    bool communicate();
    void run();

    Player(int w, int h, mode m );
    Player(int w, int h, int startlevel, mode m);
    Player(){}
    ~Player() {}
};


#endif
