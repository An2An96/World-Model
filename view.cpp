#include "view.h"
#include "world.h"

Sector::Sector(QWidget* parent, int size, QColor color) : QWidget(parent)
{
    QSize s;
    s.setWidth(size);
    s.setHeight(size);
    setFixedSize(s);
    setPalette(QPalette(color));
    setAutoFillBackground(true);
}

Field::Field(QWidget *parent, int ASizeX, int ASizeY) : QWidget(parent)
{
    SizeX = ASizeX; SizeY = ASizeY;
    QGridLayout *grid = new QGridLayout;
    int SectorSizeX = 350/SizeX;
    int SectorSizeY = 350/SizeY;
    int SectorSize = (SectorSizeX > SectorSizeY) ? SectorSizeY : SectorSizeX;
    for (int y = 0; y < SizeY; y++)
    {
        for (int x = 0; x < SizeX; x++)
        {
            arr[y * SizeX + x] = new Sector(this, SectorSize, QColor(255, 255, 255));
            grid->addWidget(arr[y * SizeX + x], y, x);
        }
    }
    grid->setSpacing(2);
    setLayout(grid);
    setPalette(QPalette(QColor(100, 100, 100)));
    setAutoFillBackground(true);
    setFixedSize(sizeHint());
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

void Console::ChangeText(const QString& str)
{
    Pause->setText(str);
}

Console::Console(QWidget *parent) : QWidget(parent)
{
    QPushButton* NewWorld = new QPushButton("New world", this);
    NewWorld->setFixedSize(100, 30);

    connect(NewWorld, SIGNAL(clicked(bool)), parent, SLOT(SetNewWorld()));

    QPushButton* Load = new QPushButton("Load world", this);
    Load->setFixedSize(100, 30);

    fd_load = new QFileDialog(this, "Loading");
    fd_load->setFileMode(QFileDialog::ExistingFile);

    connect(Load, SIGNAL(clicked()), fd_load, SLOT(show()));
    connect(fd_load, SIGNAL(fileSelected(const QString&)), this, SLOT(FileChosenLoad(const QString&)));

    QPushButton* Save = new QPushButton("Save world", this);
    Save->setFixedSize(100, 30);

    connect(Save, SIGNAL(clicked()), parent, SLOT(SaveFile()));

    Pause = new QPushButton("Start", this);
    Pause->setFixedSize(100, 30);

    connect(Pause, SIGNAL(clicked(bool)), parent, SLOT(Pause()));
    connect(parent, SIGNAL(Paused(const QString&)), this, SLOT(ChangeText(const QString&)));

    QSlider* slider = new QSlider(this);
    slider->setRange(0, 500);
    slider->setValue(250);
    slider->setOrientation(Qt::Horizontal);
    connect(slider, SIGNAL(valueChanged(int)), parent, SLOT(SetPeriod(int)));

    QLabel* label = new QLabel("<p align = center>- Speed +</p>", this);

    QVBoxLayout* vl = new QVBoxLayout(this);
    vl->addWidget(NewWorld);
    vl->addWidget(Load);
    vl->addWidget(Save);
    vl->addWidget(Pause);
    vl->addWidget(slider);
    vl->addWidget(label);
    setLayout(vl);
}

void NewWorldDialog::PredSync(int)
{
    if (PredBox->value()+VictBox->value() > 100)
    {
        PredBox->setValue(100-VictBox->value());
    }
}

void NewWorldDialog::VictSync(int)
{
    if (VictBox->value()+PredBox->value() > 100)
    {
        VictBox->setValue(100-PredBox->value());
    }
}

void NewWorldDialog::Confirmed(bool)
{
    emit ConfirmedNewWorld();
    close();
}

NewWorldDialog::NewWorldDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle("New World");
    setFixedSize(200, 150);

    QLabel* HSize = new QLabel("World horizontal size");
    HBox = new QSpinBox(this);
    HBox->setRange(1, MaxSizeX);
    HBox->setValue(20);
    HBox->setFixedWidth(50);
    QHBoxLayout* HLayout = new QHBoxLayout();
    HLayout->addWidget(HSize);
    HLayout->addWidget(HBox);

    QLabel* VSize = new QLabel("World vertical size");
    VBox = new QSpinBox(this);
    VBox->setRange(1, MaxSizeY);
    VBox->setValue(20);
    VBox->setFixedWidth(50);
    QHBoxLayout* VLayout = new QHBoxLayout();
    VLayout->addWidget(VSize);
    VLayout->addWidget(VBox);

    QLabel* PredPerCent = new QLabel("Percentage of predators");
    PredBox = new QSpinBox(this);
    PredBox->setRange(0, 100);
    PredBox->setFixedWidth(50);
    PredBox->setValue(10);
    QHBoxLayout* PredLayout = new QHBoxLayout();
    PredLayout->addWidget(PredPerCent);
    PredLayout->addWidget(PredBox);

    QLabel* VictPerCent = new QLabel("Percentage of victims");
    VictBox = new QSpinBox(this);
    VictBox->setRange(0, 100);
    VictBox->setFixedWidth(50);
    VictBox->setValue(10);
    QHBoxLayout* VictLayout = new QHBoxLayout();
    VictLayout->addWidget(VictPerCent);
    VictLayout->addWidget(VictBox);

    QPushButton* confirm = new QPushButton("Create", this);
    confirm->setFixedSize(100, 30);

    connect(PredBox, SIGNAL(valueChanged(int)), this, SLOT(VictSync(int)));
    connect(VictBox, SIGNAL(valueChanged(int)), this, SLOT(PredSync(int)));
    connect(confirm, SIGNAL(clicked(bool)), this, SLOT(Confirmed(bool)));

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addLayout(HLayout);
    layout->addLayout(VLayout);
    layout->addLayout(PredLayout);
    layout->addLayout(VictLayout);
    layout->addWidget(confirm);
    setLayout(layout);
}

void Window::SetSectorColor(QColor color, int x, int y)
{
    field->SetSectorColor(color, x, y);
}

Window::Window() : QWidget(0)
{
    Console* console = new Console(this);

    connect(console, SIGNAL(LoadSelect(const QString&)), this, SLOT(Load(const QString&)));
    dialog = new NewWorldDialog(this);
    connect(dialog, SIGNAL(ConfirmedNewWorld()), this, SLOT(CreateNewWorld()));
    fd_save = new QFileDialog(this, "Saving");
    fd_save->setFileMode(QFileDialog::AnyFile);
    connect(fd_save, SIGNAL(fileSelected(const QString&)), this, SLOT(Save(const QString&)));
    setMinimumSize(640, 480);
}

void Window::SetPeriod(int p)
{
    period = 500-p;
}

void Window::Action()
{
    E_STEP step;

    while ((step = world->PerformStep()) == STEP_LIVE)
    {
        if (!this->isActiveWindow())
            break;
        if(pause)   break;
        UpdateField();
        QTime time;
        time.start();
        for (; time.elapsed() < period;)
            qApp->processEvents();
        qApp->processEvents();
    }
    UpdateField();
    if(step == STEP_VICTIMS_WIN)
    {
        emit Paused("Start");
        QMessageBox::information(0, "The end", "All predators are dead, victims populated the whole planet!");

    }
    else if(step == STEP_ALL_DEATH)
    {
        emit Paused("Start");
        QMessageBox::information(0, "The end", "Predators killed all the victims and died of hunger!");

    }
}

void Window::UpdateField()
{
    for (int x = 0; x < field->SizeX; x++)
    {
        for (int y = 0; y < field->SizeY; y++)
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
        fd_save->show();
    }
    else
    {
        QMessageBox::information(0, "Error", "No world for saving");
    }
}

void Window::SetNewWorld()
{    
    dialog->show();
}

void Window::CreateNewWorld()
{
    world = std::make_unique<World>(dialog->HBox->value(), dialog->VBox->value(),
                      dialog->PredBox->value(), dialog->VictBox->value());
    if (field)
        field->close();
    field = new Field(this, dialog->HBox->value(), dialog->VBox->value());
    field->move(150, 10);
    field->show();
    UpdateField();
    pause = true;
}

void Window::Load(const QString& filename)
{
    world = std::make_unique<World>(filename.toStdString().c_str());
    if (field)
        field->close();
    Coord ws = world->GetWorldSize();
    field = new Field(this, ws.x, ws.y);
    field->move(150, 10);
    field->show();
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
            emit Paused("Pause");
            Action();
        }
        else
        {
            pause = true;
            emit Paused("Start");
        }
    }
}
