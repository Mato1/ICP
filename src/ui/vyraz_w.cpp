#include "ui/vyraz_w.h"
#include <QPalette>
#include <QColor>

namespace icp
{
namespace ui
{
    VyrazW::VyrazW(std::string expression, std::string premenna)
        :model::Vyraz(expression, premenna)
    {
        QPalette pal = palette();
        setMinimumHeight(10);
        pal.setColor(QPalette::Background, QColor(200, 200, 200));
        setAutoFillBackground(true);
        setPalette(pal);
     
        show();
    }
}
}