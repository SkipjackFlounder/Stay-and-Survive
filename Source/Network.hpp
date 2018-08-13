#ifndef _NETWORK_HPP
#define _NETWORK_HPP

#include <SFML/Network.hpp>
#include "Game.hpp"

class Liason
{
	protected:
		sf::IpAddress ip;
		unsigned short port;
		std::string name;
		sf::TcpSocket client;
		std::atomic<bool> quit;
		std::string line;
		std::mutex mtx;
		sf::Clock netClock;
	public:
		Liason();
		virtual void send(std::string);
		virtual std::string retrieve();	
		virtual void listen();
};

class Host : public Liason
{
	private:
		sf::TcpListener listener;
	public:
		Host();
};

class Client : public Liason
{
	private:
	public:
		Client();
};

#endif


