#ifndef _PROJECTILE_HPP
#define _PROJECTILE_HPP
#include "Map.hpp"

class Projectile
{
	protected:
		sf::Vector2f position;
		sf::Vector2f direction;
		float speed;
		float orientation;
		sf::Sprite beam;
		sf::Texture tex;
		sf::Clock clock;
	public:
		sf::Vector2f pos(){return position;}
};
#endif
