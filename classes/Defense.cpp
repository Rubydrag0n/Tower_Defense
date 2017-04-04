#include "Defense.h"

Defense::Defense()
{
	mArmor = 0.0;
	mMagic_res = 0.0;
	mFire_res = 0.0;
	mWater_res = 0.0;
	mElec_res = 0.0;
	mPhys_immune = false;
	mMagic_immune = false;
	mFire_immune = false;
	mWater_immune = false;
	mElec_immune = false;
}

Defense::Defense(double armor, double magic_res, double fire_res, double water_res, double elec_res)
{
	mArmor = armor;
	mMagic_res = magic_res;
	mFire_res = fire_res;
	mWater_res = water_res;
	mElec_res = elec_res;
	mPhys_immune = false;
	mMagic_immune = false;
	mFire_immune = false;
	mWater_immune = false;
	mElec_immune = false;
}

void Defense::set_defenses(double armor, double magic_res, double fire_res, double water_res, double elec_res)
{
	mArmor = armor;
	mMagic_res = magic_res;
	mFire_res = fire_res;
	mWater_res = water_res;
	mElec_res = elec_res;
}

void Defense::set_immunities(bool phys, bool magic, bool fire, bool water, bool elec)
{
	mPhys_immune = phys;
	mMagic_immune = magic;
	mFire_immune = fire;
	mWater_immune = water;
	mElec_immune = elec;
}

void Defense::set_armor(double armor)
{
	mArmor = armor;
}

double Defense::get_armor() const
{
	return mArmor;
}

void Defense::set_magic_res(double magic_res)
{
	mMagic_res = magic_res;
}

double Defense::get_magic_res() const
{
	return mMagic_res;
}

void Defense::set_fire_res(double fire_res)
{
	mFire_res = fire_res;
}

double Defense::get_fire_res() const
{
	return mFire_res;
}

void Defense::set_water_res(double water_res)
{
	mWater_res = water_res;
}

double Defense::get_water_res() const
{
	return mWater_res;
}

void Defense::set_elec_res(double elec_res)
{
	mElec_res = elec_res;
}

double Defense::get_elec_res() const
{
	return mElec_res;
}

double Defense::get_damage_taken(Damage *dmg) const
{
	auto phys_dmg = dmg->get_phys_dmg() * 100 / (100 + mArmor);
	if (mPhys_immune) phys_dmg = 0.0;

	auto magic_dmg = dmg->get_magic_dmg() * 100 / (100 + mMagic_res);
	if (mMagic_immune) magic_dmg = 0.0;

	auto fire_dmg = dmg->get_fire_dmg() * 100 / (100 + mFire_res);
	if (mFire_immune) fire_dmg = 0.0;

	auto water_dmg = dmg->get_water_dmg() * 100 / (100 + mWater_res);
	if (mWater_immune) water_dmg = 0.0;

	auto elec_dmg = dmg->get_elec_dmg() * 100 / (100 + mElec_res);
	if (mElec_immune) elec_dmg = 0.0;

	auto total_dmg = phys_dmg + magic_dmg + fire_dmg + water_dmg + elec_dmg;

	return total_dmg;
}
