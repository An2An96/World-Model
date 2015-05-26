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

E_ENTITY World::Predator::Type()
{
    return ENTITY_PREDATORS;
}

int World::Predator::Step()
{
    int bufX, bufY, rand;
    bool chPos = false;
    std::vector<int> pos;
    StepWithoutFood++;
    if (gWorld->Calculate(position.x, position.y, pos))	{
        for (int s = (int)pos.size() - 1; s >= 0; s--)	{
            rand = random(0, (int)pos.size() - 1);
            gWorld->Size(pos[rand], bufX, bufY);

            if (gWorld->_InCell(gWorld->gEntitys, bufX, bufY) == ENTITY_VICTIMS || gWorld->_InCell(gWorld->gInterim, bufX, bufY) == ENTITY_VICTIMS)	{

                if (random(0, 100) <= ChanceEatVictim)	{

                    position.Set(bufX, bufY);
                    chPos = true;

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
                rand = random(0, (int)pos.size() - 1);
                gWorld->Size(pos[rand], bufX, bufY);

                //	Если в клетке никого нет
                if (gWorld->_InCell(gWorld->gEntitys, bufX, bufY) == ENTITY_NONE && gWorld->_InCell(gWorld->gInterim, bufX, bufY) == ENTITY_NONE)	{
                    if (StepWithoutFood >= StepToDeath)	{
                        return -1;  //	убиваем хищника
                    }
                    position.Set(bufX, bufY);
                    chPos = true;
                    break;
                }
                else	{
                    pos.erase(pos.begin() + rand);
                }
            } while (pos.size());
        }
    }
    if(chPos == false)  {
        if (StepWithoutFood >= StepToDeath)	{
            return -1;  //	убиваем хищника
        }
    }
    return gWorld->Cell(position.x, position.y);
}

void World::Predator::Birth()
{
    if (ActionForBirth >= BirthActionCount && random(0, 100) <= ChanceBirth)	{
        int bufX, bufY, rand;
        std::vector<int> pos;
        //	рождаем новую жертву
        ActionForBirth = 0;

        pos.clear();
        if (gWorld->Calculate(position.x, position.y, pos))	{
            do
            {
                rand = random(0, (int)pos.size() - 1);
                gWorld->Size(pos[rand], bufX, bufY);

                if (gWorld->_InCell(gWorld->gEntitys, bufX, bufY) == ENTITY_NONE && gWorld->_InCell(gWorld->gInterim, bufX, bufY) == ENTITY_NONE)	{
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

void World::Predator::PrintData(std::ofstream& out)
{
    out << ActionForBirth << " " << BirthActionCount << " " << ChanceBirth
        << " " << StepWithoutFood << " " << ChanceEatVictim << " " << StepToDeath;
}

void World::Predator::ReadData(std::ifstream& in)
{
    in >> ActionForBirth >> BirthActionCount >> ChanceBirth
        >> StepWithoutFood >> ChanceEatVictim >> StepToDeath;
}
