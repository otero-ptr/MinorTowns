#pragma once
#include <string>
#include <queue>
#include <mutex>

class MessagePool {
public:
	void push(std::string msg);
	std::string pop();
	bool empty() const;
	void clear();
private:
	std::queue<std::string> pool;
	std::mutex mtx;
};