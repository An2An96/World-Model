#include "world.h"

World::Entity::Entity(World* world, int x, int y)
{
	gWorld = world;
	pos[0] = x;	pos[1] = y;
	BirthActionCount = random(5, 20);
	ChanceBirth = random(50, 100);
}

World::Entity::Entity(World* world, int x, int y, int birtch_ac, int birtch_c)
{
	gWorld = world;
	pos[0] = x;	pos[1] = y;
	BirthActionCount = birtch_ac;
	ChanceBirth = birtch_c;
}

World::Entity::~Entity()
{
}
