#include "Map.hpp"

Map::Map()
{
	std::ifstream file;
	file.open("Resources/Maps/1.txt");
	block.push_back(std::vector<int>());
	if(file.is_open())
	{
		char content;
		int y = 0;
		while(!file.eof())
		{
			file.get(content);
			if(content == '0')
				block[y].push_back(0);
			else if(content == '1')
				block[y].push_back(1); 
			else if(content == '2')
				block[y].push_back(2);
			else if(content == '\n')
			{
				block.push_back(std::vector<int>());
				y++;
			}
	 	}
	}
	file.close();
	blockTex.loadFromFile("Resources/Block.png");
	tBlockTex.loadFromFile("Resources/TransparentBlock.png");
}

bool Map::collide(sf::Vector2f pos, float radius)
{
	sf::Vector2i p = sf::Vector2i((int)(pos.x/BLOCK_SIZE), (int)(pos.y/BLOCK_SIZE));
	for(int i = p.y - 2; i < p.y + 2; i++)
	{
		for(int j = p.x - 2; j < p.x + 2; j++)
		{
			if(i >= 0 and j >= 0 and j < block[i].size() and i < block.size())
				if(block[i][j] != 0)
					if(helper::withinRectangle(pos, sf::Vector2f((float)j * BLOCK_SIZE - radius, (float)i * BLOCK_SIZE), sf::Vector2f((float)j * BLOCK_SIZE + BLOCK_SIZE + radius, (float)i * BLOCK_SIZE + BLOCK_SIZE))
					or helper::withinRectangle(pos, sf::Vector2f((float)j * BLOCK_SIZE, (float)i * BLOCK_SIZE - radius), sf::Vector2f((float)j * BLOCK_SIZE + BLOCK_SIZE, (float)i * BLOCK_SIZE + BLOCK_SIZE + radius))
					or helper::distance(pos, sf::Vector2f((float)j * BLOCK_SIZE, (float)i * BLOCK_SIZE)) < radius
					or helper::distance(pos, sf::Vector2f((float)j * BLOCK_SIZE + BLOCK_SIZE, (float)i * BLOCK_SIZE)) < radius
					or helper::distance(pos, sf::Vector2f((float)j * BLOCK_SIZE + BLOCK_SIZE, (float)i * BLOCK_SIZE + BLOCK_SIZE)) < radius
					or helper::distance(pos, sf::Vector2f((float)j * BLOCK_SIZE, (float)i * BLOCK_SIZE + BLOCK_SIZE)) < radius)
						return true;
		}
	}
	return false;
}

sf::Vector2f Map::directionalCollide(sf::Vector2f pos, float radius)
{
	if(collide(pos, radius) and radius > 16)
	{
		float closestDistance = -1;
		sf::Vector2f direction = sf::Vector2f(0, 0);
		sf::Vector2i p = sf::Vector2i((int)(pos.x/BLOCK_SIZE), (int)(pos.y/BLOCK_SIZE));
		for(int i = p.y - 2; i < p.y + 2; i++)
		{
			for(int j = p.x - 2; j < p.x + 2; j++)
			{
				if(i >= 2 and j >= 2 and j < block[i].size() - 2 and i < block.size() - 2)
					if(block[i][j])
					{
						float d = helper::distance(pos, helper::vectorIntToFloat(sf::Vector2i(j * BLOCK_SIZE + BLOCK_SIZE/2, i * BLOCK_SIZE + BLOCK_SIZE/2)));
						if(d < closestDistance or closestDistance == -1)
						{
							closestDistance = d;
							direction = helper::unitVector(pos, helper::vectorIntToFloat(sf::Vector2i(j * BLOCK_SIZE + BLOCK_SIZE/2, i * BLOCK_SIZE + BLOCK_SIZE/2)));
						}
					}
			}
		}
		return -direction;
	}
	return sf::Vector2f(0, 0);
}

bool Map::blocked(sf::Vector2i pos)
{
	if(block[pos.y][pos.x] == 0)
		return false;
	return true;
}

bool Map::collideSegment(sf::Vector2f a, sf::Vector2f b)
{
	for(int i = 0; i < block.size(); i++)
	{
		for(int j = 0; j < block[i].size(); j++)
		{
			if(block[i][j] != 0)
			{
				sf::Vector2f blockPos = sf::Vector2f((float)j * BLOCK_SIZE + BLOCK_SIZE/2, (float)i * BLOCK_SIZE + BLOCK_SIZE/2);
				if(helper::distanceToLineSegment(blockPos.x, blockPos.y, a.x, a.y, b.x, b.y) <= BLOCK_SIZE)
					return true;
			}
		}
	}
	return false;
}

sf::Vector2i Map::dimensions()
{
	return sf::Vector2i(block[0].size(), block.size());
}

sf::Vector2f Map::find(sf::Vector2f position, sf::Vector2f destination, float radius)
{
	std::vector< std::vector<bool> > closed;
	for(int i = 0; i < block.size(); i++)
	{
		closed.push_back(std::vector<bool>());
		for(int j = 0; j < block[i].size(); j++)
		{
			if(block[i][j] == 0)
				closed[i].push_back(false);
			else
				closed[i].push_back(true);
		}
	}
	std::vector<PathTile> openSet;
	std::vector<PathTile> closedSet;
	const sf::Vector2i cardinal[4] = 
	{sf::Vector2i(0, -1), sf::Vector2i(1, 0), 
	sf::Vector2i(0, 1), sf::Vector2i(-1, 0)};
	
	sf::Vector2i start = sf::Vector2i((int)(position.x/BLOCK_SIZE), (int)(position.y/BLOCK_SIZE));
	openSet.push_back(*(new PathTile(start)));
	closed[start.y][start.x] = true;
	sf::Vector2i end = sf::Vector2i((int)(destination.x/BLOCK_SIZE), (int)(destination.y/BLOCK_SIZE));
	bool found = false;
	while(openSet.size() > 0 and !found)
	{
		float currentLowest = 0; int p;
		for(int i = 0; i < openSet.size(); i++)
		{
			if(openSet.at(i).f <= currentLowest or currentLowest == 0)
				{currentLowest = openSet[i].f; p = i;}
		}
		for(int i = 0; i < 4; i++)
		{
			sf::Vector2i c = openSet[p].position + cardinal[i];
			if(c.y >= 0 and c.y < closed.size()
			and c.x >= 0 and c.x < closed[0].size())
			{
				if(!closed[c.y][c.x])
				{
					closed[c.y][c.x] = true;
					openSet.push_back(*(new PathTile(c)));
					int n = openSet.size() - 1;
					openSet[n].g = openSet[p].g + 1;
					openSet[n].h = helper::distance(helper::vectorIntToFloat(c), destination);
					openSet[n].f = openSet[n].g + openSet[n].h;
					openSet[n].dir = cardinal[i];
					if(openSet[n].position == end)
					{
						//std::cout << "FOUND\n";
						found = true;
						closedSet.push_back(*(&openSet[n]));
					}
				}
			}
		}
		//std::cout << openSet[p].position.x << " " << openSet[p].position.y << "--\n";
		closedSet.push_back(*(&openSet[p]));
		openSet.erase(openSet.begin() + p);
	}
	if(!found)
		return sf::Vector2f(0, 0);
	int lastNode = closedSet.size() - 2;
	//std::cout << start.x << " s: " << start.y << "\n";
	//std::cout << end.x << " e: " << end.y << "\n";
	sf::Vector2i direction;
	std::vector<sf::Vector2f> route;
	while(closedSet[lastNode].position != start)
	{
		direction = -closedSet[lastNode].dir;
		for(int i = 0; i < closedSet.size(); i++)
		{
			if(closedSet[i].position == closedSet[lastNode].position + direction)
			{
				route.push_back(helper::vectorIntToFloat((closedSet[lastNode].position)*BLOCK_SIZE + sf::Vector2i(BLOCK_SIZE/2, BLOCK_SIZE/2)));
				//std::cout << closedSet[lastNode].position.x << " " << closedSet[lastNode].position.y << "\n"; 
				closedSet.erase(closedSet.begin() + lastNode); 
				if(i < lastNode)
					lastNode = i;
				else
					lastNode = i - 1;
				break;
			}
		}
	}
	int goodNode = route.size() - 1;
	for(int i = route.size() - 1; i >= 0; i--)
	{
		sf::Vector2f u = helper::unitVector(position, route[i]);
		sf::Vector2f unit = sf::Vector2f(u.x * radius, u.y * radius);
		if(!collideSegment(position, route[i]))
			goodNode = i;
	}
	return helper::unitVector(position, route[goodNode]);
}

void Map::draw(sf::RenderWindow* window)
{
	for(int i = 0; i < block.size(); i++)
	{
		for(int j = 0; j < block[i].size(); j++)
		{
			if(block[i][j] == 1)
			{
				 square.setTexture(blockTex);
				 square.setScale(BLOCK_SIZE/512.0f, BLOCK_SIZE/512.0f);
				 square.setPosition((float)j * BLOCK_SIZE, (float)i * BLOCK_SIZE);
				 window->draw(square);
			}
			if(block[i][j] == 2)
			{
				 square.setTexture(tBlockTex);
				 square.setScale(BLOCK_SIZE/512.0f, BLOCK_SIZE/512.0f);
				 square.setPosition((float)j * BLOCK_SIZE, (float)i * BLOCK_SIZE);
				 window->draw(square);
			}	
		}
	}
}
