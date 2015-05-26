#include "world.h"

World::World(int x, int y, int PredPerCent, int VictPerCent)
{
    sizeX = x, sizeY = y;
    GenerateEntitys(PredPerCent, VictPerCent);
}

void World::GenerateEntitys(int PredPerCent, int VictPerCent)
{
    //	Заполняем поле рандомно существами <PredCount> % и <VictCount> % от всего размера
    int PredCount = (int)((sizeX * sizeY) * ((double)PredPerCent / 100)),
        VictCount = (int)((sizeX * sizeY) * ((double)VictPerCent / 100));

    while (PredCount)
    {
        if(CreateEntity(ENTITY_PREDATORS, random(0, sizeX - 1), random(0, sizeY - 1)))
            PredCount--;
    }
    while (VictCount)
    {
        if(CreateEntity(ENTITY_VICTIMS, random(0, sizeX - 1), random(0, sizeY - 1)))
            VictCount--;
    }
}

bool World::CreateEntity(E_ENTITY type, int x, int y)
{
    if (x >= 0 && x < sizeX && y >= 0 && y < sizeY && _InCell(gEntitys, x, y) == ENTITY_NONE)
    {
        if (type == ENTITY_PREDATORS)	{
            gEntitys[Cell(x, y)] = std::make_unique<World::Predator>(this, x, y);
            return true;
        }
        else if (type == ENTITY_VICTIMS)	{
            gEntitys[Cell(x, y)] = std::make_unique<World::Victim>(this, x, y);
            return true;
        }
    }
    return false;
}

E_STEP World::PerformStep()
{
    for (auto i = gEntitys.begin(); i != gEntitys.end(); i = gEntitys.begin())
    {
        if (EntityStep(i))	{
            gEntitys.erase(i);
        }
    }
    gInterim.swap(gEntitys);
    gInterim.clear();

    gCycle = (gCycle == ENTITY_PREDATORS) ? ENTITY_VICTIMS : ENTITY_PREDATORS;
    if(predatorsCount == 0 && victimsCount == (sizeX * sizeY))
    {
        return STEP_VICTIMS_WIN;
    }
    else if(predatorsCount == 0 && victimsCount == 0)
    {
        return STEP_ALL_DEATH;
    }
    else
    {
        return STEP_LIVE;
    }
}

bool World::EntityStep(std::map<int, std::unique_ptr<World::Entity>>::iterator cell)
{
    if (cell->second.get())	{
        if (gCycle == cell->second->Type())
        {
            int nCell = cell->second->Step();
            if (nCell != (-1))	{
                gInterim[nCell] = std::move(gEntitys[cell->first]);
                gEntitys.erase(cell);
                gInterim[nCell].get()->Birth();
                return false;
            }
        }
        else {
            gInterim[cell->first] = std::move(gEntitys[cell->first]);
        }
    }
    return true;
}

bool World::Calculate(int curx, int cury, std::vector<int>& pos)
{
    for (int i = cury - 1; i <= cury + 1; i++)	{
        for (int j = curx - 1; j <= curx + 1; j++)	{
            if (i < 0 || j < 0 || j >= sizeX || i >= sizeY || (j == curx && i == cury))	{
                continue;
            }
            pos.push_back(Cell(j, i));
        }
    }
    return pos.size() ? true : false;
}

void World::Size(int size, int& sx, int& sy)
{
    sy = size / sizeX; sx = size % sizeX;
}

int World::Cell(int x, int y)
{
    return (y * sizeX + x);
}

E_ENTITY World::InCell(int x, int y)
{
    return _InCell(gEntitys, x, y);
}

E_ENTITY World::_InCell(std::map<int, std::unique_ptr<World::Entity>>& object, int x, int y)
{
    if (object[Cell(x, y)]) {
        if (dynamic_cast<Victim*>(object[Cell(x, y)].get()))	{
            return ENTITY_VICTIMS;
        }
        else if (dynamic_cast<Predator*>(object[Cell(x, y)].get()))	{
            return ENTITY_PREDATORS;
        }
    }
    return ENTITY_NONE;
}

Coord World::GetWorldSize()
{
    Coord ws;
    ws.Set(sizeX, sizeY);
    return ws;
}
