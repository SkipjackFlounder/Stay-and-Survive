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
		sf::Clock framerateClock;
		sf::Time frameElapsed;
		float moveCharge;
	public: 
		Enemy();
		Enemy(sf::Vector2f, float, Map*);
		Enemy(sf::Vector2f, float, float, Map*);
		void updateDirection(Map* map, Player* player);
		void updateCollision(Map* map, std::vector<sf::Vector2f>* unit);
		bool update(Player* player, Map* map, bool buildMode);
		void move(sf::Vector2f);
		sf::Vector2f pos();
		float getRotation();
		float getHealth();
		void draw(sf::RenderWindow*);
		bool isABoss();
};

class Boss: public Enemy
{
	private:
	public:
		Boss(sf::Vector2f, float, Map*);
		Boss(sf::Vector2f, float, float, Map*);
};

#endif
