#pragma once

class Army {
public:
	Army() = delete;
	Army(int soldiers, int ownTownId);
	~Army();
	void detach(const int soldiers);
	void merge(const int soldiers);
	const int getCount();
	const int getNode();
private:
	int soldiers;
	int ownTownId;
	int idNode;
};