#include "ActionLast.h"

ActionLast::ActionLast(std::function<void()> funcAction, int8_t counterTick) : Action(funcAction, counterTick)
{

}

ActionLast::~ActionLast()
{
}

void ActionLast::doAction()
{
	this->subOneTick();
	if (this->isCounterEnd()) {
		this->callFuncAction();
	}
}
