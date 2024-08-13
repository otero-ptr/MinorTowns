#pragma once
#include "GameMinorTowns\GameManager\Game\ActionManager\Action\Action.h"

class ActionForever : public Action {
public:
	ActionForever(std::function<void()> funcAction, int8_t counterTick);
	~ActionForever();
	virtual void doAction() override;
};