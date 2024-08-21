#include "ActionForever.h"

ActionForever::ActionForever(std::function<void()> funcAction, int8_t counterTick) : Action(funcAction, counterTick)
{

}

ActionForever::~ActionForever()
{
}

void ActionForever::doAction()
{
	if (!this->isCounterEnd()) {
		this->callFuncAction();
	}
}
