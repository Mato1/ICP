#include "ui/port_w.h"
#include <QPalette>
#include <QColor>
#include <QToolTip>

namespace icp 
{
namespace ui
{

    PortW::PortW(std::string nazov_bloku, int cislo, model::PortType type)
        : model::Port(nazov_bloku, cislo, type)
    {
        // port = new model::Port(nazov_bloku, cislo, type);
        QPalette pal = palette();

        // set black background
        setMinimumHeight(10);

        if (type == model::PortType::input)
        {
            pal.setColor(QPalette::Background, QColor(50, 200, 50));
        } else
        {
            pal.setColor(QPalette::Background, QColor(200, 50, 50));
        }
        
        setMouseTracking(true);
        setAutoFillBackground(true);
        setPalette(pal);
     
        show();
    }

    void PortW::mouseMoveEvent(QMouseEvent * event)
    {
        if (this->rect().contains(event->pos()))
        {
            QToolTip::showText(event->globalPos(), get_data_type().to_string().c_str());
        }
    }
}
}