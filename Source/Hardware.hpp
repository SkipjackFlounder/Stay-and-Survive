#ifndef _HARDWARE_HPP
#define _HARDWARE_HPP
#define PI 3.1415926536

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <sstream>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include <mutex>
#include <atomic>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <fstream>

const int SW = sf::VideoMode::getDesktopMode().width; //Get screenwidth
const int SH = sf::VideoMode::getDesktopMode().height; //Get screenheight

namespace helper
{	
	std::string intToString(int);
	float distance(sf::Vector2f, sf::Vector2f);
	float distanceToLine(sf::Vector2f, sf::Vector2f, sf::Vector2f);
	float distanceToLineSegment(float pointX, float pointY, float x1, float y1, float x2, float y2);
	bool withinRectangle(sf::Vector2f, sf::Vector2f, sf::Vector2f);
	float findZero(sf::Vector2f, sf::Vector2f);
	bool spacePressed();
	bool bPressed();
	bool leftMouseClicked();
	bool rightMouseClicked();
	bool middleMouseClicked();
	sf::Vector2f vectorIntToFloat(sf::Vector2i);
	sf::Vector2f unitVector(sf::Vector2f, sf::Vector2f);
}


#endif
