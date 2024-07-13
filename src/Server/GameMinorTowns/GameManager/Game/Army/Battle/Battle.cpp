#include "Battle.h"
#include "../Army.h"

Battle::Battle(Army& defender, Army& attacker, int node) : node(node), battles(true)
{
	this->defenders.push_back(&defender);
	this->attackers.push_back(&attacker);
}

Battle::~Battle()
{
}

void Battle::join(Army& ally, bool side)
{
	this->mtx.lock();
	if (side) {
		this->defenders.push_back(&ally);
	}
	else {
		this->attackers.push_back(&ally);
	}
	this->mtx.unlock();
}

bool Battle::leave(Army& ally)
{
	std::lock_guard<std::mutex> lock(this->mtx);
	if (this->defenders.remove_if([&ally](Army* army) {return (army == &ally); }) > 0) {
		return true;
	}
	if (this->attackers.remove_if([&ally](Army* army) {return (army == &ally); }) > 0) {
		return true;
	}
	return false;
}

bool Battle::battle()
{
	std::lock_guard<std::mutex> lock(this->mtx);
	if (!this->battles) {
		return false;
	}
	int total_defenders = 0, total_attackers = 0;
	for (auto& it : this->defenders) {
		total_defenders += it->getCount();
	}
	for (auto& it : this->attackers) {
		total_attackers += it->getCount();
	}
	if (total_defenders <= 0 || total_attackers <= 0) {
		this->battles = false;
		return false;
	}
	int percent_defenders = total_defenders * 0.1;
	int percent_attackers = total_attackers * 0.1;
	int battle_result = percent_defenders - percent_attackers;
	int losses_defenders, losses_attackers;
	if (battle_result > 0) {
		losses_defenders = percent_defenders - battle_result;
		losses_attackers = percent_attackers;
	}
	else if (battle_result < 0) {
		losses_defenders = percent_defenders;
		losses_attackers = percent_attackers + (battle_result * -1);
	}
	else {
		losses_defenders = percent_defenders;
		losses_attackers = percent_attackers;
	}

	for (auto& it : this->defenders) {
		it->detach(this->calculateLosses(total_defenders, it->getCount(), losses_defenders));
	}
	for (auto& it : this->attackers) {
		it->detach(this->calculateLosses(total_attackers, it->getCount(), losses_attackers));
	}
	return true;
}

int Battle::calculateLosses(int total, int army, int losses)
{
	int percent = (army / total) * 100;
	return losses * (percent / 100);
}
