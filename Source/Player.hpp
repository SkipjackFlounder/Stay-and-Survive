#ifndef _PLAYER_HPP
#define _PLAYER_HPP
#include "Laser.hpp"
#include "Missile.hpp"
#include "Part.hpp"

#define PLAYER_RADIUS 32

class Player
{
	private:
		sf::Sprite body;
		sf::Sprite otherBody;
		sf::Texture tex;
		sf::Texture tex1;
		sf::SoundBuffer buffer;
		sf::Sound sound;
		sf::Vector2f position;
		float health;
		float rotation;
		bool isShooting;
		std::vector<Laser> shot;
		std::vector<Missile> miss;
		std::vector<Part> part;
		std::vector<Tower> tower;
		PartSet partSet;
		float laserHealth;
		int missCapacity;
		int missAdd;
		Pickup pickup[5];
		float laserDamage, laserCapacity;
		bool justSpawned;
		sf::Clock framerateClock;
		sf::Time frameElapsed;
	public:
		Player();
		~Player();
		void update(Map* map, bool buildMode);
		void addTower(int type, sf::Vector2i pos);
		void draw(sf::RenderWindow *window, Map *map);
		void drawAt(sf::RenderWindow *window, std::vector<float>&);
		void orient(sf::Vector2f cursor);
		void move(float x, float y, Map*);
		std::pair<float, sf::Vector2f> checkEnemyCollide(sf::Vector2f, float, Map* map);
		sf::Vector2f pos();
		sf::Vector2f otherPos();
		float getRotation();
		float playerHealth();
		float playerLaserHealth();
		float playerLaserCapacity();
		int playerMissileCapacity();
		int partCount();
		void fireLaser(sf::Vector2f cursor);
		void fireMissile(sf::Vector2f cursor);
		void placePart(sf::Vector2f mPos, Map *map);
};
#endif
