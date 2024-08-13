#pragma once
#include <functional>

class Action {
public:
	Action() = delete;
	Action(std::function<void()> funcAction, int8_t counterTick) : funcAction(funcAction), counterTick(counterTick) {}
	~Action() {};
	virtual void doAction() = 0;
	bool isCounterEnd() const;
protected:
	void subOneTick();
	void callFuncAction();
private:
	int8_t counterTick;
	std::function<void()> funcAction;
};