#pragma once
#include "GameMinorTowns\GameManager\Game\ActionManager\Action\Action.h"

class ActionEvery : public Action {
public:
	ActionEvery(std::function<void()> funcAction, int8_t counterTick);
	~ActionEvery();
	virtual void doAction() override;
};