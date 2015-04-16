#include "world.h"

World::Victim::Victim(int birtch_ac, int birtch_c, int poisoning_c) :
	Entity(birtch_ac, birtch_c)
{
	ChancePoisoning = poisoning_c;
}

World::Victim::Victim() :
	Entity()
{
	ChancePoisoning = random(0, 60);
}

World::Victim::~Victim()
{
}

E_STEP World::Victim::Step(bool eat = false)
{
	if (++ActionForBirth >= BirthActionCount)	{
		if (random(0, 100) <= ChanceBirth)	{
			//	рождаем новую жертву 
			ActionForBirth = 0;
			return STEP_BIRTH;
		}
	}
	return STEP_NONE;
}

int World::Victim::GetPoisoning()	{
	return ChancePoisoning;
}

void World::Victim::View()
{
	printf("O");
}

void World::Victim::PrintData(std::ofstream& out)
{
	out << ActionForBirth << " " << BirthActionCount << " " << ChanceBirth << " " << ChancePoisoning;
};

void World::Victim::ReadData(std::ifstream& in)
{
	in >> ActionForBirth >> BirthActionCount >> ChanceBirth >> ChancePoisoning;
};
