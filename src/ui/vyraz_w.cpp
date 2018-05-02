#include "ui/vyraz_w.h"
#include <Qpalette>
#include <QColor>

namespace icp
{
namespace ui
{
    VyrazW::VyrazW(std::string expression)
    {
        vyraz = new model::Vyraz(expression);
        QPalette pal = palette();
        setMinimumHeight(10);
        pal.setColor(QPalette::Background, QColor(50, 100, 50));
        setAutoFillBackground(true);
        setPalette(pal);
     
        show();
    }
}
}