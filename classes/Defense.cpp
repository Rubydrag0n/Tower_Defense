#include "Defense.h"

Defense::Defense() : mDefenses{0}, mDisplay { 0.0 }
{
	mFull_health = 0.0;
	mHealth = 0.0;
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

	set_display_zero();
}

Defense::Defense(const double health, const double armor, const double magic_res, const double fire_res,
                 const double water_res, const double elec_res) : mDefenses{ 0 }, mDisplay{ 0.0 }
{
	mFull_health = health;
	mHealth = health;
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

	mDefenses[HEALTH] = health;
	mDefenses[ARMOR] = armor;
	mDefenses[MAGIC_RES] = magic_res;
	mDefenses[FIRE_RES] = fire_res;
	mDefenses[WATER_RES] = water_res;
	mDefenses[ELEC_RES] = elec_res;

	set_display_zero();
}

void Defense::set_defenses(const double health, const double armor, const double magic_res, const double fire_res,
                           const double water_res, const double elec_res)
{
	mFull_health = health;
	mHealth = health;
	mArmor = armor;
	mMagic_res = magic_res;
	mFire_res = fire_res;
	mWater_res = water_res;
	mElec_res = elec_res;
	mDefenses[HEALTH] = health;
	mDefenses[ARMOR] = armor;
	mDefenses[MAGIC_RES] = magic_res;
	mDefenses[FIRE_RES] = fire_res;
	mDefenses[WATER_RES] = water_res;
	mDefenses[ELEC_RES] = elec_res;
}

void Defense::set_defenses(DEFENSETYPES type, int value)
{
	mDefenses[type] = value;
}

int Defense::get_defense(DEFENSETYPES type)
{
	return mDefenses[type];
}

void Defense::set_immunities(const bool phys, const bool magic, const bool fire, const bool water, const bool elec)
{
	mPhys_immune = phys;
	mMagic_immune = magic;
	mFire_immune = fire;
	mWater_immune = water;
	mElec_immune = elec;
}

void Defense::set_health(const double health)
{
	mHealth = health;
}

double Defense::get_health() const
{
	return mHealth;
}

void Defense::set_full_health(const double health)
{
	mFull_health = health;
}

double Defense::get_full_health() const
{
	return mFull_health;
}

void Defense::set_armor(const double armor)
{
	mArmor = armor;
}

double Defense::get_armor() const
{
	return mArmor;
}

void Defense::set_magic_res(const double magic_res)
{
	mMagic_res = magic_res;
}

double Defense::get_magic_res() const
{
	return mMagic_res;
}

void Defense::set_fire_res(const double fire_res)
{
	mFire_res = fire_res;
}

double Defense::get_fire_res() const
{
	return mFire_res;
}

void Defense::set_water_res(const double water_res)
{
	mWater_res = water_res;
}

double Defense::get_water_res() const
{
	return mWater_res;
}

void Defense::set_elec_res(const double elec_res)
{
	mElec_res = elec_res;
}

double Defense::get_elec_res() const
{
	return mElec_res;
}

bool Defense::take_damage(Damage *dmg)
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

	const auto total_dmg = phys_dmg + magic_dmg + fire_dmg + water_dmg + elec_dmg;

	auto killed = false;

	if (total_dmg >= mHealth)
	{
		mHealth = 0.0;
		killed = true;
	}
	else
	{
		mHealth -= total_dmg;
	}
	return killed;
}

std::string Defense::get_name(DEFENSETYPES type)
{
	switch (type)
	{
	case HEALTH:
		return "Health";
	case ARMOR:
		return "Armor";
	case MAGIC_RES:
		return "Magic Resist";
	case FIRE_RES:
		return "Fire Resist";
	case WATER_RES:
		return "Water Resist";
	case ELEC_RES:
		return "Elec Resist";
	default:
		return "Unknown Resource";
	}
}

Defense Defense::get_display_defenses()
{
	Defense def;

	for(auto i = 0; i < DEFENSETYPES_TOTAL; i++)
	{
		mDisplay[i] += float(mDefenses[i] - mDisplay[i]) / 100.f;
		if (mDefenses[i] - mDisplay[i] < 1.f) mDisplay[i] = mDefenses[i];
		def.set_defenses(DEFENSETYPES(i), int(mDisplay[i]));
	}
	return def;
}

void Defense::set_display_zero()
{
	for (auto i = 0; i < DEFENSETYPES_TOTAL; i++)
	{
		mDisplay[DEFENSETYPES(i)] = 0;
	}
}
