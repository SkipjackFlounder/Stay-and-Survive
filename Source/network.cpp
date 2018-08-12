#include "Network.hpp"

std::string Liason::retrieve()
{
	sf::Packet packet;
	client.receive(packet);
	std::string line;
	if(packet >> line)
	{
		return line;
	}
	return "";
}

Liason::Liason()
{
	
}

void Liason::send(std::string data)
{
	sf::Packet packet;
	packet << data;
	client.send(packet);
}

Host::Host() : Liason()
{
	ip = sf::IpAddress::getPublicAddress();
	std::cout << "Public IPv4: " << ip;
	std::cout << "\nEnter Port: ";
	std::cin >> port;
	if (listener.listen(port) != sf::Socket::Done)
	{
		std::cout << "Cannot connect to port\n";
	}
	std::cout << "Waiting for connection...\n";
	if (listener.accept(client) != sf::Socket::Done)
	{
		std::cout << "Cannot connect to client\n";
	}
	std::cout << "Connected\n";
}

Client::Client() : Liason()
{
	sf::Socket::Status status;
	do
	{
		std::cout << "Enter IPv4: ";
		std::cin >> ip;
		std::cout << "Enter Port: ";
		std::cin >> port;
		std::cout << "Attempting to connnect...\n";
		status = client.connect(ip, port);
		std::cout << "Connecting...\n";
		if (status != sf::Socket::Done)
		{
			std::cout << "Cannot connect to host\n";
		}
	}while(status != sf::Socket::Done);
}