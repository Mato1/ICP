#include "ui/port_w.h"
#include <QPalette>
#include <QColor>
namespace icp 
{
namespace ui
{

    PortW::PortW(std::string nazov_bloku, int cislo, model::PortType type)
    {
        port = new model::Port(nazov_bloku, cislo, type);
        //setStyleSheet(style_sheet.c_str());
        setMinimumWidth(30);
        setMinimumHeight(30);
        QPalette pal = palette();

        // set black background
        pal.setColor(QPalette::Background, QColor(44, 200, 50));
        
        setAutoFillBackground(true);
        setPalette(pal);
        
        
        show();
    }

}
}