#include "Resources.h"

Resources::Resources()
{
	this->set_empty();
	this->mLimit = nullptr;
}

Resources::Resources(const int gold, const int wood, const int stone, const int iron, const int energy, const int water, const int food)
{
	this->set_resources(gold, wood, stone, iron, energy, water, food);
	this->mLimit = nullptr;
}

Resources::Resources(Resources* resource, Resources* limit)
{
	for (auto i = 0; i < RESOURCETYPES::RESOURCES_TOTAL; i++) {
		this->set_resource(RESOURCETYPES(i), resource->get_resource(RESOURCETYPES(i)));
	}
	if (limit != nullptr)
	{
		this->mLimit = limit;
	}
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

void Resources::set_empty()
{
	for (auto i = 0; i < RESOURCETYPES::RESOURCES_TOTAL; i++) {
		this->set_resource(RESOURCETYPES(i), 0);
	}
}

bool Resources::is_empty()
{
	for (auto i = 0; i < RESOURCETYPES::RESOURCES_TOTAL; i++) {
		if (mResources[RESOURCETYPES(i)] != 0) return false;
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
	for (auto i = 0; i < RESOURCETYPES::RESOURCES_TOTAL; i++) {
		if (mResources[RESOURCETYPES(i)] - cost->get_resource(RESOURCETYPES(i)) < 0) {
			return false;
		}
	}
	for (auto i = 0; i < RESOURCETYPES::RESOURCES_TOTAL; i++) {
		mResources[RESOURCETYPES(i)] -= cost->get_resource(RESOURCETYPES(i));
	}
	return true;
}

void Resources::add(Resources *income)
{
	for (auto i = 0; i < RESOURCETYPES::RESOURCES_TOTAL; i++) {
		this->add(RESOURCETYPES(i), income->get_resource(RESOURCETYPES(i)));
	}
}

Resources Resources::operator/(const int &d)
{
	Resources r;

	for (auto i = 0; i < RESOURCETYPES::RESOURCES_TOTAL; i++) {
		r.set_resource(RESOURCETYPES(i), this->get_resource(RESOURCETYPES(i)) / 2);
	}
	return r;
}

void Resources::set_limit(Resources * limit)
{
	this->mLimit = limit;
}

bool Resources::transfer(Resources *source)
{
	if (this->mLimit == nullptr) {
		this->add(source);
		source->set_empty();
		return true;
	}

	for (auto i = 0; i < RESOURCETYPES::RESOURCES_TOTAL; i++) 
	{
		const auto adding = mLimit->get_resource(RESOURCETYPES(i)) - mResources[RESOURCETYPES(i)];
		this->add(RESOURCETYPES(i), source->get_resource(RESOURCETYPES(i)));
		source->sub(RESOURCETYPES(i), adding);
	}

	return source->is_empty();
}
