#include "Ressources.h"



Ressources::Ressources()
{
	mGold = 0;
	mWood = 0;
	mStone = 0;
	mIron = 0;
	mEnergy = 0;
	mWater = 0;
	mFood = 0;
}

Ressources::Ressources(int gold, int wood, int stone, int iron, int energy, int water, int food)
{
	mGold = gold;
	mWood = wood;
	mStone = stone;
	mIron = iron;
	mEnergy = energy;
	mWater = water;
	mFood = food;
}

void Ressources::set_ressources(int gold, int wood, int stone, int iron, int energy, int water, int food)
{
	mGold = gold;
	mWood = wood;
	mStone = stone;
	mIron = iron;
	mEnergy = energy;
	mWater = water;
	mFood = food;
}

void Ressources::set_gold(int gold)
{
	mGold = gold;
}

int Ressources::get_gold() const
{
	return mGold;
}

void Ressources::set_wood(int wood)
{
	mWood = wood;
}

int Ressources::get_wood() const
{
	return mWood;
}

void Ressources::set_stone(int stone)
{
	mStone = stone;
}

int Ressources::get_stone() const
{
	return mStone;
}

void Ressources::set_iron(int iron)
{
	mIron = iron;
}

int Ressources::get_iron() const
{
	return mIron;
}

void Ressources::set_energy(int energy)
{
	mEnergy = energy;
}

int Ressources::get_energy() const
{
	return mEnergy;
}

void Ressources::set_water(int water)
{
	mWater = water;
}

int Ressources::get_water() const
{
	return mWater;
}

void Ressources::set_food(int food)
{
	mFood = food;
}

int Ressources::get_food() const
{
	return mFood;
}

bool Ressources::sub(Ressources *cost)
{
	//Returns true if the subtracting succeeded, false if there wasn't enough ressources
	if (mGold - cost->get_gold() > 0 &&
		mWood - cost->get_wood() > 0 &&
		mStone - cost->get_stone() > 0 &&
		mIron - cost->get_iron() > 0 &&
		mEnergy - cost->get_energy() > 0 &&
		mWater - cost->get_water() > 0 &&
		mFood - cost->get_food() > 0)
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

void Ressources::add(Ressources *income)
{
	mGold += income->get_gold();
	mWood += income->get_wood();
	mStone += income->get_stone();
	mIron += income->get_iron();
	mEnergy += income->get_energy();
	mWater += income->get_water();
	mFood += income->get_food();
}
