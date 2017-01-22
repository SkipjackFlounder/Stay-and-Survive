#include "Projectile.hpp"

class Laser: public Projectile
{
	public:
		Laser(sf::Vector2f, sf::Vector2f, float, bool);
		void update();
		void draw(sf::RenderWindow *window);
		bool expired(Map*);
		sf::Vector2f getDirection();
};
