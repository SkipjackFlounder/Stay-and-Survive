#include "Tower.hpp"

Tower::Tower()
{

}

Hospital::Hospital(sf::Vector2i pos)
{
	tex.loadFromFile("Resources/Hospital.png");
	body.setTexture(tex);
	body.setPosition(sf::Vector2f((float)(pos.x * BLOCK_SIZE), (float)(pos.y * BLOCK_SIZE)));
	id = towerShape::towerID::Hospital;
}

LaserPad::LaserPad(sf::Vector2i pos)
{
	tex.loadFromFile("Resources/LaserPad.png");
	body.setTexture(tex);
	body.setPosition(sf::Vector2f((float)(pos.x * BLOCK_SIZE), (float)(pos.y * BLOCK_SIZE)));
	id = towerShape::towerID::LaserPad;
}

Sentry::Sentry(sf::Vector2i pos)
{
	tex.loadFromFile("Resources/Sentry.png");
	body.setTexture(tex);
	body.setPosition(sf::Vector2f((float)(pos.x * BLOCK_SIZE), (float)(pos.y * BLOCK_SIZE)));
	id = towerShape::towerID::Sentry;
}

bool Tower::update(sf::Vector2f pos)
{
	if(helper::withinRectangle(pos, body.getPosition(), body.getPosition() + sf::Vector2f(96.0, 96.0)))
		return true;
	return false;
}

int Tower::ID()
{
	return id;
}

sf::Vector2f Tower::pos()
{
	return body.getPosition();
}

int Tower::clock()
{
	return c.getElapsedTime().asMilliseconds();
}

void Tower::restartClock()
{
	c.restart();
}

void Tower::draw(sf::RenderWindow* window)
{
	window->draw(body);
}
