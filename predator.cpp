#include "world.h"

World::Predator::Predator(int birtch_ac, int birtch_c, int eat_c, int death_s) :
	Entity(birtch_ac, birtch_c) 
{
	ChanceEatVictim = eat_c;
	StepToDeath = death_s;
}

World::Predator::Predator() :
	Entity()
{
	ChanceEatVictim = random(50, 100);
	StepToDeath = random(10, 30);
}

World::Predator::~Predator()
{
}

E_STEP World::Predator::Step(bool eat = false)
{
	if (eat)	{
		StepWithoutFood = 0;
		if (++ActionForBirth >= BirthActionCount)	{
			if (random(0, 100) <= ChanceBirth)	{
				//	рождаем новую жертву
				ActionForBirth = 0;
				return STEP_BIRTH;
			}
		}
		return STEP_EAT;
	}
	else if (++StepWithoutFood >= StepToDeath)	{
		//	убиваем хищника
		return STEP_DEATH;
	}
	return STEP_NONE;
}

void World::Predator::View()
{
	printf("X");
}

void World::Predator::PrintData(std::ofstream& out)
{
	out << ActionForBirth << " " << BirthActionCount << " " << ChanceBirth
		<< " " << StepWithoutFood << " " << ChanceEatVictim << " " << StepToDeath;
};

void World::Predator::ReadData(std::ifstream& in)
{
	in >> ActionForBirth >> BirthActionCount >> ChanceBirth
		>> StepWithoutFood >> ChanceEatVictim >> StepToDeath;
};
