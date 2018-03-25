#ifndef SERVER_HPP
#define SERVER_HPP

#include <set>

class Server
{
public:
	//Instance for the Singleton design pattern;
	static Server* instance;

	Server();
	virtual ~Server();

	void update(double deltaTime);
	void exitGame();
	const bool getShouldClose();
private:

	bool shouldClose = false;
};

#endif //SERVER_HPP
