#include <QApplication>
#include "ui/TrackerWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Créer la fenêtre principale du tracker
    TrackerWindow w;
    w.show();

    return app.exec();
}