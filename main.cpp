#include "app/app.h"

int main(int argc, char *argv[])
{
    if(!TApp::isRunning())
    {
        TApp app(argc, argv);
        return app.start();
    } else {
        qDebug() << "Foreplayer is running";
    }

    return 0;
}
