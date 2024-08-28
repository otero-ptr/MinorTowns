#pragma once
#include <cstdint>

class NodeMap {
public:
	NodeMap(uint8_t id);
	~NodeMap();
	uint8_t getID() const;
private:
	uint8_t id;
};