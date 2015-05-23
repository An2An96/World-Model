#ifndef VIEW
#define VIEW

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QObject>
#include <QLabel>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QMenuBar>

#include <memory>

class World;

class Sector : public QWidget
{
public:
    Sector(QWidget *parent, int size, QColor color);
private:

};

class Field : public QWidget
{
public:
    Field(QWidget* parent, int ASizeX, int ASizeY);
    void SetSectorColor(QColor color, int x, int y);

private:
    int SizeX, SizeY;
    std::map<int, Sector*> arr;
};

class Console : public QWidget
{
    Q_OBJECT
    QFileDialog* fd_load;
    //QFileDialog* fd_save;
public:
    Console(QWidget* parent);
public slots:
    //void FileChosenSave(const QString&);
    void FileChosenLoad(const QString&);
signals:
    void LoadSelect(const QString& str);
    void SaveSelect(const QString& str);
};

class Window : public QWidget
{
    Q_OBJECT
    Field* field;
    std::unique_ptr<World> world;

    bool pause = false;

    QFileDialog* fd_save;
    QVBoxLayout* vlayout;

public:
    Window();
    void SetSectorColor(QColor color, int x, int y);
    void Action();
    void UpdateField();

public slots:
    void NewWorld();
    void Load(const QString&);
    void SaveFile();
    void Save(const QString&);
    void Pause();
signals:
    void step();

};

#endif // VIEW

