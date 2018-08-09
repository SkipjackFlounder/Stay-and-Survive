#include "Part.hpp"

PartSet::PartSet()
{
	tex.loadFromFile("Resources/Part.png");
	partBody.setTexture(tex);
}

bool PartSet::partExist(sf::Vector2f pos)
{
	for(int i = 0; i < part.size(); i++)
	{
		if(part[i] == sf::Vector2i((int)(pos.x/BLOCK_SIZE), (int)(pos.y/BLOCK_SIZE)))
			return true;
	}
	return false;
}

std::pair<towerShape::towerID, sf::Vector2i> PartSet::addPart(sf::Vector2f pos)
{
	part.push_back(sf::Vector2i((int)(pos.x/BLOCK_SIZE), (int)(pos.y/BLOCK_SIZE)));
	
	for(int i = 0; i < part.size(); i++)
	{
		switch(detectTower(i))
		{
			case towerShape::towerID::Hospital:
				return std::make_pair(towerShape::towerID::Hospital, part[i]);
			case towerShape::towerID::LaserPad:
				return std::make_pair(towerShape::towerID::LaserPad, part[i]);
			case towerShape::towerID::Sentry:
				return std::make_pair(towerShape::towerID::Sentry, part[i]);
			case towerShape::towerID::WeaponPack:
				return std::make_pair(towerShape::towerID::WeaponPack, part[i]);
			case towerShape::towerID::WeaponUpgrade:
				return std::make_pair(towerShape::towerID::WeaponUpgrade, part[i]);
			default:
				{}
		}
	}
}

int PartSet::detectTower(int n)
{
	for(int h = 0; h < towerShape::towerCount; h++)
	{
		bool isTower = true;
		bool hasNewPart = false;
		for(int i = part[n].x; i < part[n].x + 3; i++)
		{
			for(int j = part[n].y; j < part[n].y + 3; j++)
			{
				if((partExist(helper::vectorIntToFloat(sf::Vector2i(i, j) * BLOCK_SIZE)) and towerShape::towerArray[h][i - part[n].x][j - part[n].y] == 0) or (!partExist(helper::vectorIntToFloat(sf::Vector2i(i, j) * BLOCK_SIZE)) and towerShape::towerArray[h][i - part[n].x][j - part[n].y] == 1))
					isTower = false;
				if(i == part[part.size() - 1].x and j == part[part.size() - 1].y)
					hasNewPart = true;
			}
		}
		if(isTower and hasNewPart)
		{
			return static_cast<towerShape::towerID>(h + 1);
		}
	}
}

void PartSet::removeTower(int towerID, sf::Vector2i pos)
{
	for(int i = pos.x; i < pos.x + 3; i++)
	{
		for(int j = pos.y; j < pos.y + 3; j++)
		{
			if(towerShape::towerArray[static_cast<int>(towerID) - 1][i - pos.x][j - pos.y] == 1)
				removePart(sf::Vector2f((float)(i * BLOCK_SIZE), (float)(j * BLOCK_SIZE)));
		}
	}
}

void PartSet::removePart(sf::Vector2f pos)
{
	for(int i = 0; i < part.size(); i++)
	{
		if(part[i] == sf::Vector2i((int)(pos.x/BLOCK_SIZE), (int)(pos.y/BLOCK_SIZE)))
		{
			part.erase(part.begin() + i);
			return;
		}
	}
}

void PartSet::draw(sf::RenderWindow *window)
{
	for(int i = 0; i < part.size(); i++)
	{
		partBody.setPosition(helper::vectorIntToFloat(part[i] * BLOCK_SIZE));
		window->draw(partBody);
	}
}

Part::Part(int num)
{
	body.setPosition(33.0, 706.0);
	body.move((float)((81 - 33) * num), 0);
	tex.loadFromFile("Resources/PartInBar.png");
	body.setTexture(tex);
}

void Part::draw(sf::RenderWindow* window)
{
	window->draw(body);
}

Pickup::Pickup()
{
	tex.loadFromFile("Resources/Pickup.png");
	body.setTexture(tex);
	body.setOrigin(8.0, 8.0);
	body.setPosition(100.0, 100.0);
}

Pickup::Pickup(Map* map)
{
	tex.loadFromFile("Resources/Pickup.png");
	body.setTexture(tex);
	body.setOrigin(8.0, 8.0);
	body.setPosition(100.0, 100.0);
	relocate(map);
}

bool Pickup::update(sf::Vector2f playerPos, float radius, Map *map, int partCount)
{
	if(helper::distance(playerPos, body.getPosition()) < radius + 8)
	{
		relocate(map);
		return true;
	}
	return false;
}

void Pickup::relocate(Map *map)
{
	do{
		body.setPosition(sf::Vector2f((float)(rand()%map->dimensions().x) * BLOCK_SIZE, (float)(rand()%map->dimensions().y) * BLOCK_SIZE));
	}while(map->collide(body.getPosition(), 8 + 3));
}

void Pickup::draw(sf::RenderWindow* window)
{
	window->draw(body);
}
