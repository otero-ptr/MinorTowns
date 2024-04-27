#pragma once

enum TypeBuilding {
	Church = 0,
	Manufactory = 1,
	MaxValue = 2
};

class Buildings {
public:
	Buildings() = delete;
	Buildings(double priceIncrease);
	void build(const int& type);
	void destroy(const int& type);
	bool isTypeExists(const int& type);
	int	getCountBuildings(const int& type) const;
	void setPriceBuildings(const int& type, double value, double modifier);
	double getPriceBuildings(const int& type) const;
	double getModifierBuildings(const int& type) const;
	double getPriceIncrease() const;
private:
	int church = 0;
	int manufactory = 0;
	double priceChurch;
	double priceManufactory;

	double modifierChurch;
	double modifierManufactory;

	double priceIncrease;
};