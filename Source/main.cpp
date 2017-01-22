#include "Game.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(SW, SH), "Top Down Survival", sf::Style::Fullscreen);
    //Create Window at fullscreen
    window.setFramerateLimit(60);
    //Set frame limit - hopefully I will replace this in the future with a dynamic framerate
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
			
        window.clear(sf::Color(230, 230, 230));
        	game.update(&window, &view);
        	game.handleInput(&window);
        window.display();
        //std::cout << 1000.0/c.restart().asMilliseconds() << " ";
        //print the framerate
    }

    return 0;
}
