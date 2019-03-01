#pragma once

#include <map>
#include "Enums.h"
#include "Building.h"

class Production : public std::map<RESOURCETYPES, PRODUCTIONSTATE> {
public:
	Production();
	explicit Production(Building* building);

	//initializes the map and sets everything to "none"
	void initialize();

	//updates the map in case the building got upgraded etc.
	void update(Building* building);
};