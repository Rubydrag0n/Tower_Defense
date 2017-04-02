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

