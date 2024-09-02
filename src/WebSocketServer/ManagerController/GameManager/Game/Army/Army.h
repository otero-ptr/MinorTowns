#pragma once
#include <cstdint>

class Army {
public:
	Army() = delete;
	Army(uint32_t soldiers);
	~Army();
	bool detach(const uint32_t soldiers) noexcept;
	void merge(const uint32_t soldiers) noexcept;
	const uint32_t getCount() const;
private:
	uint32_t soldiers;
};