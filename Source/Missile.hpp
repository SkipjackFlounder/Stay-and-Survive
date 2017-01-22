#include "Explosion.hpp"

class Missile: public Projectile
{
	private:
		bool exploded;
		Explosion *site;
	public:
		Missile(sf::Vector2f, sf::Vector2f, float);
		void update();
		void blowUp();
		void reverse();
		bool didUse();
		void draw(sf::RenderWindow *window, Map *map);
		bool expired(Map*);
};
