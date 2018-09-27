#include "Resources.h"

Resources::Resources()
{
	mGold = 0;
	mWood = 0;
	mStone = 0;
	mIron = 0;
	mEnergy = 0;
	mWater = 0;
	mFood = 0;
}

Resources::Resources(int gold, int wood, int stone, int iron, int energy, int water, int food)
{
	mGold = gold;
	mWood = wood;
	mStone = stone;
	mIron = iron;
	mEnergy = energy;
	mWater = water;
	mFood = food;
}

void Resources::set_resources(int gold, int wood, int stone, int iron, int energy, int water, int food)
{
	mGold = gold;
	mFood = food;
	mWood = wood;
	mStone = stone;
	mIron = iron;
	mWater = water;
	mEnergy = energy;
}

void Resources::set_gold(int gold)
{
	mGold = gold;
}

int Resources::get_gold() const
{
	return mGold;
}

void Resources::set_wood(int wood)
{
	mWood = wood;
}

int Resources::get_wood() const
{
	return mWood;
}

void Resources::set_stone(int stone)
{
	mStone = stone;
}

int Resources::get_stone() const
{
	return mStone;
}

void Resources::set_iron(int iron)
{
	mIron = iron;
}

int Resources::get_iron() const
{
	return mIron;
}

void Resources::set_energy(int energy)
{
	mEnergy = energy;
}

int Resources::get_energy() const
{
	return mEnergy;
}

void Resources::set_water(int water)
{
	mWater = water;
}

int Resources::get_water() const
{
	return mWater;
}

void Resources::set_food(int food)
{
	mFood = food;
}

int Resources::get_food() const
{
	return mFood;
}

bool Resources::sub(Resources *cost)
{
	//Returns true if the subtracting succeeded, false if there wasn't enough ressources
	if (mGold - cost->get_gold() >= 0 &&
		mWood - cost->get_wood() >= 0 &&
		mStone - cost->get_stone() >= 0 &&
		mIron - cost->get_iron() >= 0 &&
		mEnergy - cost->get_energy() >= 0 &&
		mWater - cost->get_water() >= 0 &&
		mFood - cost->get_food() >= 0)
	{
		mGold -= cost->get_gold();
		mWood -= cost->get_wood();
		mStone -= cost->get_stone();
		mIron -= cost->get_iron();
		mEnergy -= cost->get_energy();
		mWater -= cost->get_water();
		mFood -= cost->get_food();
		return true;
	}
	else
	{
		return false;
	}
}

void Resources::add(Resources *income)
{
	mGold += income->get_gold();
	mWood += income->get_wood();
	mStone += income->get_stone();
	mIron += income->get_iron();
	mEnergy += income->get_energy();
	mWater += income->get_water();
	mFood += income->get_food();
}


Resources Resources::operator/(const int &d)
{
	Resources r;
	r.set_resources(this->get_gold() / 2, this->get_wood() / 2, this->get_stone() / 2,
		this->get_iron() / 2, this->get_energy() / 2, this->get_water() / 2, this->get_food() / 2);
	return r;
}
