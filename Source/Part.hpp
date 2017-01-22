#ifndef _PART_HPP
#define _PART_HPP
#include "Tower.hpp"

class PartSet
{
	private:
		sf::Sprite partBody;
		sf::Texture tex;
		std::vector<sf::Vector2i> part;
		int detectTower(int);
	public:
		PartSet();
		bool partExist(sf::Vector2f);
		std::pair<towerShape::towerID, sf::Vector2i> addPart(sf::Vector2f);
		void removePart(sf::Vector2f);
		void removeTower(int, sf::Vector2i);
		void draw(sf::RenderWindow*);
		
};

class Part
{
	private:
		sf::Sprite body;
		sf::Texture tex;
	public:
		Part(int);
		void draw(sf::RenderWindow*);
};

class Pickup
{
	private:
		sf::Sprite body;
		sf::Texture tex;
		void relocate(Map*);
		bool justSpawned;
	public:
		Pickup();
		Pickup(Map*);
		bool update(sf::Vector2f, float, Map*, int);
		void draw(sf::RenderWindow*);
};
#endif
