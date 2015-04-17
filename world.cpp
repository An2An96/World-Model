#include "world.h"

World::World(int x, int y)
{
	sizeX = x, sizeY = y;
	GenerateEntitys();
}

void World::GenerateEntitys()
{
	//	Заполняем поле рандомно существами от 10% до 30% от всего размера
	int min = (int)((sizeX * sizeY) * 0.1), max = (int)((sizeX * sizeY) * 0.3);
	int count = random(min, max);
	while (count)
	{
		//	50 на 50 генерируем жертв и хищников
		if(CreateEntity(random(0, 1) ? ENTITY_PREDATORS : ENTITY_VICTIMS, random(0, sizeX), random(0, sizeY)))	count--;
	}
}

bool World::CreateEntity(E_ENTITY type, int x, int y)
{
	if (x >= 0 && x < sizeX && y >= 0 && y < sizeY && InCell(x, y) == ENTITY_NONE)
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

bool World::PerformStep()
{
	//	@Step Victims
	if (gCycle == ENTITY_VICTIMS)	{

		for (auto i = gEntitys.begin(); i != gEntitys.end(); i = gEntitys.begin())
		{
			if (dynamic_cast<Victim*>(i->second.get()))	{
				int cell = i->second->Step();
				gInterim[cell] = std::move(gEntitys[i->first]);
			}
			else if (dynamic_cast<Predator*>(i->second.get()))	{
				gInterim[i->first] = std::move(gEntitys[i->first]);
			}
			gEntitys.erase(i);
		}
		gCycle = ENTITY_PREDATORS;
	}

//	@Step Predators
	else if (gCycle == ENTITY_PREDATORS)	{

		for (auto i = gEntitys.begin(); i != gEntitys.end(); i = gEntitys.begin())
		{
			if (dynamic_cast<Predator*>(i->second.get()))	{
				int cell = i->second->Step();
				if (cell != (-1))	{
					gInterim[cell] = std::move(gEntitys[i->first]);
				}
			}
			else if (dynamic_cast<Victim*>(i->second.get()))	{
				gInterim[i->first] = std::move(gEntitys[i->first]);
			}
			gEntitys.erase(i);
		}
		gCycle = ENTITY_VICTIMS; 
	}
	gInterim.swap(gEntitys);
	gInterim.clear();
	return (predatorsCount || victimsCount) ? true : false;
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
	if (gEntitys[Cell(x, y)]) {
		if (dynamic_cast<Victim*>(gEntitys[Cell(x, y)].get()))	{
			return ENTITY_VICTIMS;
		}
		else if (dynamic_cast<Predator*>(gEntitys[Cell(x, y)].get()))	{
			return ENTITY_PREDATORS;
		}
	}
	return ENTITY_NONE;
}

E_ENTITY World::InInterim(int x, int y)
{
	if (gInterim[Cell(x, y)])	{
		if (dynamic_cast<Victim*>(gInterim[Cell(x, y)].get()))	{
			return ENTITY_VICTIMS;
		}
		else if (dynamic_cast<Predator*>(gInterim[Cell(x, y)].get()))	{
			return ENTITY_PREDATORS;
		}
	}
	return ENTITY_NONE;
}

void World::View()
{
	for (int x = 0; x < sizeX + 2; x++)	std::cout << "_";
	std::cout << std::endl;
	for (int y = 0; y < sizeY; y++)	{
		std::cout << "|";
		for (int x = 0; x < sizeX; x++)	{
			if (gEntitys[Cell(x, y)])	{
				gEntitys[Cell(x, y)]->View();
			} else {
				std::cout << " ";
			}
		}
		std::cout << "|" << std::endl;
	}
	for (int x = 0; x < sizeX + 2; x++)	std::cout << "_";

	std::cout << std::endl << "Predators: " << predatorsCount << ", Victims: " << victimsCount << std::endl;
}
