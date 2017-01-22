#include "EnemyManager.hpp"

EnemyManager::EnemyManager()
{
	for(int i = 0; i < 3; i++)
		enemy.push_back(*(new Enemy(sf::Vector2f(90, -1000), 8)));
	points = 0;
	buildMode = false;
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
				std::lock_guard<std::mutex> lock(mtx); //RAII in case the thread is killed without an unlock
				enemy[i].updateDirection(map, player);
			}
			sf::sleep(sf::milliseconds(1)); //Give the updateMovement function a millisecond to catch up
			//Alternatives to this include joining this thread rather than attaching
		}
	}
}

void EnemyManager::updateMovement(Map* map, Player* player, int count, bool buildMode)
{
	mtx.lock();
	int killed = 0;
	for(int i = 0; i < enemy.size(); i++)
		if(enemy[i].update(player, map))
		{
			killed ++;
			if(enemy[i].isABoss())
				enemy[i] = *(new Enemy(sf::Vector2f(90, -1000), enemy.size()*2 + 5));
			int c = rand()%4;
			switch(c)
			{
				case 0:
					enemy[i].move(sf::Vector2f((float)(rand()%(map->dimensions().x - 4)) * BLOCK_SIZE + 2 * BLOCK_SIZE, -2300));
					break;
				case 1:
					enemy[i].move(sf::Vector2f(-2300, (float)(rand()%(map->dimensions().y - 4)) * BLOCK_SIZE + 2 * BLOCK_SIZE));
					break;
				case 2:
					enemy[i].move(sf::Vector2f((float)(rand()%(map->dimensions().x - 4)) * BLOCK_SIZE + 2 * BLOCK_SIZE, map->dimensions().y * BLOCK_SIZE + 2300));
					break;
				case 3:
					enemy[i].move(sf::Vector2f((float)(map->dimensions().x) * BLOCK_SIZE + 2300, (float)(rand()%(map->dimensions().y - 4)) * BLOCK_SIZE + 2 * BLOCK_SIZE));
					break;
			}
		}
	if(count % (60 * 23) == 0 and enemy.size() < 52)
	{
		enemy.push_back(*(new Enemy(sf::Vector2f(90, -1000), enemy.size()*2 + 5)));
	}
	if(count % (12000) == 0 and enemy.size() < 64)
	{
		float health = enemy.size() * enemy.size() + 270;
		if(enemy.size() >= 16)
			health += (float)(count)/62.0;
		if(enemy.size() >= 30)
			health *= 1.2;
		if(count > (60 * 60 * 15))
			health *= 1.3;
		if(count > (60 * 60 * 20))
			health *= 1.4;
		if(count > (60 * 60 * 25))
			health *= 1.5;
		if(count > (60 * 60 * 30))
			health *= 1.6;
		std::cout << health << " ";
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

void EnemyManager::draw(sf::RenderWindow* window)
{
	for(int i = 0; i < enemy.size(); i++)
	{
		enemy[i].draw(window);
	}
}
