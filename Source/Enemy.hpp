#ifndef _ENEMY_HPP
#define _ENEMY_HPP
#include "Map.hpp"
#include "Player.hpp"

class Boss;

class Enemy
{
	protected:
		sf::Vector2f position;
		sf::Vector2f direction;
		float rotation;
		float speed;
		float health, healthSet;
		sf::Sprite body;
		sf::Texture tex;
		float radius;
		bool isBoss;
	public: 
		Enemy();
		Enemy(sf::Vector2f, float);
		void updateDirection(Map* map, Player* player);
		void updateCollision(Map* map, std::vector<sf::Vector2f>* unit);
		bool update(Player* player, Map* map);
		void move(sf::Vector2f);
		sf::Vector2f pos();
		void draw(sf::RenderWindow*);
		bool isABoss();
};

class Boss: public Enemy
{
	private:
	public:
		Boss(sf::Vector2f, float, Map*);
};

#endif
