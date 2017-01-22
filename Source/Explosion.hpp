#include "Fragment.hpp"
#define FRAG_COUNT 20

class Explosion
{
	private:
		std::vector<Fragment> fragment;
		sf::SoundBuffer buffer;
		sf::Sound sound;
	public:
		Explosion(sf::Vector2f);
		~Explosion();
		void update(Map*);
		void draw(sf::RenderWindow*, Map*);
};
