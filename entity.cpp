#include "world.h"

World::Entity::Entity()
{
	BirthActionCount = random(5, 20);
	ChanceBirth = random(50, 100);
}

World::Entity::Entity(int birtch_ac, int birtch_c)
{
	BirthActionCount = birtch_ac;
	ChanceBirth = birtch_c;
}

World::Entity::~Entity()
{
}
