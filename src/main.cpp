/**
 * @brief 
 * 
 * @file main.cpp
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-05-06
 */

#include <QApplication>

#include "main_window.h"
#include "model/vyraz.h"

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    icp::MainWindow window;
    window.show();
    return app.exec();
}