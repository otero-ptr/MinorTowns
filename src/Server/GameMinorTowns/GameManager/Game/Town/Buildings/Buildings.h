#pragma once

enum TypeBuilding {
	Church = 0,
	Manufactory = 1,
	MaxValue = 2
};

class Buildings {
public:
	void build(const int& type);
	void destroy(const int& type);
	bool isTypeExists(const int& type);
	int	getCountBuildings(const int& type);
	void setPriceBuildings(const int& type, double value);
	double getPriceBuildings(const int& type);
private:
	int church = 0;
	int manufactory = 0;
	double priceChurch;
	double priceManufactory;
};