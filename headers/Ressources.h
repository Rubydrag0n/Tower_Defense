#pragma once
class Ressources
{
public:
	Ressources();
	Ressources(int gold, int wood, int stone, int iron, int energy, int water, int food);

	//set and get all the different values
	void set_gold(int gold);
	int get_gold() const;
	void set_wood(int wood);
	int get_wood() const;
	void set_stone(int stone);
	int get_stone() const;
	void set_iron(int iron);
	int get_iron() const;
	void set_energy(int energy);
	int get_energy() const;
	void set_water(int water);
	int get_water() const;
	void set_food(int food);
	int get_food() const;

private:
	int mGold;
	int mWood;
	int mStone;
	int mIron;
	int mEnergy;
	int mWater;
	int mFood;
};
