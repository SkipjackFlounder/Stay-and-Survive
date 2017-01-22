#ifndef _TOWER_HPP
#define _TOWER_HPP
#include "Map.hpp"

class Hospital;
class LaserPad;
class Sentry;

namespace towerShape
{
	const int towerCount = 5;
	const float sentryRange = 350.0;
	enum towerID
	{
		Nothing,
		Hospital,
		LaserPad,
		Sentry,
		WeaponPack,
		WeaponUpgrade
	};
	/*Shape of the towers*/
	const int towerArray[5][3][3] = 
   {{{1, 1, 1},
	 {0, 1, 0},
	 {1, 1, 1}},
	 
	{{1, 1, 1},
	 {0, 0, 1},
	 {0, 0, 0}},
	 
	{{1, 1, 1},
	 {1, 0, 1},
	 {1, 1, 1}},
	 
	{{1, 0, 0},
	 {0, 1, 0},
	 {1, 0, 0}},
	 
	{{1, 1, 1},
	 {1, 1, 1},
	 {1, 1, 1}}};
}

class Tower
{
	protected:
		sf::Sprite body;
		sf::Texture tex;
		int id;
		sf::Clock c;
	public:
		Tower();
		void draw(sf::RenderWindow*);
		bool update(sf::Vector2f);
		int ID();
		sf::Vector2f pos();
		int clock();
		void restartClock();
};

class Hospital: public Tower
{
	private:
	public:
		Hospital(sf::Vector2i pos);
};

class LaserPad: public Tower
{
	private:
	public:
		LaserPad(sf::Vector2i pos);
};

class Sentry: public Tower
{
	private:
	public:
		Sentry(sf::Vector2i);
};

#endif
