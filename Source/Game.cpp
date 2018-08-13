#include "Game.hpp"

Game::Game()
{
	/*Initialize everything that is used in the overlay*/
	font.loadFromFile("Resources/DroidSans.ttf");
	missileText.setFont(font);
	missileText.setCharacterSize(24);
	missileText.setColor(sf::Color::Black);
	missileText.setOrigin(12.0, 12.0);
	missileText.setPosition(100.0, 539.0);
	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setColor(sf::Color::Black);
	scoreText.setOrigin(12.0, 12.0);
	scoreText.setPosition(740.0, 12.0);
	pausedText.setFont(font);
	pausedText.setCharacterSize(24);
	pausedText.setColor(sf::Color::Black);
	pausedText.setOrigin(12.0, 12.0);
	pausedText.setPosition(SW - 160, SH - 28);
	pausedText.setString("Game Paused");
	tex[0].loadFromFile("Resources/Information Bar.png");
	frame.setTexture(tex[0]);
	tex[1].loadFromFile("Resources/Health Bar.png");
	healthBar.setTexture(tex[1]);
	healthBar.setPosition(16.0, 181.0);
	tex[2].loadFromFile("Resources/Laser Bar.png");
	laserBar.setTexture(tex[2]);
	laserBar.setPosition(16.0, 360.0);
	tex[3].loadFromFile("Resources/Losing Screen.png");
	back.setTexture(tex[3]);
	showInfoBar = true;
	buildMode = false;
	score = 0;
	frameCount = 1;
	
	otherPlayerFloats.push_back(0.0);
	otherPlayerFloats.push_back(0.0);
	otherPlayerFloats.push_back(0.0);
	
	/*Start the thread that updates the enemy pathfinding*/
	thread.push_back(std::thread(&EnemyManager::update, &eManager, &map, &player));
	thread[0].detach(); //Detach the thread so that it runs in the background
}

void Game::update(sf::RenderWindow* window, sf::View* view)
{
	player.draw(window, &map);
	player.drawAt(window, otherPlayerFloats);
	map.draw(window);
	
	float s = 10 * eManager.pointScore();
	if(player.playerHealth() >= 0)
	{
		player.update(&map, buildMode);
		score += s;
	}
	
	
	//eManager.updateMovement(&map, &player, frameCount, buildMode);
	frameCount++;
	
	eManager.draw(window);
	if(player.playerHealth() <= 0)
	{
		view->setCenter(SW/2, SH/2);
		window->setView(*view);
		window->draw(back);
	}
	if(showInfoBar) //Turned off if you press b
	{
		view->setCenter(SW/2, SH/2);
		window->setView(*view);
		window->draw(frame); 
		healthBar.setScale(player.playerHealth()/100, 1.0);
		window->draw(healthBar);
		laserBar.setScale(player.playerLaserHealth()/player.playerLaserCapacity(), 1.0);
		window->draw(laserBar);
		missileText.setOrigin(6.0 * (int)(log(player.playerMissileCapacity())/log(10.0) + 1), 12.0);
		missileText.setString(helper::intToString(player.playerMissileCapacity()));
		window->draw(missileText);
		missileText.setPosition(sf::Vector2f(100, 717.0));
		missileText.setOrigin(6.0 * (int)(log(player.partCount())/log(10.0) + 1), 12.0);
		missileText.setString(helper::intToString(player.partCount()));
		window->draw(missileText);
		missileText.setPosition(100.0, 539.0);
		if(buildMode)
			window->draw(pausedText);
			/*
		for(int i = 0; i < player.partCount(); i++)
		{
			Part part = Part(i);
			part.draw(window); //Draw in the overlay
		}*/
		scoreText.setString("Score: " + helper::intToString(score));
		window->draw(scoreText);
	}
	view->setCenter(player.pos());
	window->setView(*view);
}

void Game::handleInput(sf::RenderWindow* window)
{
	if(player.playerHealth() > 0)
	{
		sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
		player.orient(mousePos);
		sf::Vector2f moveVec = sf::Vector2f(0, 0);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			moveVec += sf::Vector2f(0, -1);
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			moveVec += sf::Vector2f(-1, 0);
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			moveVec += sf::Vector2f(0, 1);
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			moveVec += sf::Vector2f(1, 0);
		}
		if(moveVec != sf::Vector2f(0, 0) and !buildMode)
		{
			moveVec = helper::unitVector(sf::Vector2f(0.0, 0.0), moveVec); //Keep the velocity of the player constant
			player.move(moveVec.x, moveVec.y, &map);
		}
		if(helper::bPressed())
			showInfoBar = !showInfoBar;
		if(helper::spacePressed())
			buildMode = !buildMode;
		if(helper::rightMouseClicked())
			player.placePart(mousePos, &map); //Right click to place part
		if(!buildMode)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) and player.playerLaserHealth() >= 2.0)
			{	
				if(fireClock.getElapsedTime().asMilliseconds() > 70 or (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) and fireClock.getElapsedTime().asMilliseconds() > 16)) //Normal fire: once every five frames
				{
					fireClock.restart();
					player.fireLaser(mousePos);
				}
			}
			if (helper::middleMouseClicked() and player.playerMissileCapacity() > 0) //Middle click to fire missile
				player.fireMissile(mousePos);
		}
    }
}

/* Copied from https://stackoverflow.com/questions/236129/the-most-elegant-way-to-iterate-the-words-of-a-string
Splits a string into a parts deliminated by a character */
template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}
/* -------------------------- */

void Game::handleMessage(std::string message, sf::RenderWindow *window)
{
	std::vector<std::string> commands = split(message, '\n');
	for (std::string command : commands)
	{
		switch(command.at(0))
		{
			case 'p':
				std::vector<std::string> numsStr = split(command.substr(1), ' ');
				std::vector<float> nums;
				for (std::string &str : numsStr)
				{
					nums.push_back(std::stof(str));
				}
				otherPlayerFloats = nums;
		}
	}
}

std::string Game::messageUpdates()
{
	std::string message;
	message += "p";
	message += std::to_string(player.pos().x);
	message += " ";
	message += std::to_string(player.pos().y);
	message += " ";
	message += std::to_string(player.getRotation());
	return message;
}
