#ifndef _MAP_HPP
#define _MAP_HPP
#define BLOCK_SIZE 32
#include "Hardware.hpp"

class Map
{
	private:
		std::vector< std::vector<int> > block;
		sf::Sprite square;
		sf::Texture blockTex, tBlockTex;
	public:
		Map();
		bool collide(sf::Vector2f, float);
		bool blocked(sf::Vector2i);
		bool collideSegment(sf::Vector2f a, sf::Vector2f b);
		sf::Vector2f directionalCollide(sf::Vector2f, float);
		sf::Vector2i dimensions();
		sf::Vector2f find(sf::Vector2f, sf::Vector2f, float);
		
		void draw(sf::RenderWindow*);
};

struct PathTile
{
	sf::Vector2i position;
	sf::Vector2i dir;
	float f, g, h;
	PathTile(sf::Vector2i pos) {this->position = pos; 
								dir = sf::Vector2i(0,0);
								f = 0; g = 0; h = 0;}
};
#endif
