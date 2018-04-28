#include <QApplication>

#include "main_window.h"
#include "model/vyraz.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    icp::MainWindow window;
    window.show();
    return app.exec();
}