#include "Laser.hpp"

Laser::Laser(sf::Vector2f position, sf::Vector2f direction, float orientation, bool upgraded)
{
	if(!upgraded)
		tex.loadFromFile("Resources/Beam.png");
	else
		tex.loadFromFile("Resources/UpgradedBeam.png");
	beam.setTexture(tex);
	this->position = position;
	this->direction = direction;
	beam.setPosition(position);
	beam.setOrigin(8, 2);
	beam.setRotation(orientation);
	this->direction += sf::Vector2f(((float)(rand()%100)/10000 - .005), ((float)(rand()%100)/10000 - .005));
	speed = 32; - (float)(rand()%5);
	
}

void Laser::update()
{
	frameElapsed = framerateClock.restart();
	position += direction * (speed * frameElapsed.asMicroseconds() / 10000);
	beam.setPosition(position);
}

void Laser::draw(sf::RenderWindow *window)
{
	window->draw(beam);
}

bool Laser::expired(Map *map)
{
	if(map->collide(this->position, 3))
		return true;
	if(clock.getElapsedTime().asSeconds() > 10)
		return true;
	return false; 
}

sf::Vector2f Laser::getDirection()
{
	return direction;
}
