#include "manager.hpp"

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>


Manager::Manager()
{
    socket.bind(port);
}




void Manager::run()
{
    /*! \brief Infinite loop waiting for players
    */
    while(1)
	{
		std::cout << "waiting...\n";
		// wait for a player to connect
		int askedLevel;
		std::size_t received = 0;
		sf::IpAddress sender;
		socket.receive(&askedLevel, sizeof(askedLevel), received, sender, port);
        int32_t buffer;
		if (waitingList[askedLevel]!="") 
        {
            // if there's already somebdy waiting connect them 
            sf::IpAddress toSendIp = waitingList[askedLevel];
            buffer = toSendIp.toInteger();
            waitingList[askedLevel] = "";
        } 
        else
        {
            // if no one is waiting put him in the waiting list at his level
            waitingList[askedLevel] = sender.toString();
            buffer = 0;
        }
        socket.send(&buffer, sizeof(buffer), sender, port);
        
	}
    
}



