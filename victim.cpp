#include "world.h"

World::Victim::Victim(World* world, int x, int y, int birtch_ac, int birtch_c, int poisoning_c) :
	Entity(world, x, y, birtch_ac, birtch_c)
{
	ChancePoisoning = poisoning_c;

	gWorld->victimsCount++;
}

World::Victim::Victim(World* world, int x, int y) :
	Entity(world, x, y)
{
	ChancePoisoning = random(0, 60);

	gWorld->victimsCount++;
}

World::Victim::~Victim()
{
	gWorld->victimsCount--;
}

int World::Victim::Step()
{
    int bufX, bufY, rand;
    std::vector<int> pos;
    //	подбираем позицию для хода
    if (gWorld->Calculate(this->pos[0], this->pos[1], pos))	{
        do
        {
            rand = random(0, (int)pos.size() - 1);
            gWorld->Size(pos[rand], bufX, bufY);

            //	Если в клетке никого нет
            if (gWorld->InCell(bufX, bufY) == ENTITY_NONE && gWorld->InInterim(bufX, bufY) == ENTITY_NONE)	{

                this->pos[0] = bufX; this->pos[1] = bufY;

                ActionForBirth++;
                /*if (++ActionForBirth >= BirthActionCount && random(0, 100) <= ChanceBirth)	{
                    //	рождаем новую жертву
                    ActionForBirth = 0;

                    pos.clear();
                    if (gWorld->Calculate(this->pos[0], this->pos[1], pos))	{
                        do
                        {
                            rand = random(0, pos.size() - 1);
                            gWorld->Size(pos[rand], bufX, bufY);

                            if (gWorld->InCell(bufX, bufY) == ENTITY_NONE && gWorld->InInterim(bufX, bufY) == ENTITY_NONE)	{
                                gWorld->CreateEntity(ENTITY_VICTIMS, bufX, bufY);
                                break;
                            }
                            else {
                                pos.erase(pos.begin() + rand);
                            }
                        } while (pos.size());
                    }
                }  */
                break;
            }
            else	{
                pos.erase(pos.begin() + rand);
            }
        } while (pos.size());
    }
    return gWorld->Cell(this->pos[0], this->pos[1]);
}

void World::Victim::Birth()
{
    if (ActionForBirth >= BirthActionCount && random(0, 100) <= ChanceBirth)	{
        int bufX, bufY, rand;
        std::vector<int> pos;

        //	рождаем новую жертву
        ActionForBirth = 0;

        pos.clear();
        if (gWorld->Calculate(this->pos[0], this->pos[1], pos))	{
            do
            {
                rand = random(0, (int)pos.size() - 1);
                gWorld->Size(pos[rand], bufX, bufY);

                if (gWorld->InCell(bufX, bufY) == ENTITY_NONE && gWorld->InInterim(bufX, bufY) == ENTITY_NONE)	{
                    gWorld->CreateEntity(ENTITY_VICTIMS, bufX, bufY);
                    break;
                }
                else {
                    pos.erase(pos.begin() + rand);
                }
            } while (pos.size());
        }
    }
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
