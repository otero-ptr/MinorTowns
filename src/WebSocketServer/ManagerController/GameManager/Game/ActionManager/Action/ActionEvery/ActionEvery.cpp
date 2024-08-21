#include "ActionEvery.h"

ActionEvery::ActionEvery(std::function<void()> funcAction, int8_t counterTick) : Action(funcAction, counterTick)
{

}

ActionEvery::~ActionEvery()
{
}

void ActionEvery::doAction()
{
	if (!this->isCounterEnd()) {
		this->callFuncAction();
		this->subOneTick();
	}
}
