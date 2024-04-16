#include "MessagePool.h"
#include <iostream>

void MessagePool::pushBackMessage(std::string msg)
{
	if (!msg.empty()) {
		this->pool.push(msg);
	}
}

std::string MessagePool::popFrontMessage()
{
	std::string msg = this->pool.front();
	this->pool.pop();
	return msg;
}

bool MessagePool::isEmpty()
{
	return this->pool.empty();
}
