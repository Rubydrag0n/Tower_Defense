#pragma once
#include <string>
#include "Enums.h"

class Resources
{
public:
	Resources();
	Resources(double gold, double wood, double stone, double iron, double energy, double water, double food);
	Resources(int gold, int wood, int stone, int iron, int energy, int water, int food);
	explicit Resources(Resources* resource, Resources* limit = nullptr);

	~Resources();

	//set and get all the different values
	void set_resources(double gold, double wood, double stone, double iron, double energy, double water, double food);
	void set_resources(int gold, int wood, int stone, int iron, int energy, int water, int food);
	
	//set and get individual resource
	void set_resource(RESOURCETYPES type, double res);
	double get_resource(RESOURCETYPES type);
	double* get_resource_pointer(RESOURCETYPES type);

	//sets all resources to 0
	void set_empty();
	//checks if all resources are 0
	bool is_empty();

	//add and sub individual resources, checking for bounds (underflow, overflow)
	//on overflow resource is set to the limit
	//on underflow nothing is done and false returned
	void add(RESOURCETYPES type, double res);
	bool sub(RESOURCETYPES type, double res);

	//subtracts cost resources if possible, returns true on success
	//cost is unchanged
	//can't be const cause for some reason operator[] of vector is not const
	bool sub(Resources *cost);
	//if it is possible to subtract the resources return true, otherwise false
	bool sub_possible(Resources *cost);

	//adds resources, checking for limits
	void add(Resources *income);

	Resources operator/(const double &d);
	//sets new limit, current resources will not change
	void set_limit(const Resources& limit);
	Resources* get_limit() const;

	//transfers all the resources it can into this object taking into account the limit
	//the source is emptied during this
	//returns true if the source is empty afterwards
	//false otherwise
	bool transfer(Resources *source);

	//transfers one resource type
	//returns true if the source is empty afterwards
	//false otherwise
	bool transfer(RESOURCETYPES type, double *r);

	//returns a Resources object that is ticking behind the actual resources
	//to make it nice to display
	Resources get_display_resources();

	//returns a string literal containing the name of the resource
	static std::string get_name(RESOURCETYPES type);

private:
	double mResources[RESOURCES_TOTAL];
	double mDisplay[RESOURCES_TOTAL];

	void set_display_zero();

	//if there should be a limit to how many resources there can be
	Resources* mLimit;
};
