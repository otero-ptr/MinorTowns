#include "ActionManager.h"

ActionManager::~ActionManager()
{
	if (!this->actions.empty()) {
		for (Action* action : this->actions) {
			delete action;
		}
		this->actions.clear();
	}
}

void ActionManager::addAction(Action* action)
{
	this->actions.push_back(action);
}

void ActionManager::doActions()
{
	if (!this->actions.empty()) {
		for (Action* action : this->actions) {
			action->doAction();
		}
	}
}

void ActionManager::clearCompletedActions()
{
	if (!this->actions.empty()) {
		this->actions.remove_if([](const Action* action) { if (action->isCounterEnd()) { delete action; return true; } else { return false; }});
	}
}
