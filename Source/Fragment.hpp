#include "Projectile.hpp"

class Fragment: public Projectile
{
	private:
	public:
		Fragment(sf::Vector2f, sf::Vector2f, float);
		void update();
		void draw(sf::RenderWindow *window);
		bool expired(Map*);
};
