#include "Player.hpp"

Player::Player()
{
	tex.loadFromFile("Resources/PlayerBlue.png");
	health = 100;
	body.setTexture(tex);
	body.setScale((float)64/(float)512, (float)64/(float)512);
	body.setOrigin(256, 256);
	body.setPosition(100, 100);
	tex.loadFromFile("Resources/PlayerBlack.png");
	otherBody.setTexture(tex);
	otherBody.setScale((float)64/(float)512, (float)64/(float)512);
	otherBody.setOrigin(256, 256);
	otherBody.setPosition(100, 100);
	position.x = 100; position.y = 100;
	laserCapacity = 100;
	laserHealth = laserCapacity;
	missCapacity = 5;
	laserDamage = 1.0;
	justSpawned = true;
	missAdd = 10;
}

Player::~Player()
{
	//Free up the memory
	for(int i = 0; i < shot.size(); i++)
		delete &shot[i];
	for(int i = 0; i < miss.size(); i++)
		delete &miss[i];
	for(int i = 0; i < tower.size(); i++)
		delete &tower[i];
	for(int i = 0; i < sizeof(pickup)/sizeof(pickup[0]); i++)
		delete &pickup[i];
}

void Player::update(Map* map, bool buildMode)
{
	frameElapsed = framerateClock.restart();
	if (!buildMode)
	{
		if(justSpawned)
		{
			for(int i = 0; i < sizeof(pickup)/sizeof(pickup[0]); i++)
				pickup[i] = *(new Pickup(map));
			justSpawned = false;
		}
		if(laserHealth <= laserCapacity)
			laserHealth += .1 * frameElapsed.asMicroseconds() / 10000;
		for(int i = 0; i < sizeof(pickup)/sizeof(pickup[0]); i++)
			if(pickup[i].update(position, PLAYER_RADIUS, map, part.size()))
				part.push_back(*(new Part(part.size())));
		for(int i = 0; i < tower.size(); i++)
		{
			bool onTower = tower[i].update(position);
			if(onTower)
				switch(tower[i].ID())
				{
					case towerShape::towerID::Hospital:
					{
						if(health < 100)
							health += 90 * ((float)frameElapsed.asMilliseconds() / 10000.0);
						break;
					}
					case towerShape::towerID::LaserPad:
					{
						if(laserHealth < laserCapacity)
							laserHealth += .45 * ((float)frameElapsed.asMicroseconds() / 10000.0);
						break;
					}
				}
		}
	}
}

void Player::placePart(sf::Vector2f mPos, Map *map)
{
	bool freeSpace = true;
	for(int i = 0; i < tower.size(); i++)
	{
		if(tower[i].ID() == towerShape::towerID::Sentry and helper::withinRectangle(mPos, tower[i].pos(), tower[i].pos() + sf::Vector2f(96.0, 96.0)))
			freeSpace = false;
	}
	
	if(helper::withinRectangle(mPos, sf::Vector2f(0, 0), helper::vectorIntToFloat(map->dimensions() * BLOCK_SIZE)) and freeSpace)
	if(!map->blocked(sf::Vector2i((int)(mPos.x/BLOCK_SIZE), (int)(mPos.y/BLOCK_SIZE))))
	{
		if(!partSet.partExist(mPos) and part.size() > 0)
		{
			part.erase(part.begin() + part.size() - 1);
			std::pair<towerShape::towerID, sf::Vector2i> newTower = partSet.addPart(mPos);
			addTower(newTower.first, newTower.second); 
		}else if(partSet.partExist(mPos))
		{
			part.push_back(*(new Part(part.size())));
			partSet.removePart(mPos);
		}
	}
}

void Player::addTower(int type, sf::Vector2i pos)
{
	if(type == 0)
		return;
	bool freeSpace = true;
	for(int i = 0; i < tower.size(); i++)
	{
		if((helper::withinRectangle(helper::vectorIntToFloat(pos * BLOCK_SIZE), tower[i].pos(), tower[i].pos() + sf::Vector2f(96.0, 96.0))
		or helper::withinRectangle(helper::vectorIntToFloat(pos * BLOCK_SIZE) + sf::Vector2f(96.0, 96.0), tower[i].pos(), tower[i].pos() + sf::Vector2f(96.0, 96.0))) and helper::distance(helper::vectorIntToFloat(pos * BLOCK_SIZE), tower[i].pos()) < 96.0)
			freeSpace = false;
		if(tower[i].pos() == helper::vectorIntToFloat(pos * BLOCK_SIZE) and tower[i].ID() == type and tower[i].ID() != towerShape::towerID::Sentry)
			freeSpace = true;
	}
	if(freeSpace)
	{
		switch(type)
		{
			case towerShape::towerID::Hospital:
			{
				partSet.removeTower(towerShape::towerID::Hospital, pos);
				tower.push_back(*(new Hospital(pos)));
				break;
			}
			case towerShape::towerID::LaserPad:
			{
				partSet.removeTower(towerShape::towerID::LaserPad, pos);
				tower.push_back(*(new LaserPad(pos)));
				break;
			}
			case towerShape::towerID::Sentry:
			{
				partSet.removeTower(towerShape::towerID::Sentry, pos);
				tower.push_back(*(new Sentry(pos)));
				break;
			}
			case towerShape::towerID::WeaponPack:
			{
				partSet.removeTower(towerShape::towerID::WeaponPack, pos);
				missCapacity += missAdd;
				if (missAdd > 3)
				{
					missAdd --;
				}
				break;
			}
			case towerShape::towerID::WeaponUpgrade:
			{
				partSet.removeTower(towerShape::towerID::WeaponUpgrade, pos);
				missCapacity += 5;
				laserDamage += .4 / sqrt(laserDamage);
				laserCapacity *= 1.7;
				//sound.play();
				break;
			}
		}
		
	}
}

void Player::draw(sf::RenderWindow *window, Map *map)
{
	partSet.draw(window);
	for(int i = 0; i < tower.size(); i++)
	{
		tower[i].draw(window);
	}
	for(int i = 0; i < shot.size(); i++)
	{
		shot[i].update();
		shot[i].draw(window);
		if(shot[i].expired(map))
			 shot.erase(shot.begin() + i);
	}
	for(int i = 0; i < miss.size(); i++)
	{
		miss[i].update();
		miss[i].draw(window, map);
		if(miss[i].expired(map))
			 miss.erase(miss.begin() + i);
	}
	for(int i = 0; i < sizeof(pickup)/sizeof(pickup[0]); i++)
		pickup[i].draw(window);
	window->draw(body);		
}

void Player::drawAt(sf::RenderWindow *window, std::vector<float>& nums)
{
	otherBody.setPosition(nums[0], nums[1]);
	otherBody.setRotation(nums[2]);
	window->draw(body);
}

float Player::playerHealth()
{
	return this->health;
}

float Player::playerLaserHealth()
{
	return laserHealth;
}

float Player::playerLaserCapacity()
{
	return laserCapacity;
}

int Player::playerMissileCapacity()
{
	return missCapacity;
}

int Player::partCount()
{
	return part.size();
}

void Player::orient(sf::Vector2f cursor)
{
	this->rotation = (180/PI) * atan2((cursor.y - position.y), (cursor.x - position.x));
	body.setRotation(rotation);
}

void Player::move(float x, float y, Map *map)
{
	static float speed = 1.3;
	float s = speed;
	if(map->collide(sf::Vector2f(position.x + x * s, position.y + y * s), PLAYER_RADIUS))
	{
		if(!map->collide(sf::Vector2f(position.x, position.y + y * s), PLAYER_RADIUS))
		{
			body.move(0.0, (float)y * s * frameElapsed.asMicroseconds() / 10000);
			position.y += y * s * frameElapsed.asMicroseconds() / 10000;
		}else if(!map->collide(sf::Vector2f(position.x + x * s, position.y), PLAYER_RADIUS))
		{
			body.move((float)x * s * frameElapsed.asMicroseconds() / 10000, 0.0);
			position.x += x * s * frameElapsed.asMicroseconds() / 10000;
		}
	}
	while(map->collide(sf::Vector2f(position.x + x * s, position.y + y * s), PLAYER_RADIUS))
		s -= 1;
	if(s < 0)
		s = 0;
	body.move((float)x * s * frameElapsed.asMicroseconds() / 10000, (float)y * s * frameElapsed.asMicroseconds() / 10000);
	position.x += x * s * frameElapsed.asMicroseconds() / 10000;
	position.y += y * s * frameElapsed.asMicroseconds() / 10000;
}

std::pair<float, sf::Vector2f> Player::checkEnemyCollide(sf::Vector2f p, float radius, Map* map)
{
	if(helper::distance(p, position) < PLAYER_RADIUS + radius and health >= 0)
	{
		health -= ((float)frameElapsed.asMicroseconds()) / 10000.0;
	}
	for(int i = 0; i < shot.size(); i++)
	{
		if(helper::distance(shot[i].pos(), p) < 5 + radius)
		{
			shot.erase(shot.begin() + i);
			return std::make_pair(laserDamage, shot[i].getDirection());
		}
	}
	for(int i = 0; i < miss.size(); i++)
	{
		float dist = helper::distance(miss[i].pos(), p);
		if(dist < 10 + radius and !miss[i].didUse())
		{
			if(radius <= 16)
			{
				miss[i].blowUp();
				return std::make_pair(100000.0/(dist * dist + .01), sf::Vector2f(0.0, 0.0));
			}else
			{
				miss[i].reverse();
			}
		}
	}
	for(int i = 0; i < tower.size(); i++)
	{
		sf::Vector2f tPos = tower[i].pos() + sf::Vector2f(48.0, 48.0);
		if(tower[i].ID() == towerShape::towerID::Sentry and tower[i].clock() > 150 and helper::distance(tPos, p) < towerShape::sentryRange and !map->collideSegment(tPos, p))
		{
			tower[i].restartClock();
			sf::Vector2f dir = p - tPos;
			float r;
			r = (180/PI) * atan2(dir.y, dir.x);
			shot.push_back(*(new Laser(tPos, helper::unitVector(tPos, p), r, (laserDamage >= 2.0))));
		}
	}
	return std::make_pair(0.0, sf::Vector2f(0.0, 0.0));
}

sf::Vector2f Player::pos()
{
	return this->position;
}

float Player::getRotation()
{
	return this->rotation;
}

void Player::fireLaser(sf::Vector2f cursor)
{
	shot.push_back(*(new Laser(position, helper::unitVector(position, cursor), rotation, (laserDamage >= 2.0))));
	laserHealth -= 2.0;
}

void Player::fireMissile(sf::Vector2f cursor)
{
	miss.push_back(*(new Missile(position, helper::unitVector(position, cursor), rotation)));
	missCapacity--;
}
