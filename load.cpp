#include "world.h"

World::World(const char* filename)
{
	std::ifstream in(filename);
	if (!in.is_open())
		throw "cannot open file";
	int Cycle;
	in >> sizeX >> sizeY >> Cycle;
	gCycle = (Cycle == 1) ? ENTITY_VICTIMS : ENTITY_PREDATORS;
	while (true)
	{
		int Cell, Type;
		in >> Cell >> Type;

		int x = Cell % sizeX,
			y = Cell / sizeX;

		if (World::CreateEntity((E_ENTITY)Type, x, y) == false)	{
			return;
		}
		gEntitys[Cell]->ReadData(in);
		if (in.fail())	{
			gEntitys.erase(Cell);
			break;
		}
	};
	in.close();
};

bool World::SaveWorld(const char* filename)
{
	std::ofstream out(filename);
	if (!out.is_open())	{
		return false;
	}
	out << sizeX << " " << sizeY << " " << gCycle << '\n';
	for (auto i = gEntitys.begin(); i != gEntitys.end(); ++i)
	{
		if (i->second != 0)
		{
			out << i->first << " ";
			E_ENTITY TYPE = dynamic_cast<Victim*>(i->second.get()) ? ENTITY_VICTIMS : ENTITY_PREDATORS;
			out << TYPE << " ";
			i->second->PrintData(out);
			out << '\n';
		};
	};
	out.close();
	return true;
};
