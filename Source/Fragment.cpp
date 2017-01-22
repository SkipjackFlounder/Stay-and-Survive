#include "Fragment.hpp"

Fragment::Fragment(sf::Vector2f position, sf::Vector2f direction, float orientation)
{
	tex.loadFromFile("Resources/Fragment.png");
	beam.setTexture(tex);
	this->position = position + sf::Vector2f((float)(rand()%20) - 10, (float)(rand()%20) - 10);
	this->direction = direction;
	beam.setPosition(position);
	beam.setOrigin(8, 8);
	beam.setRotation(orientation);
	this->direction += sf::Vector2f(((float)(rand()%100)/10000 - .005), ((float)(rand()%100)/10000 - .005));
	speed = 1 - (float)(rand()%10)/5;
}

void Fragment::update()
{
	position += direction * speed;
	beam.setPosition(position);
}

void Fragment::draw(sf::RenderWindow *window)
{
	window->draw(beam);
}

bool Fragment::expired(Map *map)
{
	if(map->collide(this->position, 8))
		return true;
	if(clock.getElapsedTime().asSeconds() > 2)
		return true;
	return false; 
}
