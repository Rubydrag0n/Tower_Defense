#pragma once
#include "Damage.h"
#include "Enums.h"
#include <string>

/*
 * Holds all the defensive stats of a Unit: life, armor, resistances, immunities
 * Also able to directly calculate damage taken when given a Damage Object
 */
class Defense
{
public:
	Defense();
	Defense(double health, double armor, double magic_res, double fire_res, double water_res, double elec_res);

	//Setting all the different defenses
	void set_defenses(double health, double armor, double magic_res, double fire_res, double water_res, double elec_res);
	void set_defenses(DEFENSETYPES type, int value);
	int get_defense(DEFENSETYPES type);
	void set_immunities(bool phys, bool magic, bool fire, bool water, bool elec);
	void set_health(double health);
	double get_health() const;
	void set_full_health(double health);
	double get_full_health() const;
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
	//Returns true if unit got killed
	bool take_damage(Damage *dmg);

	static std::string get_name(DEFENSETYPES type);

	Defense get_display_defenses();

private:
	double mHealth;
	double mFull_health;

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

	int mDefenses[DEFENSETYPES_TOTAL];
	float mDisplay[DEFENSETYPES_TOTAL];

	void set_display_zero();
};
