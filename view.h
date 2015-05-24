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
#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QMenuBar>
#include <QSpinBox>
#include <QTime>
#include <QSlider>

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
    int SizeX, SizeY;
private:
    std::map<int, Sector*> arr;
};

class Console : public QWidget
{
    Q_OBJECT
    QFileDialog* fd_load;
    QPushButton* Pause;
    //QFileDialog* fd_save;
public:
    Console(QWidget* parent);
public slots:
    //void FileChosenSave(const QString&);
    void FileChosenLoad(const QString&);
    void ChangeText(const QString&);
signals:
    void LoadSelect(const QString& str);
    void SaveSelect(const QString& str);
};

class NewWorldDialog : public QDialog
{
    Q_OBJECT

//    class Box : public QHBoxLayout
//    {
//    private:
//        QLabel* label;
//        QSpinBox* box;
//        QHBoxLayout* layout;
//    public:
//        Box(QHBoxLayout* parent);
//    };

public:
    QSpinBox* HBox;
    QSpinBox* VBox;
    QSpinBox* PredBox;
    QSpinBox* VictBox;

    NewWorldDialog(QWidget* parent);
public slots:
    void PredSync(int);
    void VictSync(int);
    void Confirmed(bool);
signals:
    void ConfirmedNewWorld();
};

class Window : public QWidget
{
    Q_OBJECT
    Field* field = 0;
    std::unique_ptr<World> world;

    bool pause = false;

    QFileDialog* fd_save;
    NewWorldDialog* dialog;
    QVBoxLayout* vlayout;
    int period = 250;

public:
    Window();
    void SetSectorColor(QColor color, int x, int y);
    void Action();
    void UpdateField();

public slots:
    void CreateNewWorld();
    void SetNewWorld();
    void Load(const QString&);
    void SaveFile();
    void Save(const QString&);
    void Pause();
    void SetPeriod(int);
signals:
    void step();
    void Paused(QString);
    void Started();

};

#endif // VIEW

