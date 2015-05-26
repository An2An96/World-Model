#ifndef H_WORLD
#define H_WORLD

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

enum E_STEP {
    STEP_LIVE,
    STEP_VICTIMS_WIN,
    STEP_ALL_DEATH
};

struct Coord
{
    int x, y;
    void Set(int x, int y)	{ this->x = x, this->y = y; }
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

        virtual E_ENTITY Type() = 0;
        virtual int Step() = 0;
        virtual void Birth() = 0;
        virtual void PrintData(std::ofstream& out) = 0;
        virtual void ReadData(std::ifstream& in) = 0;

    protected:
        //	data
        World* gWorld = 0;			//	ссылка на мир
        Coord position;				//	позиция
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
        Predator(World*, int, int);
        Predator(World*, int, int, int, int, int, int);
        ~Predator();

        virtual E_ENTITY Type();
        virtual int Step();
        virtual void Birth();
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

        virtual E_ENTITY Type();
        virtual int Step();
        virtual void Birth();
        virtual void PrintData(std::ofstream&);
        virtual void ReadData(std::ifstream&);

        int GetPoisoning();

    private:
        int ChancePoisoning = 0;	//	шанс того, что хищник съевший жертву отравится
    };

//	@World
    World(int x, int y, int PredPerCent, int VictPerCent);
    World(const char* filename);			// Загрузка

    bool SaveWorld(const char*);			// Сохранение
    void GenerateEntitys(int PredPerCent, int VictPerCent);
    bool CreateEntity(E_ENTITY, int, int);

    E_STEP PerformStep();
    bool Calculate(int, int, std::vector<int>&);
    int Cell(int, int);
    void Size(int, int&, int&);
    Coord GetWorldSize();

    E_ENTITY InCell(int, int);

protected:
    E_ENTITY _InCell(std::map<int, std::unique_ptr<World::Entity>>&, int, int);

    int predatorsCount = 0,						//	кол-во хищников на поле
        victimsCount = 0;						//	кол-во жертв на поле
private:

    bool EntityStep(std::map<int, std::unique_ptr<World::Entity>>::iterator cell);

    //	data
    int sizeX, sizeY;						//	размеры поля
    std::map<int, std::unique_ptr<World::Entity>>
        gEntitys,							//	указатели на всех сущностей мира (хищников/жертв)
        gInterim;							//	промежуточная 'карта' сущностей (пока выполняется очередной ход)
    E_ENTITY gCycle = ENTITY_VICTIMS;		//	показывает кто в данный момент делает шаг
};

#endif // H_WORLD
