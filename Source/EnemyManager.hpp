#include "Enemy.hpp"

class EnemyManager
{
	private:
		std::vector<Enemy> enemy;
		std::mutex mtx;
		sf::Clock clock;
		int points;
		void addPoints(int);
		std::atomic<bool> buildMode;
		sf::Clock spawnClock;
		float spawnCharge;
		sf::Clock bossClock;
		float bossCharge;
		float totalElapsed;
	public:
		EnemyManager();
		~EnemyManager();
		void update(Map* map, Player* player);
		void updateMovement(Map* map, Player* player, int, bool);
		int pointScore();
		std::string getMessage();
		void erase();
		void add(std::vector<float>&, Map*, bool);
		void draw(sf::RenderWindow*);
};
