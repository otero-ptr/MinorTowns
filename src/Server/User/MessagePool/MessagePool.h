#pragma once
#include <string>
#include <queue>

class MessagePool {
public:
	void pushBackMessage(std::string msg);
	std::string popFrontMessage();
	bool isEmpty();
private:
	std::queue<std::string> pool;
};