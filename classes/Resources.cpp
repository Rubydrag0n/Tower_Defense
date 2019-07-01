#include "Resources.h"
#include <algorithm>

Resources::Resources() : mResources{ 0 }, mDisplay{ 0.0 }
{
	this->set_empty();
	this->mLimit = nullptr;
	this->set_display_zero();
}

Resources::Resources(const int gold, const int wood, const int stone, const int iron, const int energy, const int water, const int food) : mResources { 0 }, mDisplay{ 0.0 }
{
	this->set_resources(gold, wood, stone, iron, energy, water, food);
	this->mLimit = nullptr;
	this->set_display_zero();
}

Resources::Resources(Resources* resource, Resources* limit) : mResources{ 0 }, mDisplay{ 0.0 }
{
	for (auto i = 0; i < RESOURCES_TOTAL; i++) {
		this->set_resource(RESOURCETYPES(i), resource->get_resource(RESOURCETYPES(i)));
	}
	if (limit == nullptr) this->mLimit = nullptr;
	else {
		this->mLimit = new Resources(limit);
	}
	this->set_display_zero();
}

Resources::~Resources()
{
	delete mLimit;
}

void Resources::set_resources(const int gold, const int wood, const int stone, const int iron, const int energy, const int water, const int food)
{
	mResources[GOLD] = gold;
	mResources[FOOD] = food;
	mResources[WOOD] = wood;
	mResources[STONE] = stone;
	mResources[IRON] = iron;
	mResources[WATER] = water;
	mResources[ENERGY] = energy;
}

void Resources::set_resource(const RESOURCETYPES type, const int res)
{
	this->mResources[type] = res;
}

int Resources::get_resource(const RESOURCETYPES type)
{
	return mResources[type];
}

int* Resources::get_resource_pointer(const RESOURCETYPES type)
{
	return &mResources[type];
}

void Resources::set_empty()
{
	for (auto& resource : mResources)
	{
		resource = 0;
	}
}

bool Resources::is_empty()
{
	for (auto resource : mResources)
	{
		if (resource != 0) return false;
	}
	return true;
}

void Resources::add(const RESOURCETYPES type, const int res)
{
	if (mLimit != nullptr) {
		if (res + mResources[type] > mLimit->get_resource(type)) {
			mResources[type] = mLimit->get_resource(type);
			return;
		}
	}
	mResources[type] += res;
}

bool Resources::sub(const RESOURCETYPES type, const int res)
{
	if (mResources[type] - res < 0) {
		return false;
	}
	mResources[type] -= res;
	return true;
}

//Returns true if the subtracting succeeded, false if there wasn't enough resources
bool Resources::sub(Resources *cost)
{
	for (auto i = 0; i < RESOURCES_TOTAL; i++) {
		if (mResources[i] - cost->get_resource(RESOURCETYPES(i)) < 0) {
			return false;
		}
	}
	for (auto i = 0; i < RESOURCES_TOTAL; i++) {
		mResources[i] -= cost->get_resource(RESOURCETYPES(i));
	}
	return true;
}

void Resources::add(Resources *income)
{
	for (auto i = 0; i < RESOURCES_TOTAL; i++) {
		this->add(RESOURCETYPES(i), income->get_resource(RESOURCETYPES(i)));
	}
}

Resources Resources::operator/(const int &d)
{
	Resources r;

	for (auto i = 0; i < RESOURCES_TOTAL; i++) {
		r.set_resource(RESOURCETYPES(i), this->get_resource(RESOURCETYPES(i)) / d);
	}
	return r;
}

void Resources::set_limit(Resources* limit)
{
	delete mLimit;
	this->mLimit = new Resources(limit);
}

Resources* Resources::get_limit() const
{
	return mLimit;
}


bool Resources::transfer(Resources *source)
{
	if (this->mLimit == nullptr) {
		this->add(source);
		source->set_empty();
		return true;
	}

	for (auto i = 0; i < RESOURCES_TOTAL; i++) 
	{
		auto adding = mLimit->get_resource(RESOURCETYPES(i)) - mResources[RESOURCETYPES(i)];
		//skip if over full already
		if (adding <= 0) continue;
		adding = std::min(adding, source->get_resource(RESOURCETYPES(i)));
		this->add(RESOURCETYPES(i), source->get_resource(RESOURCETYPES(i)));
		source->sub(RESOURCETYPES(i), adding);
	}

	return source->is_empty();
}

bool Resources::transfer(const RESOURCETYPES type, int *r)
{
	if (this->mLimit == nullptr)
	{
		this->add(type, *r);
		*r = 0;
		return true;
	}

	auto adding = mLimit->get_resource(type) - mResources[type];
	if (adding > 0)
	{
		adding = std::min(adding, *r);
		this->add(type, *r);
		*r -= adding;
	}

	return *r == 0;
}

Resources Resources::get_display_resources()
{
	Resources res;

	for (auto i = 0; i < RESOURCES_TOTAL; i++) {
		this->mDisplay[i] += float(mResources[i] - mDisplay[i]) / 100.f;
		if (mResources[i] - mDisplay[i] < 1.f) mDisplay[i] = float(mResources[i]);
		res.set_resource(RESOURCETYPES(i), int(mDisplay[i]));
	}
	return res;
}

std::string Resources::get_name(const RESOURCETYPES type)
{
	//TODO: locale stuff monkaS
	switch (type) {
	case GOLD:
		return "Gold";
	case WOOD:
		return "Wood";
	case FOOD:
		return "Food";
	case IRON:
		return "Iron";
	case STONE:
		return "Stone";
	case WATER:
		return "Water";
	case ENERGY:
		return "Energy";
	default:
		return "Unknown Resource";
	}
}

void Resources::set_display_zero()
{
	for (auto i = 0; i < RESOURCETYPES::RESOURCES_TOTAL; i++) {
		this->mDisplay[RESOURCETYPES(i)] = 0;
	}
}
