#include "Enemy.hpp"

Enemy::Enemy()
{

}

Enemy::Enemy(sf::Vector2f pos, float h, Map* map)
{
	radius = 16;
	body.setOrigin(radius, radius);
	this->direction = sf::Vector2f(0, 0);
	tex.loadFromFile("Resources/Enemy.png");
	body.setTexture(tex);
	this->position = pos;
	body.setPosition(pos);
	this->healthSet = h;
	health = h;
	int c = rand()%4;
	moveCharge = 0;
	switch(c)
	{
		case 0:
			move(sf::Vector2f((float)(rand()%(map->dimensions().x - 4)) * BLOCK_SIZE + 2 * BLOCK_SIZE, -2300));
			break;
		case 1:
			move(sf::Vector2f(-2300, (float)(rand()%(map->dimensions().y - 4)) * BLOCK_SIZE + 2 * BLOCK_SIZE));
			break;
		case 2:
			move(sf::Vector2f((float)(rand()%(map->dimensions().x - 4)) * BLOCK_SIZE + 2 * BLOCK_SIZE, map->dimensions().y * BLOCK_SIZE + 2300));
			break;
		case 3:
			move(sf::Vector2f((float)(map->dimensions().x) * BLOCK_SIZE + 2300, (float)(rand()%(map->dimensions().y - 4)) * BLOCK_SIZE + 2 * BLOCK_SIZE));
			break;
	}
	speed = ((float)(rand()%100))/200 + .65;
	isBoss = false;
}

Boss::Boss(sf::Vector2f pos, float h, Map* map)
{
	radius = 32;
	body.setOrigin(radius, radius);
	this->direction = sf::Vector2f(0, 0);
	tex.loadFromFile("Resources/SuperEnemy.png");
	body.setTexture(tex);
	this->position = pos;
	body.setPosition(pos);
	this->healthSet = h;
	health = healthSet;
	int c = rand()%4;
	speed = ((float)(rand()%100))/200 + .5;
	health = h;
	switch(c)
	{
		case 0:
			move(sf::Vector2f((float)(rand()%(map->dimensions().x - 4)) * BLOCK_SIZE + 2 * BLOCK_SIZE, -2000));
			break;
		case 1:
			move(sf::Vector2f(-2000, (float)(rand()%(map->dimensions().y - 4)) * BLOCK_SIZE + 2 * BLOCK_SIZE));
			break;
		case 2:
			move(sf::Vector2f((float)(rand()%(map->dimensions().x - 4)) * BLOCK_SIZE + 2 * BLOCK_SIZE, map->dimensions().y * BLOCK_SIZE + 2000));
			break;
		case 3:
			move(sf::Vector2f((float)(map->dimensions().x) * BLOCK_SIZE + 2000, (float)(rand()%(map->dimensions().y - 4)) * BLOCK_SIZE + 2 * BLOCK_SIZE));
			break;
	}
	isBoss = true;
}

void Enemy::updateDirection(Map* map, Player* player)
{
	if(position.x < 10)
	{
		direction = sf::Vector2f(1, 0);
		return;
	}else if(position.x > BLOCK_SIZE * map->dimensions().x - 10)
	{
		direction = sf::Vector2f(-1, 0);
		return;
	}else if(position.y < 10)
	{
		direction = sf::Vector2f(0, 1);
		return;
	}else if(position.y > BLOCK_SIZE * map->dimensions().y - 10)
	{
		direction = sf::Vector2f(0, -1);
		return;
	}
	Player* playerCopy = player;
	Map* mapCopy = map;
	direction = mapCopy->find(position, playerCopy->pos(), radius);
}

void Enemy::updateCollision(Map* map, std::vector<sf::Vector2f>* unit)
{
	if(position.x < 10)
		return;
	else if(position.x > BLOCK_SIZE * map->dimensions().x - 10)
		return;
	else if(position.y < 10)
		return;
	else if(position.y > BLOCK_SIZE * map->dimensions().y - 10)
		return;
	for(int i = 0; i < unit->size(); i++)
	{
		float dist = helper::distance((*unit)[i], position);
		bool collide = map->collide(helper::unitVector(position, (*unit)[i]) + position, radius);
		if((*unit)[i] != position and dist < radius + 16 and !collide)
			position -= helper::unitVector(position, (*unit)[i]);
		else if(collide)
			position += map->directionalCollide(helper::unitVector(position, (*unit)[i]) + position, radius);
	}
}

bool Enemy::update(Player* player, Map* map, bool buildMode)
{
	if (!buildMode)
	{
		moveCharge = 0;
		/* Move the enemy unit during the frame */
		if (!buildMode)
		{
			moveCharge += framerateClock.restart().asMicroseconds();
		}
		position += direction * (speed * moveCharge / 10000);
		
		std::pair<float, sf::Vector2f> n = player->checkEnemyCollide(position, radius, map);
		health -= n.first;
		sf::Vector2f v = helper::unitVector(sf::Vector2f(0, 0), n.second);
		int knockBack = 5.5;
		if(radius > 16)
			knockBack = 1.0;
		if(n.first >= 1.0)
			position += sf::Vector2f(v.x * knockBack, v.y * knockBack);
		
		
		
		body.setPosition(position);
	
		if(health < 0)
		{
			health = healthSet;
			return true;
		}	
		this->rotation = (180/PI) * atan2(direction.y, direction.x);
		body.setRotation(rotation);
	}
	framerateClock.restart();
	return false;
}

void Enemy::move(sf::Vector2f pos)
{
	position = pos;
	healthSet += 2.0;
	healthSet *= 1.08;
	if(!isBoss and radius <= 16)
		speed = ((float)(rand()%100))/200 + .6;
	else
		speed = ((float)(rand()%100))/200 + .5;
}


sf::Vector2f Enemy::pos()
{
	return position;
}

bool Enemy::isABoss()
{
	return isBoss;
}

void Enemy::draw(sf::RenderWindow* window)
{
	window->draw(body);
}
