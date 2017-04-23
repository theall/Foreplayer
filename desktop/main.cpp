#include "app/app.h"

int main(int argc, char *argv[])
{
    TApp app(argc, argv);
    if(app.isRunning())
    {
        qDebug() << "Foreplayer is running.";
        return 0;
    }
    return app.start();
}
