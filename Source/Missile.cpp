#include "Missile.hpp"

Missile::Missile(sf::Vector2f position, sf::Vector2f direction, float orientation)
{
	tex.loadFromFile("Resources/Missile.png");
	beam.setTexture(tex);
	this->position = position;
	this->direction = direction;
	beam.setPosition(position);
	beam.setOrigin(512, 256);
	beam.setScale(64.0/1024.0, 32.0/512.0);
	beam.setRotation(orientation);
	this->direction += sf::Vector2f(((float)(rand()%100)/10000 - .005), ((float)(rand()%100)/10000 - .005));
	speed = 16 - (float)(rand()%3);
	exploded = false;
}

void Missile::update()
{
	if(!exploded)
		position += direction * speed;
		beam.setPosition(position);
}

void Missile::blowUp()
{
	if(!exploded)
	{
		exploded = true;
		clock.restart();
		site = NULL; site = new Explosion(this->position);
	}
}

void Missile::reverse()
{
	this->direction = -this->direction;
	beam.setRotation(180 + beam.getRotation());
	position += direction * speed;
}

bool Missile::didUse()
{
	return exploded and clock.getElapsedTime().asSeconds() > 2;
}

void Missile::draw(sf::RenderWindow *window, Map *map)
{
	if(!exploded)
		window->draw(beam);
	else
	{
		site->update(map);
		site->draw(window, map);
	}
}

bool Missile::expired(Map *map)
{
	if(map->collide(this->position, 16) and !exploded)
	{
		exploded = true;
		clock.restart();
		site = NULL; site = new Explosion(this->position);
	}
	if(clock.getElapsedTime().asSeconds() > 15)// or (exploded = true and clock.getElapsedTime().asSeconds() > 2))
		return true;
	return false; 
}
