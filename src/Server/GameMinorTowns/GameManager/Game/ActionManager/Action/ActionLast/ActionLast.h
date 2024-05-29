#pragma once
#include "GameMinorTowns\GameManager\Game\ActionManager\Action\Action.h"

class ActionLast : public Action {
public:
	ActionLast(std::function<void()> funcAction, int8_t counterTick);
	~ActionLast();
	virtual void doAction() override;
};