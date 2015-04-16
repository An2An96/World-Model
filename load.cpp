#include "world.h"

World::World(const char* filename)
{
	std::ifstream in(filename);
	if (!in.is_open())
		throw "cannot open file";
	int Cycle;
	in >> sizeX >> sizeY >> Cycle;
	gCycle = Cycle ? ENTITY_PREDATORS : ENTITY_VICTIMS;
	while (true)
	{
		int Cell, Type;
		Entity* E;
		in >> Cell >> Type;
		if (Type)
			E = new Predator();
		else
			E = new Victim();
		E->ReadData(in);
		if (in.fail())
		{
			delete E;
			break;
		};
		gEntitys[Cell] = E;
	};
	in.close();
};

bool World::SaveWorld(const char* filename)
{
	std::ofstream out(filename);
	if (!out.is_open())
	{
		std::cout << "error";
		return 0;
	};
	out << sizeX << " " << sizeY << " " << gCycle << '\n';
	std::map<int, World::Entity*>::iterator it;
	for (it = gEntitys.begin(), it++; it != gEntitys.end(); it++)
	{
		if (it->second != 0)
		{
			out << it->first << " ";
			E_ENTITY TYPE = dynamic_cast<Victim*>(it->second) ? ENTITY_VICTIMS : ENTITY_PREDATORS;
			out << TYPE << " ";
			it->second->PrintData(out);
			out << '\n';
		};
	};
	out.close();
	return 1;
};
