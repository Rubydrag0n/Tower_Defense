#pragma once
#include "Damage.h"

class Defense
{
public:
	Defense();
	Defense(double armor, double magic_res, double fire_res, double water_res, double elec_res);

	//Setting all the different defenses
	void set_defenses(double armor, double magic_res, double fire_res, double water_res, double elec_res);
	void set_immunities(bool phys, bool magic, bool fire, bool water, bool elec);
	void set_armor(double armor);
	double get_armor() const;
	void set_magic_res(double magic_res);
	double get_magic_res() const;
	void set_fire_res(double fire_res);
	double get_fire_res() const;
	void set_water_res(double water_res);
	double get_water_res() const;
	void set_elec_res(double elec_res);
	double get_elec_res() const;

	//Calculating the actual Damage taken from an object of the Damage type
	double get_damage_taken(Damage *dmg) const;

private:
	double mArmor;
	double mMagic_res;
	double mFire_res;
	double mWater_res;
	double mElec_res;
	bool mPhys_immune;
	bool mMagic_immune;
	bool mFire_immune;
	bool mWater_immune;
	bool mElec_immune;
};
