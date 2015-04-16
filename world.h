#pragma once

#include <map>
#include <vector>
#include "useful.h"
#include <fstream>

#define Cell(x,y)	(y * sizeX + x)

enum E_ENTITY	{
	ENTITY_NONE,
	ENTITY_VICTIMS,
	ENTITY_PREDATORS,
};

enum E_STEP		{
	STEP_NONE,
	STEP_SKIP,
	STEP_BIRTH,
	STEP_DEATH,
	STEP_EAT,
};

class World
{
public:

//	@Entity
	class Entity
	{

	public:

		Entity();
		Entity(int, int);
		virtual ~Entity();

		virtual E_STEP Step(bool = false) = 0;
		virtual void View() = 0;
		virtual void PrintData(std::ofstream& out) = 0;
		virtual void ReadData(std::ifstream& in) = 0;

	protected:

		//	data
		int ActionForBirth = 0;		//	кол-во совершенных действий для рождения

		//	features
		int BirthActionCount = 0;	//	кол-во требуемых действий для рождения
		int ChanceBirth = 0;		//	вероятность успешного рождения
	};

//	@Predator
	class Predator :
		public World::Entity
	{

	public:

		Predator();
		Predator(int, int, int, int);
		~Predator();

		virtual E_STEP Step(bool);
		virtual void View();
		virtual void PrintData(std::ofstream& out);
		virtual void ReadData(std::ifstream& in);

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

		Victim();
		Victim(int birtch_ac, int birtch_c, int poisoning_c);

		~Victim();

		virtual E_STEP Step(bool);
		virtual void View();
		virtual void PrintData(std::ofstream& out);
		virtual void ReadData(std::ifstream& in);

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

	E_ENTITY InCell(int, int);
	E_ENTITY InInterim(int, int);
	void View();

protected:
	int predatorsCount = 0,						//	кол-во хищников на поле
		victimsCount = 0;						//	кол-во жертв на поле
private:

	//	data
	int sizeX, sizeY;						//	размеры поля

	std::map<int, World::Entity*> 
		gEntitys,							//	указатели на всех сущностей мира (хищников/жертв)
		gInterim;							//	промежуточная 'карта' сущностей (пока выполняется очередной ход)
	E_ENTITY gCycle = ENTITY_PREDATORS;		//	показывает кто в данный момент делает шаг
};
