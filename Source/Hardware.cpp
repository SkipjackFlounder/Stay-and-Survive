#include "Hardware.hpp"

std::string helper::intToString (int number)
{
	std::ostringstream oss;
	oss << number;
	return oss.str();
}

float helper::distance(sf::Vector2f a, sf::Vector2f b)
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

float helper::distanceToLine(sf::Vector2f a, sf::Vector2f l1, sf::Vector2f l2)
{
	return abs((l2.y - l1.y) * a.x - (l2.x - l1.x) * a.y + l2.x * l1.y - l2.y * l1.x)/sqrt((l2.y - l1.y) * (l2.y - l1.y) + (l2.x - l1.x) * (l2.x - l1.x));
}

/*float helper::distanceToLineSegment(sf::Vector2f a, sf::Vector2f l1, sf::Vector2f l2)
{
	if(l1.x == l2.x)
	{
		if((a.y > l1.y and a.y < l2.y) or (a.y < l1.y and a.y > l2.y))
			return abs(a.x - l1.x);
		else
			if(helper::distance(a, l1) < helper::distance(a, l2))
				return helper::distance(a, l1);
			else
				return helper::distance(a, l2);
	}else if(l1.y == l2.y)
	{
		if((a.x > l1.x and a.x < l2.x) or (a.x < l1.x and a.x > l2.x))
			return abs(a.y - l1.y);
		else
			if(helper::distance(a, l1) < helper::distance(a, l2))
				return helper::distance(a, l1);
			else
				return helper::distance(a, l2);
	}else
	{
		float slope = (l1.y - l2.y)/(l1.x - l2.x);
		float pSlope = -(l1.x - l2.x)/(l1.y - l2.y);
		float x = (l1.x * slope - a.x * pSlope - l1.y + a.y)/(slope - pSlope);
		float y = l1.y + slope * (x - l1.x);
		//std::cout << x << " " << y << "\n";
		if(((x > l1.x and x < l2.x) or (x < l1.x and x > l2.x)) and ((y > l1.y and x < l2.y) or (y < l1.y and y > l2.y)))
			return helper::distanceToLine(a, l1, l2);
		else
			if(helper::distance(a, l1) < helper::distance(a, l2))
				return helper::distance(a, l1);
			else
				return helper::distance(a, l2);
	}
}*/
float helper::distanceToLineSegment(float pointX, float pointY, float x1, float y1, float x2, float y2)
{
    float diffX = x2 - x1;
    float diffY = y2 - y1;
    if ((diffX == 0) && (diffY == 0))
    {
        diffX = pointX - x1;
        diffY = pointY - y1;
        return sqrt(diffX * diffX + diffY * diffY);
    }

    float t = ((pointX - x1) * diffX + (pointY - y1) * diffY) / (diffX * diffX + diffY * diffY);

    if (t < 0)
    {
        //point is nearest to the first point i.e x1 and y1
        diffX = pointX - x1;
        diffY = pointY - y1;
    }
    else if (t > 1)
    {
        //point is nearest to the end point i.e x2 and y2
        diffX = pointX - x2;
        diffY = pointY - y2;
    }
    else
    {
        //if perpendicular line intersect the line segment.
        diffX = pointX - (x1 + t * diffX);
        diffY = pointY - (y1 + t * diffY);
    }

    //returning shortest distance
    return sqrt(diffX * diffX + diffY * diffY);
}

bool helper::withinRectangle(sf::Vector2f object, sf::Vector2f bound1, sf::Vector2f bound2)
{
	if(object.x >= bound1.x and object.y >= bound1.y and object.x <= bound2.x and object.y <= bound2.y)
		return true;
	else
		return false;	
}

float helper::findZero(sf::Vector2f a, sf::Vector2f b)
{
	if(a.x != b.x)
	{
		float slope = (b.y - a.y)/(b.x - a.x);
		return a.x - (a.y/slope);
	}else
		return a.x;
}

bool helper::spacePressed(){
	static bool pressed = false;
	if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		pressed = false;
	else
	if(!pressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		pressed = true;
		return true;
	}
	return false;
}

bool helper::bPressed(){
	static bool pressed = false;
	if(!sf::Keyboard::isKeyPressed(sf::Keyboard::B))
		pressed = false;
	else
	if(!pressed && sf::Keyboard::isKeyPressed(sf::Keyboard::B))
	{
		pressed = true;
		return true;
	}
	return false;
}

bool helper::leftMouseClicked(){
	static bool pressed = false;
	if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		pressed = false;
	else
	if(!pressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		pressed = true;
		return true;
	}
	return false;
}

bool helper::rightMouseClicked(){
	static bool pressed = false;
	if(!sf::Mouse::isButtonPressed(sf::Mouse::Right))
		pressed = false;
	else
	if(!pressed && sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		pressed = true;
		return true;
	}
	return false;
}

bool helper::middleMouseClicked(){
	static bool pressed = false;
	if(!sf::Mouse::isButtonPressed(sf::Mouse::Middle))
		pressed = false;
	else
	if(!pressed && sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	{
		pressed = true;
		return true;
	}
	return false;
}

sf::Vector2f helper::vectorIntToFloat(sf::Vector2i var)
{
	return sf::Vector2f((float)var.x, (float)var.y);
}

sf::Vector2f helper::unitVector(sf::Vector2f v, sf::Vector2f u)
{
	if(u != v)
		return sf::Vector2f((u.x - v.x)/distance(u, v), (u.y - v.y)/distance(u, v));
	return sf::Vector2f(0.0, 0.0);
}
