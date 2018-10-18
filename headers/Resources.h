#pragma once
#include <map>
#include "Enums.h"

class Resources
{
public:
	Resources();
	Resources(int gold, int wood, int stone, int iron, int energy, int water, int food);
	explicit Resources(Resources* resource, Resources* limit = nullptr);

	//set and get all the different values
	void set_resources(int gold, int wood, int stone, int iron, int energy, int water, int food);
	
	//set and get individual resource
	void set_resource(RESOURCETYPES type, int res);
	int get_resource(RESOURCETYPES type);

	//sets all resources to 0
	void set_empty();
	//checks if all resources are 0
	bool is_empty();

	//add and sub individual resources, checking for bounds (underflow, overflow)
	//on overflow resource is set to the limit
	//on underflow nothing is done and false returned
	void add(RESOURCETYPES type, int res);
	bool sub(RESOURCETYPES type, int res);

	//subtracts cost resources if possible, returns true on success
	//cost is unchanged
	//can't be const cause for some reason operator[] of vector is not const
	bool sub(Resources *cost);

	//adds resources, checking for limits
	void add(Resources *income);

	Resources operator/(const int &d);
	//sets new limit, current resources will not change
	void set_limit(Resources* limit);

	//transfers all the resources it can into this object taking into account the limit
	//the source is emptied during this
	//returns true if the source is empty afterwards
	//false otherwise
	bool transfer(Resources *source);

private:
	std::map<RESOURCETYPES, int> mResources;

	//if there should be a limit to how many resources there can be
	Resources* mLimit;
};
