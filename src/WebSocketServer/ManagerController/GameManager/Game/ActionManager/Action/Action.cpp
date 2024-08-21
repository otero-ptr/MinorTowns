#include "Action.h"

bool Action::isCounterEnd() const
{
	if (this->counterTick < 1) {
		return true;
	}
	else {
		return false;
	}
}

void Action::subOneTick()
{
	--this->counterTick;
}

void Action::callFuncAction()
{
	if (this->funcAction) {
		this->funcAction();
	}
}
