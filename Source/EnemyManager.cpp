#include "EnemyManager.hpp"

EnemyManager::EnemyManager()
{
	points = 0;
	buildMode = false;
	spawnClock.restart();
	bossClock.restart();
	spawnCharge = 0;
	bossCharge = 0;
	totalElapsed = 0;
}

EnemyManager::~EnemyManager()
{
	for(int i = 0; i < enemy.size(); i++)
		delete &enemy[i];
}

void EnemyManager::update(Map* map, Player* player)
{
	while(1)
	{
		for(int i = 0; i < enemy.size(); i++)
		{
			{
				//std::lock_guard<std::mutex> lock(mtx); //RAII in case the thread is killed without an unlock
				mtx.lock();
				enemy[i].updateDirection(map, player);
				mtx.unlock();
			}
			sf::sleep(sf::milliseconds(5)); //Give the updateMovement function a few millisecond to catch up
			//Alternatives to this include joining this thread rather than attaching
		}
	}
}

void EnemyManager::updateMovement(Map* map, Player* player, int count, bool buildMode)
{
	mtx.lock();
	int killed = 0;
	
	if (!buildMode)
	{
		spawnCharge += spawnClock.restart().asSeconds();
		bossCharge += bossClock.restart().asSeconds();
	}
	spawnClock.restart();
	bossClock.restart();
	
	for(int i = 0; i < enemy.size(); i++)
		if(enemy[i].update(player, map, buildMode))
		{
			killed ++;
			enemy.erase(enemy.begin() + i);
			i -= 1;
		}
	
	/* Spawn an enemy every 4.5 seconds */
	if(spawnCharge > 4.5 and enemy.size() < 100)
	{
		totalElapsed += 1;
		spawnCharge = 0;
		enemy.push_back(*(new Enemy(sf::Vector2f(90, -1000), pow(totalElapsed, 0.8) * 5, map)));
	}
	/* Spawn a boss every 3 minutes */
	if(bossCharge > 180 and enemy.size() < 104)
	{
		float health = totalElapsed * 25;
		bossCharge = 0;
		enemy.push_back(*(new Boss(sf::Vector2f(90, -1000), health, map)));
	}
	std::vector<sf::Vector2f> position = *(new std::vector<sf::Vector2f>());
	//int compression = (int)(enemy.size())/15;
	int compression = 1;
	for(int j = count % compression; j < enemy.size(); j+=compression)
	{
		position.push_back(enemy[j].pos());
	}
	for(int i = 0; i < enemy.size(); i++)
		enemy[i].updateCollision(map, &position);
	addPoints(killed);
	mtx.unlock();
}

void EnemyManager::addPoints(int num)
{
	points += num;
}

int EnemyManager::pointScore()
{
	int p = points;
	points = 0;
	return p;
}

std::string EnemyManager::getMessage()
{
	std::string out;
	mtx.lock();
	for(int i = 0; i < enemy.size(); i++)
	{
		if (enemy[i].isABoss())
		{
			out += "b";
		} else {
			out += "e";
		}
		out += std::to_string(enemy[i].pos().x) + " ";
		out += std::to_string(enemy[i].pos().y) + " ";
		out += std::to_string(enemy[i].getRotation()) + " ";
		out += std::to_string(enemy[i].getHealth()) + "\n";
	}
	mtx.unlock();
	return out;
}

void EnemyManager::erase()
{
	mtx.lock();
	enemy = std::vector<Enemy>();
	mtx.unlock();
}

void EnemyManager::add(std::vector<float>& parameters, Map *map, bool boss)
{
	mtx.lock();
	if (!boss)
	{
		enemy.push_back(Enemy(sf::Vector2f(parameters[0], parameters[1]), parameters[2], parameters[3], map));
	} else {
		enemy.push_back(Boss(sf::Vector2f(parameters[0], parameters[1]), parameters[2], parameters[3], map));
	}
	mtx.unlock();
}

void EnemyManager::draw(sf::RenderWindow* window)
{
	mtx.lock();
	for(int i = 0; i < enemy.size(); i++)
	{
		enemy[i].draw(window);
	}
	mtx.unlock();
}
