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

E_ENTITY World::Victim::Type()
{
    return ENTITY_VICTIMS;
}

int World::Victim::Step()
{
    int bufX, bufY, rand;
    std::vector<int> pos;
    //	подбираем позицию для хода
    if (gWorld->Calculate(position.x, position.y, pos))	{
        do
        {
            rand = random(0, (int)pos.size() - 1);
            gWorld->Size(pos[rand], bufX, bufY);

            //	Если в клетке никого нет
            if (gWorld->_InCell(gWorld->gEntitys, bufX, bufY) == ENTITY_NONE && gWorld->_InCell(gWorld->gInterim, bufX, bufY) == ENTITY_NONE)	{

                position.Set(bufX, bufY);

                ActionForBirth++;
                break;
            }
            else	{
                pos.erase(pos.begin() + rand);
            }
        } while (pos.size());
    }
    return gWorld->Cell(position.x, position.y);
}

void World::Victim::Birth()
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

void World::Victim::PrintData(std::ofstream& out)
{
    out << ActionForBirth << " " << BirthActionCount << " " << ChanceBirth << " " << ChancePoisoning;
}

void World::Victim::ReadData(std::ifstream& in)
{
    in >> ActionForBirth >> BirthActionCount >> ChanceBirth >> ChancePoisoning;
}
