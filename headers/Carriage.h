#pragma once
#include "Unit.h"
#include "Building.h"
#include "Resources.h"
#include "Production.h"

class Carriage final
	: public Unit
{
public:
	explicit Carriage(const std::string& unit_name, Level* level, Building* source = nullptr, Building* drain = nullptr);

	void on_tick() override;

	void set_source(Building* b);
	void set_drain(Building* b);

	Building* get_source() const;
	Building* get_drain() const;

	bool move_towards(SDL_Point target);

	void move();

	bool update_checkpoints_to(Building* source, Building* target);

private:
	//building the carriage is getting resources from
	Building* mSource;
	//building the carriage is bringing resources to
	Building* mDrain;

	//total capacity
	Resources* mCurrent_resources;

	//what will be transported, saved as what the drain is consuming
	Production mTransporting;

	ACTIVITY mCurrent_activity;

	std::vector<SDL_Point> mCheckpoints;

	Level* mLevel;
};
