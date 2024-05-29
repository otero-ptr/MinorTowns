#pragma once
#include <list>
#include "Action\Action.h"

class ActionManager {
public:
	~ActionManager();
	void addAction(Action* action);
	void doActions();
private:
	void clearCompletedActions();
	std::list<Action*> actions;
};