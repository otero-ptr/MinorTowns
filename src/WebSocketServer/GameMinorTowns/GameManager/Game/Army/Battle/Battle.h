#pragma once
#include <list>
#include <mutex>
class Army;

class Battle {
public:
	explicit Battle(Army& defender, Army& attacker, int node);
	~Battle();
	void join(Army& ally, bool side /* true - defenders || false - attackers*/);
	bool leave(Army& ally);
	bool battle(); /* false = battle end. true - batle continues */
	int calculateLosses(int total, int army, int losses);
private:
	std::list<Army*> defenders;
	std::list<Army*> attackers;
	int node;
	bool battles;
	std::mutex mtx;
};