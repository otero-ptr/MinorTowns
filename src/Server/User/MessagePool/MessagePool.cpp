#include "MessagePool.h"

void MessagePool::push(std::string msg)
{
	std::lock_guard<std::mutex> guard(mtx);
	if (!msg.empty()) {
		pool.push(msg);
	}
}

std::string MessagePool::pop()
{
	std::lock_guard<std::mutex> guard(mtx);
	std::string msg = pool.front();
	pool.pop();
	return msg;
}

bool MessagePool::empty() const
{
	return pool.empty();
}

void MessagePool::clear()
{
	std::lock_guard<std::mutex> guard(mtx);
	while (!pool.empty()) {
		pool.pop();
	}
}
