#pragma once
#include <cstdint>
#include <mutex>

class Buildings {
public:
	Buildings(uint32_t price) 
	: count(0), price(price) {

	}
	virtual void build() = 0;
	virtual void destroy() = 0;
	const uint32_t getCount() const {
		return this->count;
	}
	const uint32_t getPrice() const {
		return this->price;
	}
	virtual ~Buildings() = default;
protected:
	uint32_t count;
	uint32_t price;
	std::mutex mtx;
};