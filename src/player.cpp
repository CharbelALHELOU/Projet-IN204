#include "../include/player.hpp"

Player::Player(int w, int h, mode m): W(w), H(h)
{
	/*! \brief Constructor sets the level at 0
	*/
	nb_kinds = 7; // only uses the standard forms so we don 't have problems
	games.emplace_back(NetworkGame {w,h}); 
	if (m != SOLO) {for (int i=0; i<m;i++) games.emplace_back(NetworkGame {w,h}); }
    sizeBuffer= 15+w*h;
    myStatus = NEITHER;
	socket.bind(port);
}

Player::Player(int w, int h, int startlevel, mode m): W(w), H(h)
{
	/*! \brief Constructor sets the level at startlevel
	*/
	nb_kinds =7;
	games.reserve(m+1);
	games.emplace_back(NetworkGame {w,h,startlevel}); 
	if (m != SOLO) {for (int i=0; i<m;i++) games.emplace_back(NetworkGame {w,h,startlevel}); }
    sizeBuffer= 15+w*h;
    myStatus = NEITHER;
	socket.bind(port);
}

void Player::run()
{ 	
	/*! \*brief Launch the party on your computer
	*/
	sf::Clock clock;
	sf::RenderWindow window (sf::VideoMode(games[0].getPrintData().x*games.size(),games[0].getPrintData().y), "TETRIS IN204");
	sf::Event event;

	//load game sounds
	sf::Music startmusic;
    startmusic.openFromFile("sounds/music.wav");
    startmusic.setVolume(16.f);
    startmusic.setLoop(true);
    startmusic.play();	
	//load rotation music
	sf::SoundBuffer rot_buffer;
	rot_buffer.loadFromFile("sounds/block-rotate.wav");
	sf::Sound rotate_sound;
	rotate_sound.setBuffer(rot_buffer);
	rotate_sound.setVolume(25.f);
	//load game over game
	sf::SoundBuffer gameOverBuffer;
	gameOverBuffer.loadFromFile("sounds/gameover.wav");
	sf::Sound gameOverSound;
	gameOverSound.setBuffer(gameOverBuffer);
	gameOverSound.setVolume(16.f);
	games[0].setTexture("pictures/me.jpg");
	float p;
	bool first = true;
	//////////////// THE MAIN LOOP ////////////////////////////////////////
	while ( window.isOpen() )
	{
		p=1;
		int elapsed = clock.getElapsedTime().asMicroseconds();

		//TODO level increases every 6 lines cleared
		int dx=0;
		bool rt=false;
		bool dy=false;
		while (window.pollEvent(event)) //event is captured
		{
			if (event.type == sf::Event::Closed) {window.close();}
			if (event.type == sf::Event::KeyPressed)
					{
					if (event.key.code == sf::Keyboard::Left) dx=-1;
					if (event.key.code == sf::Keyboard::Right) dx=1;
					if (event.key.code == sf::Keyboard::Down) {dy=true;p=p/2;}
					if (event.key.code == sf::Keyboard::Up) {rt= true; 
															rotate_sound.play();}
					}
		}
		if (elapsed > p*games[0].getDelay() ) {games[0].advance(0,true,false); clock.restart();dy=false;}
		games[0].advance(dx,dy,rt);
		communicate();
		//checking who s wining
		window.clear(sf::Color(170,170,170)); //grid window color
		for (int i=0; i < ((int) games.size()); i++) games[i].print(&window, {i*games[0].getPrintData().x,0}); // printing the vector of games 
		if (games[0].isGameOver())
		{
			if (first) {startmusic.stop(); gameOverSound.play(); first = false;}

			// checking if all games are over
			if (games.size() > 1)
			{
				bool partyOver =true;
				for (unsigned int i=0; i<games.size(); i++) partyOver &= games[i].isGameOver(); 
				if (partyOver) 
				{
					int winner=0;
					// looking for the winner
					for (unsigned i=1; i< games.size(); i++) {if (games[i].getScore() > games[winner].getScore()) winner = i;}
					std::cout << "THE WINNER IS PLAYER " << winner << std::endl;
					window.close(); 
				}
			}
		}
		
		window.display();
	}
	// Tell the server i left (if am the server can i quit?!)
	int buffer[sizeBuffer];
	for (int i=0; i < sizeBuffer; i++) buffer[i] =0;
	socket.send(buffer,sizeof(buffer),serverIP,port);

}

void Player::createRoom()
{ 
	/*! \brief Waits for players to connect and save the IPs
	*/
	myStatus = SERVER;
	playerID = 0;
	for (int i=1; i < (int) games.size(); i++)
	{
		std::cout << "waiting for the other(s) ....\n";
		// wait for a player to connect
		int buffer[sizeBuffer];
		std::size_t received = 0;
		sf::IpAddress sender;
		socket.receive(buffer, sizeof(buffer), received, sender, port);
		std::cout << sender << "  joined\n";
		otherPlayersIP[sender] = i;
		games[i].loadData(buffer);
		// Send an answer
		int id = i;
		socket.send(&id, sizeof(int), sender, port);
	}
}


void Player::joinRoom() // could take an argumant IP adress 
{
	/*! \brief Connects to the serverIp
	*/
	myStatus = CLIENT;
	int buffer[sizeBuffer];
	games[0].getData(buffer);
	socket.send(buffer, sizeof(buffer), serverIP,port);
	std::size_t received = 0;
	sf::IpAddress sender;
	unsigned short senderPort;
	socket.receive(&playerID, sizeof(playerID), received, sender, senderPort);
	std::cout << "I'm player: " << playerID << std::endl;
}

bool Player::communicate()
{
	/*! \brief Allows us to communicate with other players 
	* for a server it will broadcast the data
	* for a client it will recieve and send it s own data
	*/
	if (myStatus == SERVER)
	{
		int buffer[games.size()*sizeBuffer];
		// recv data from all the other players
		for (unsigned i=1; i< (games.size()-exitedPlayer.size()); i++) // wait for the remain players
		{
			int bufferIndiv[sizeBuffer];
			std::size_t received = 0;
			sf::IpAddress sender;
			unsigned short senderPort;
			socket.receive(bufferIndiv, sizeof(bufferIndiv), received, sender, senderPort);
			if (bufferIndiv[0]==0) {exitedPlayer.push_back(otherPlayersIP[sender]);}
			for (int j = 0; j<sizeBuffer; j++) {buffer[(otherPlayersIP[sender])*sizeBuffer+j] = bufferIndiv[j];}
		}
		for (auto x: exitedPlayer) 
		{
			for (int j = 0; j<sizeBuffer; j++) buffer[x*sizeBuffer+j] = 0;
			buffer[x*sizeBuffer+5] = -100;
			buffer[x*sizeBuffer+11]= -100;
			buffer[x*sizeBuffer] = 1;
		}
		
		// add your own game
		games[0].getData(buffer);
		// load other games
		for (int i=1; i< (int)(games.size()); i++)
		{
			int bufferload[sizeBuffer];
			for (int j=0; j< sizeBuffer; j++) bufferload[j] = buffer[i*sizeBuffer+j];
			if (bufferload[0] != -1) games[i].loadData(bufferload);
			else games[i].advance(0,true,false);
		}
		// send all data to everyone
		for (auto x: otherPlayersIP)
		{
			socket.send(buffer, sizeof(buffer),x.first,port);
		}
	}
	else if (myStatus == CLIENT)
	{
		int bufferIndiv[sizeBuffer];
		games[0].getData(bufferIndiv);
		socket.send(bufferIndiv, sizeof(bufferIndiv), serverIP,port);
		int buffer[games.size()*sizeBuffer];
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short senderPort;
		socket.receive(buffer, sizeof(buffer), received, sender, senderPort);
		for (int i=0; i< (int)(games.size()); i++)
		{
			if (i != playerID){
			int bufferload[sizeBuffer];
			for (int j=0; j< sizeBuffer; j++) bufferload[j] = buffer[i*sizeBuffer+j];
			if (bufferload[0] != -1) games[(i-playerID)%games.size()].loadData(bufferload);
			else games[(i-playerID)%games.size()].advance(0,true,false);}
		}

	}
	return true;
}

void Player::random()
{
	int askedLevel = games[0].getLevel();
	socket.send(&askedLevel, sizeof(askedLevel), serverIP, port);
	std::size_t received = 0;
	sf::IpAddress sender;
	uint32_t buffer;
	socket.receive(&buffer, sizeof(buffer), received, sender, port);
	std::cout << buffer << std::endl;
	if (buffer !=0) 
	{
		serverIP = (sf::IpAddress) buffer;
		std::cout << "LE SERVER EST: " << serverIP << std::endl;
		myStatus = CLIENT;
		joinRoom();
	}
	else
	{
		myStatus = SERVER;
		createRoom();
	}
	
	
}

