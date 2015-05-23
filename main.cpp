#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "useful.h"
#include "world.h"
#include "view.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    srand((int)time(NULL));
    Window w;
    w.show();
    return app.exec();
}
