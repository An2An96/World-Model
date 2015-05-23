#include "view.h"
#include "world.h"

Sector::Sector(QWidget* parent, int size, QColor color) : QWidget(parent)
{
    setFixedSize(20, 20);
    setPalette(QPalette(color));
    setAutoFillBackground(true);
}

Field::Field(QWidget *parent, int ASizeX, int ASizeY) : QWidget(parent)
{
    SizeX = ASizeX; SizeY = ASizeY;
    QGridLayout *grid = new QGridLayout;
    for (int y = 0; y < SizeX; y++)
    {
        for (int x = 0; x < SizeY; x++)
        {
            arr[y * SizeX + x] = new Sector(this, 5, QColor(255, 255, 255));
            grid->addWidget(arr[y * SizeX + x], x, y);
        }
    }
    grid->setSpacing(2);
    setLayout(grid);
    setPalette(QPalette(QColor(100, 100, 100)));
    setAutoFillBackground(true);
}

void Field::SetSectorColor(QColor color, int x, int y)
{
    arr[y*SizeX + x]->setPalette(QPalette(color));
}

void Console::FileChosenLoad(const QString& str)
{
    fd_load->close();
    emit LoadSelect(str);
}

Console::Console(QWidget *parent) : QWidget(parent)
{
    QPushButton* NewWorld = new QPushButton("Создать мир", this);
    NewWorld->setFixedSize(100, 30);

    connect(NewWorld, SIGNAL(clicked(bool)), parent, SLOT(NewWorld()));

    QPushButton* Load = new QPushButton("Загрузить мир", this);
    Load->setFixedSize(100, 30);

    fd_load = new QFileDialog(this, "Загрузка мира");
    fd_load->setFileMode(QFileDialog::ExistingFile);

    connect(Load, SIGNAL(clicked()), fd_load, SLOT(show()));
    connect(fd_load, SIGNAL(fileSelected(const QString&)), this, SLOT(FileChosenLoad(const QString&)));

    QPushButton* Save = new QPushButton("Сохранить мир", this);
    Save->setFixedSize(100, 30);

    connect(Save, SIGNAL(clicked()), parent, SLOT(SaveFile()));

    QPushButton* Pause = new QPushButton("Пауза", this);
    Pause->setFixedSize(100, 30);

    connect(Pause, SIGNAL(clicked(bool)), parent, SLOT(Pause()));

    QVBoxLayout* vl = new QVBoxLayout(this);
    vl->addWidget(NewWorld);
    vl->addWidget(Load);
    vl->addWidget(Save);
    vl->addWidget(Pause);
    setLayout(vl);
}

void Window::SetSectorColor(QColor color, int x, int y)
{
    field->SetSectorColor(color, x, y);
}

Window::Window() : QWidget(0)
{
    Console* console = new Console(this);
    connect(console, SIGNAL(LoadSelect(const QString&)), this, SLOT(Load(const QString&)));
    //field = new Field(this, 20, 20);
    //field->move(150, 10);

    resize(640, 480);
}

void Window::Action()
{
    E_STEP step;
    while ((step = world->PerformStep()) == STEP_LIVE)
    {
        if(pause)   break;
        UpdateField();
        qApp->processEvents();
    }
    UpdateField();
    if(step == STEP_VICTIMS_WIN)
    {
        QMessageBox::information(0, "Конец", "Все хищники уничтожены, жертвы заполнили всю планету!");
    }
    else if(step == STEP_ALL_DEATH)
    {
        QMessageBox::information(0, "Конец", "Хищники уничтожили всех жертв и умерли от голода!");
    }
}

void Window::UpdateField()
{
    for (int x = 0; x < 20; x++)
    {
        for (int y = 0; y < 20; y++)
        {
            switch (world->InCell(x, y))
            {
                case ENTITY_NONE:
                {
                    SetSectorColor(Qt::white, x, y);
                    break;
                }
                case ENTITY_VICTIMS:
                {
                    SetSectorColor(Qt::green, x, y);
                    break;
                }
                case ENTITY_PREDATORS:
                {
                    SetSectorColor(Qt::red, x, y);
                    break;
                }
            }
        }
    }
}

void Window::SaveFile()
{
    if(world)
    {
        fd_save = new QFileDialog(this, "Сохранение мира");
        fd_save->setFileMode(QFileDialog::AnyFile);
        connect(fd_save, SIGNAL(fileSelected(const QString&)), this, SLOT(Save(const QString&)));
        fd_save->show();
    }
    else
    {
        QMessageBox::information(0, "Ошибка", "Нет созданого мира для сохранения");
    }
}

void Window::NewWorld()
{
    field = new Field(this, 20, 20);
    field->move(150, 10);
    field->show();

    world = std::make_unique<World>(20, 20);
    UpdateField();
    pause = true;
}

void Window::Load(const QString& filename)
{
    world = std::make_unique<World>(filename.toStdString().c_str());
    UpdateField();
    pause = true;
}

void Window::Save(const QString& filename)
{
    world->SaveWorld(filename.toStdString().c_str());
}

void Window::Pause()
{
    if(world)
    {
        if(pause)
        {
            pause = false;
            Action();
        }
        else
        {
            pause = true;
        }
    }
}
