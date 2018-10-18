#pragma once

enum BUILDINGTYPE {
	TOWER,
	INDUSTRIAL_BUILDING,
	LOGISTICS_BUILDING,
	STREET,
	BUILDINGTYPES_TOTAL
};

enum TILETYPES {
	EMPTY,
	BUILDINGTILE,
	STREETTILE,
	MONSTERPATH,
	WOODTILE,
	IRONTILE,
	TILETYPES_TOTAL
};

enum ACTIVITY {
	GETTING_IDLE,
	GETTING,
	DELIVERING_IDLE,
	DELIVERING,
	ACTIVITIES_TOTAL,
};

enum BUILDINGDIRECTION {
	NORTH,
	EAST,
	SOUTH,
	WEST,
	BUILDINGDIRECTIONS_TOTAL
};

enum RESOURCETYPES {
	GOLD,
	WOOD,
	STONE,
	IRON,
	ENERGY,
	WATER,
	FOOD,
	RESOURCES_TOTAL
};

//The numbers are the rowpositions of the rotations in the sprite image (outdated)
enum DIRECTION {
	DOWN = 4,
	LEFT = 5,
	UP = 6,
	RIGHT = 7
}; 

enum LClickableState {
	MOUSE_OUT = 0,			//while the mouse is not over the clickable
	MOUSE_OVER = 1,			//while the mouse hovers over the clickable
	MOUSE_DOWN_LEFT = 2,	//while left mouse button is pressed
	MOUSE_DOWN_RIGHT = 3,	//while right mouse button is pressed
	MOUSE_DOWN_MIDDLE = 4,	//while middle mouse button is pressed
	MOUSE_UP = 5,			//one tick after a mouseclick
	STATES_TOTAL = 6
};

enum ENTITYTYPE {
	UNDEFINED,
	BUILDING,
	ENEMY,
	SHOT,
	PARTICLE,
	ENTITYTYPES_TOTAL
};

enum LAYERS {
	BACKGROUND,
	BUILDINGS,
	ENEMIES,
	SHOTS,
	PARTICLES,
	WINDOWS,
	OVERLAY,
	LAYERS_TOTAL
};

enum STYLE {
	STANDARD,
	STYLES_TOTAL
};
