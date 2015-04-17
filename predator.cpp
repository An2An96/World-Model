#include "world.h"

World::Predator::Predator(World* world, int x, int y, int birtch_ac, int birtch_c, int eat_c, int death_s) :
	Entity(world, x, y, birtch_ac, birtch_c)
{
	ChanceEatVictim = eat_c;
	StepToDeath = death_s;

	gWorld->predatorsCount++;
}

World::Predator::Predator(World* world, int x, int y) :
	Entity(world, x, y)
{
	ChanceEatVictim = random(50, 100);
	StepToDeath = random(10, 30);

	gWorld->predatorsCount++;
}

World::Predator::~Predator()
{
	gWorld->predatorsCount--;
}

int World::Predator::Step(bool eat = false)
{
	int bufX, bufY, rand;
	bool chPos = false;
	std::vector<int> pos;
	if (gWorld->Calculate(this->pos[0], this->pos[1], pos))	{
		for (int s = pos.size() - 1; s >= 0; s--)	{
			rand = random(0, pos.size() - 1);
			gWorld->Size(pos[rand], bufX, bufY);

			if (gWorld->InCell(bufX, bufY) == ENTITY_VICTIMS || gWorld->InInterim(bufX, bufY) == ENTITY_VICTIMS)	{
				
				if (random(0, 100) <= ChanceEatVictim)	{

					chPos = true;

					this->pos[0] = bufX; this->pos[1] = bufY;

					StepWithoutFood = 0;
					ActionForBirth++;

					Victim* v;
					if ((v = dynamic_cast<Victim*>(gWorld->gEntitys[gWorld->Cell(bufX, bufY)].get())) == NULL)	{
						v = dynamic_cast<Victim*>(gWorld->gInterim[gWorld->Cell(bufX, bufY)].get());
					}

					gWorld->gInterim.erase(gWorld->Cell(bufX, bufY));	gWorld->gEntitys.erase(gWorld->Cell(bufX, bufY));

					//	Проверка на отравление
					if (random(0, 100) <= v->GetPoisoning())	{

						return -1;
					}
					break;
				}
			}
		}
		if (chPos == false)	{
			do
			{
				rand = random(0, pos.size() - 1);
				gWorld->Size(pos[rand], bufX, bufY);

				//	Если в клетке никого нет
				if (gWorld->InCell(bufX, bufY) == ENTITY_NONE && gWorld->InInterim(bufX, bufY) == ENTITY_NONE)	{

					this->pos[0] = bufX; this->pos[1] = bufY;

					if (++StepWithoutFood >= StepToDeath)	{
						//	убиваем хищника
						return -1;
					}
					break;
				}
				else	{
					pos.erase(pos.begin() + rand);
				}
			} while (pos.size());
		}
		if (ActionForBirth >= BirthActionCount && random(0, 100) <= ChanceBirth)	{
			//	рождаем новую жертву
			ActionForBirth = 0;

			//	вычесляем позицию и рождаем на ней новое существо
			pos.clear();
			if (gWorld->Calculate(this->pos[0], this->pos[1], pos))	{
				do
				{
					rand = random(0, pos.size() - 1);
					gWorld->Size(pos[rand], bufX, bufY);

					if (gWorld->InCell(bufX, bufY) == ENTITY_NONE && gWorld->InInterim(bufX, bufY) == ENTITY_NONE)	{
						gWorld->CreateEntity(ENTITY_PREDATORS, bufX, bufY);
						break;
					}
					else {
						pos.erase(pos.begin() + rand);
					}
				} while (pos.size());
			}
		}
	}
	return gWorld->Cell(this->pos[0], this->pos[1]);
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
