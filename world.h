#pragma once

#include <memory>
#include <map>
#include <vector>
#include "useful.h"
#include <fstream>

enum E_ENTITY	{
	ENTITY_NONE,
	ENTITY_VICTIMS,
	ENTITY_PREDATORS,
};

class World
{
public:

//	@Entity
	class Entity
	{

	public:

		Entity(World*, int, int);
		Entity(World*, int, int, int, int);
		virtual ~Entity();

		virtual int Step(bool = false) = 0;
		virtual void View() = 0;
		virtual void PrintData(std::ofstream& out) = 0;
		virtual void ReadData(std::ifstream& in) = 0;

	protected:

		//	data
		World* gWorld = 0;			//	ссылка на мир
		int ActionForBirth = 0;		//	кол-во совершенных действий для рождения

		int pos[2];					//	позиция

		//	features
		int BirthActionCount = 0;	//	кол-во требуемых действий для рождения
		int ChanceBirth = 0;		//	вероятность успешного рождения
	};

//	@Predator
	class Predator :
		public World::Entity
	{

	public:

		Predator(World*, int, int);
		Predator(World*, int, int, int, int, int, int);
		~Predator();

		virtual int Step(bool);
		virtual void View();
		virtual void PrintData(std::ofstream&);
		virtual void ReadData(std::ifstream&);

	private:

		int StepWithoutFood = 0;	//	Кол-во совершенных ходов без еды

		int ChanceEatVictim = 0;	//	Вероятность успешного съедения жертвы
		int StepToDeath = 0;		//	Кол - во ходов без еды для смерти
	};

//	@Victim
	class Victim :
		public World::Entity
	{

	public:

		Victim(World*, int, int);
		Victim(World*, int, int, int, int, int);

		~Victim();

		virtual int Step(bool);
		virtual void View();
		virtual void PrintData(std::ofstream&);
		virtual void ReadData(std::ifstream&);

		int GetPoisoning();

	private:

		int ChancePoisoning = 0;	//	шанс того, что хищник съевший жертву отравится
	};

//	@World
	World(int x, int y);
	World(const char* filename);			// Загрузка

	bool SaveWorld(const char* filename);	// Сохранение
	void GenerateEntitys();
	bool CreateEntity(E_ENTITY type, int x, int y);

	bool PerformStep();
	bool Calculate(int, int, std::vector<int>&);
	int Cell(int, int);
	void Size(int, int&, int&);

	E_ENTITY InCell(int, int);
	E_ENTITY InInterim(int, int);
	void View();

protected:
	int predatorsCount = 0,						//	кол-во хищников на поле
		victimsCount = 0;						//	кол-во жертв на поле
private:

	//	data
	int sizeX, sizeY;						//	размеры поля
	std::map<int, std::unique_ptr<World::Entity>>
		gEntitys,							//	указатели на всех сущностей мира (хищников/жертв)
		gInterim;							//	промежуточная 'карта' сущностей (пока выполняется очередной ход)
	E_ENTITY gCycle = ENTITY_VICTIMS;		//	показывает кто в данный момент делает шаг
};
