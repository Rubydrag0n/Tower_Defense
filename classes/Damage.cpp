#include "Damage.h"

Damage::Damage()
{
	mPhys_dmg = 0.0;
	mMagic_dmg = 0.0;
	mFire_dmg = 0.0;
	mWater_dmg = 0.0;
	mElec_dmg = 0.0;
}

Damage::Damage(double phys, double magic, double fire, double water, double elec)
{
	mPhys_dmg = phys;
	mMagic_dmg = magic;
	mFire_dmg = fire;
	mWater_dmg = water;
	mElec_dmg = elec;
}

void Damage::add(double phys, double magic, double fire, double water, double elec)
{
	mPhys_dmg += phys;
	mMagic_dmg += magic;
	mFire_dmg += fire;
	mWater_dmg += water;
	mElec_dmg += elec;
}


void Damage::set_damages(double phys, double magic, double fire, double water, double elec)
{
	mPhys_dmg = phys;
	mMagic_dmg = magic;
	mFire_dmg = fire;
	mWater_dmg = water;
	mElec_dmg = elec;
}

void Damage::set_phys_dmg(double phys)
{
	mPhys_dmg = phys;
}

double Damage::get_phys_dmg() const
{
	return mPhys_dmg;
}

void Damage::set_magic_dmg(double magic)
{
	mMagic_dmg = magic;
}

double Damage::get_magic_dmg() const
{
	return mMagic_dmg;
}

void Damage::set_fire_dmg(double fire)
{
	mFire_dmg = fire;
}

double Damage::get_fire_dmg() const
{
	return mFire_dmg;
}

void Damage::set_water_dmg(double water)
{
	mWater_dmg = water;
}

double Damage::get_water_dmg() const
{
	return mWater_dmg;
}

void Damage::set_elec_dmg(double elec)
{
	mElec_dmg = elec;
}

double Damage::get_elec_dmg() const
{
	return mElec_dmg;
}

double Damage::get_dmg_sum() const
{
	return mPhys_dmg + mMagic_dmg + mFire_dmg + mWater_dmg + mElec_dmg;
}
