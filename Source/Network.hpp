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
	public:
		Liason();
		virtual void send(std::string);
		virtual std::string retrieve();	
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


