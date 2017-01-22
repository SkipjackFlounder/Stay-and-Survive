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
	score = -30;
	frameCount = 1;
	/*Start the thread that updates the enemy pathfinding*/
	thread.push_back(std::thread (&EnemyManager::update, &eManager, &map, &player));
	thread[0].detach(); //Detach the thread so that it runs in the background
}

void Game::update(sf::RenderWindow* window, sf::View* view)
{
	player.draw(window, &map);
	map.draw(window);
	if(!buildMode)
	{
		float s = 10 * eManager.pointScore();
		if(player.playerHealth() >= 0)
		{
			player.update(&map);
			score += s;
		}
		eManager.updateMovement(&map, &player, frameCount, buildMode);
		frameCount++;
	}
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
		if(buildMode)
			window->draw(pausedText);
		for(int i = 0; i < player.partCount(); i++)
		{
			Part part = Part(i);
			part.draw(window); //Draw in the overlay
		}
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
		static int i = 0;
		if(helper::rightMouseClicked())
			player.placePart(mousePos, &map); //Right click to place part
		if(!buildMode)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) and player.playerLaserHealth() >= 2.0)
			{	
				if(i%5 == 0 or sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) //Normal fire: once every five frames
					player.fireLaser(mousePos);
				i++;
			}else
				i = 0;
			if (helper::middleMouseClicked() and player.playerMissileCapacity() > 0) //Middle click to fire missile
				player.fireMissile(mousePos);
		}
    }
}
