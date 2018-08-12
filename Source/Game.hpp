#include "Player.hpp"
#include "EnemyManager.hpp"

class Game
{
	private:
		Map map;
		Player player;
		EnemyManager eManager;
		int frameCount; //Counts how many frames since program began
		int score;
		std::vector<std::thread> thread; //(&EnemyManager::update, this, map, player);
		std::vector<float> otherPlayerFloats;
		sf::Font font;
		sf::Text missileText, scoreText, pausedText;
		sf::Sprite frame, healthBar, laserBar, back;
		sf::Texture tex[4];
		bool showInfoBar;
		bool buildMode;
		sf::Clock fireClock;
	public:
		Game();
		void update(sf::RenderWindow*, sf::View*);
		void handleInput(sf::RenderWindow*);
		void handleMessage(std::string, sf::RenderWindow*);
		std::string messageUpdates();
};
