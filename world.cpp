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
			gEntitys[Cell(x, y)] = new Predator();
			predatorsCount++;
			return true;
		}
		else if (type == ENTITY_VICTIMS)	{
			gEntitys[Cell(x, y)] = new Victim();
			victimsCount++;
			return true;
		}
	}
	return false;
}

bool World::PerformStep()
{
	int bufX, bufY, nX, nY, rand;
	E_STEP step = STEP_NONE;

	//	@Step Victims

	if (gCycle == ENTITY_VICTIMS)	{
		//	Ищем из списка существ, только жертв
		for (int y = 0; y < sizeY; y++)	{
			for (int x = 0; x < sizeX; x++)	{
				nX = x; nY = y;
				if (dynamic_cast<Victim*>(gEntitys[Cell(x,y)]))	{
					//	подбираем позицию для хода
					std::vector<int> pos;
					if (Calculate(x, y, pos))	{
						do
						{
							rand = random(0, pos.size() - 1);
							bufY = pos[rand] / sizeX; bufX = pos[rand] % sizeX;

							//	Если в клетке никого нет
							if (InCell(bufX, bufY) == ENTITY_NONE && InInterim(bufX, bufY) == ENTITY_NONE)	{
								step = (*gEntitys[Cell(x, y)]).Step();
								nX = bufX; nY = bufY;

								if (step == STEP_BIRTH)	{
									//	вычесляем позицию и рождаем на ней новое существо
									pos.clear();
									if (Calculate(nX, nY, pos))	{
										do
										{
											rand = random(0, pos.size() - 1);
											bufY = pos[rand] / sizeX; bufX = pos[rand] % sizeX;

											if (InCell(bufX, bufY) == ENTITY_NONE && InInterim(bufX, bufY) == ENTITY_NONE)	{
												CreateEntity(ENTITY_VICTIMS, nX, nY);
												break;
											} else {
												pos.erase(pos.begin() + rand);
											}
										} while (pos.size());
									}
								}
								break;
							} else	{
								pos.erase(pos.begin() + rand);
							}
						} while (pos.size());
					}
					gInterim[Cell(nX, nY)] = gEntitys[Cell(x, y)];
					gEntitys.erase(Cell(x, y));
				}
				else if (dynamic_cast<Predator*>(gEntitys[Cell(x, y)]))	{
					gInterim[Cell(nX, nY)] = gEntitys[Cell(x, y)];
					gEntitys.erase(Cell(x, y));
				}
			}
		}
		gCycle = ENTITY_PREDATORS;
	}

//	@Step Predators

	else if (gCycle == ENTITY_PREDATORS)	{

		//	Ищем из списка существ, только хищников
		for (int y = 0; y < sizeY; y++)	{
			for (int x = 0; x < sizeX; x++)	{
				nX = x; nY = y;
				if (dynamic_cast<Predator*>(gEntitys[Cell(x, y)]))	{
					//	подбираем позицию для хода
					std::vector<int> pos;
					if (Calculate(x, y, pos))	{
						for (int s = pos.size() - 1; s >= 0; s--)	{
							bufY = pos[s] / sizeX; bufX = pos[s] % sizeX;

							if (InCell(bufX, bufY) == ENTITY_VICTIMS || InInterim(bufX, bufY) == ENTITY_VICTIMS)	{
								step = (*gEntitys[Cell(x, y)]).Step(true);

								if (step == STEP_BIRTH || step == STEP_EAT)	{

									nX = bufX; nY = bufY;

									Victim* v = dynamic_cast<Victim*>(gEntitys[Cell(nX, nY)]);
									if ((v = dynamic_cast<Victim*>(gEntitys[Cell(nX, nY)])) == NULL)	{
										v = dynamic_cast<Victim*>(gInterim[Cell(nX, nY)]);
									}
									//	Проверка на отравление
									if (random(0, 100) <= (*v).GetPoisoning())	{
										step = STEP_DEATH;
									}

									delete gInterim[Cell(nX, nY)],	gEntitys[Cell(nX, nY)];
									gInterim.erase(Cell(nX, nY));	gEntitys.erase(Cell(nX, nY));
									victimsCount--;
									break;
								}
							}
						}
						if (nX == x && nY == y)	{
							do
							{
								rand = random(0, pos.size() - 1);
								bufY = pos[rand] / sizeX; bufX = pos[rand] % sizeX;

								//	Если в клетке никого нет
								if (InCell(bufX, bufY) == ENTITY_NONE && InInterim(bufX, bufY) == ENTITY_NONE)	{
									step = (*gEntitys[Cell(x, y)]).Step();
									nX = bufX; nY = bufY;
									break;
								}
								else	{
									pos.erase(pos.begin() + rand);
								}
							} while (pos.size());
						}
						if (step == STEP_BIRTH)	{
							//	вычесляем позицию и рождаем на ней новое существо
							pos.clear();
							if (Calculate(nX, nY, pos))	{
								do
								{
									rand = random(0, pos.size() - 1);
									bufY = pos[rand] / sizeX; bufX = pos[rand] % sizeX;

									if (InCell(bufX, bufY) == ENTITY_NONE && InInterim(bufX, bufY) == ENTITY_NONE)	{
										CreateEntity(ENTITY_PREDATORS, nX, nY);
										break;
									}
									else {
										pos.erase(pos.begin() + rand);
									}
								} while (pos.size());
							}
						}
						else if (step == STEP_DEATH)	{
							delete gEntitys[Cell(x, y)];
							gEntitys.erase(Cell(x, y));
							predatorsCount--;
							continue;
						}
					}
					gInterim[Cell(nX, nY)] = gEntitys[Cell(x, y)];
					gEntitys.erase(Cell(x, y));
				}
				else if (dynamic_cast<Victim*>(gEntitys[Cell(x, y)]))	{
					gInterim[Cell(nX, nY)] = gEntitys[Cell(x, y)];
					gEntitys.erase(Cell(x, y));
				}
			}
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

E_ENTITY World::InCell(int x, int y)
{
	if (gEntitys[Cell(x, y)]) {
		if (dynamic_cast<Victim*>(gEntitys[Cell(x, y)]))	{
			return ENTITY_VICTIMS;
		}
		else if (dynamic_cast<Predator*>(gEntitys[Cell(x, y)]))	{
			return ENTITY_PREDATORS;
		}
	}
	return ENTITY_NONE;
}

E_ENTITY World::InInterim(int x, int y)
{
	if (gInterim[Cell(x, y)])	{
		if (dynamic_cast<Victim*>(gInterim[Cell(x, y)]))	{
			return ENTITY_VICTIMS;
		}
		else if (dynamic_cast<Predator*>(gInterim[Cell(x, y)]))	{
			return ENTITY_PREDATORS;
		}
	}
	return ENTITY_NONE;
}

void World::View()
{
	for (int x = 0; x < sizeX + 2; x++)	printf("_");
	printf("\n");
	for (int y = 0; y < sizeY; y++)	{
		printf("|");
		for (int x = 0; x < sizeX; x++)	{
			if (gEntitys[Cell(x, y)])	{
				(*gEntitys[Cell(x, y)]).View();
			} else {
				printf(" ");
			}
		}
		printf("|\n");
	}
	for (int x = 0; x < sizeX + 2; x++)	printf("_");
	printf("\n");

	printf("p = %d, v = %d", predatorsCount, victimsCount);
}
