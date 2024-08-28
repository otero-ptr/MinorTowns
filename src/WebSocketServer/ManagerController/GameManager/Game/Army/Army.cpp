#include "Army.h"

Army::Army(uint32_t soldiers) :
	soldiers(soldiers)
{	

}

Army::~Army()
{
}

bool Army::detach(const uint32_t soldiers) noexcept
{
	if (this->soldiers >= soldiers) {
		this->soldiers -= soldiers;
		return true;
	}
	else {
		return false;
	}
}

void Army::merge(const uint32_t soldiers) noexcept
{
	this->soldiers += soldiers;
}

const int Army::getCount() const
{
	return soldiers;
}
