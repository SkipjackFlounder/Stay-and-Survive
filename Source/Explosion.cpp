#include "Explosion.hpp"

Explosion::Explosion(sf::Vector2f pos)
{
	for(int i = 0; i < FRAG_COUNT; i++)
	{
		fragment.push_back(*(new Fragment(pos, sf::Vector2f(((float)(rand()%10) - 5)/3, ((float)(rand()%10) - 5)/3), (float)(rand()%360))));
	}
	buffer.loadFromFile("Resources/Sound/Explosion.ogg");
	sound.setBuffer(buffer);
	sound.play();
}

Explosion::~Explosion()
{
	for(int i = 0; i < fragment.size(); i++)
		delete &fragment[i];
}

void Explosion::update(Map* map)
{
	for(int i = 0; i < fragment.size(); i++)
	{
		fragment[i].update();
		if(fragment[i].expired(map))
			fragment.erase(fragment.begin() + i);
	}
}
void Explosion::draw(sf::RenderWindow* window, Map *map)
{
	for(int i = 0; i < fragment.size(); i++)
	{
		fragment[i].draw(window);
		if(fragment[i].expired(map))
		{
			 fragment.erase(fragment.begin() + i);
		}
	}
}
