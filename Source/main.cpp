#include "Network.hpp"

int main()
{
	std::cout << "-------Welcome to Stay and Survive Networking-------\n";
	std::cout << "Type 'host' to host a connection\nor type 'join' to connect to a host\n";
	std::string query;
	bool host = false;
	do
	{
		std::cin >> query;
	}while(query != "host" and query != "join");
	Liason *liason;
	if(query == "host")
	{
		liason = new Host();
		host = true;
	}
	else if(query == "join")
	{
		liason = new Client();
	}
		
	//Create Window at fullscreen
    sf::RenderWindow window(sf::VideoMode(SW, SH), "Top Down Survival", sf::Style::Fullscreen);
    
    sf::View view(sf::FloatRect(0, 0, SW, SH));
    window.setView(view);
    sf::Clock c; //For keeping track of time per frame
    
    srand(time(0));
	
	Game game;
	
    while (window.isOpen())
    {
        sf::Event event;
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			window.close();
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
			
        window.clear(sf::Color(150, 150, 150));
        	game.update(&window, &view);
        	game.handleInput(&window);
        	std::string str = liason->retrieve();
        	game.handleMessage(str, &window);
        	liason->send(game.messageUpdates());
        window.display();
        //std::cout << 1000.0/c.restart().asMilliseconds() << " ";
        //print the framerate
    }

    return 0;
}
