#pragma once
#include "Entity.h"
#include "Unit.h"
#include "Building.h"
#include "Resources.h"

enum ACTIVITY {
	GETTING,
	DELIVERING,
	ACTIVITIES_TOTAL,
};

class Carriage
	: public Unit
{
public:
	explicit Carriage(std::string unit_name, Building* source = nullptr, Building* drain = nullptr);
	~Carriage();

	void render() override;
	void on_tick() override;

	void set_source(Building* b);
	void set_drain(Building* b);

	Building* get_source();
	Building* get_drain();

	void move();

private:
	//building the carriage is getting resources from
	Building* mSource;
	//building the carriage is bringing resources to
	Building* mDrain;

	//total capacity
	Resources* mCapacity;
	Resources* mCurrent_resources;

	ACTIVITY mCurrent_activity;
};