#include "ui/vyraz_w.h"
#include <QPalette>
#include <QColor>

namespace icp
{
namespace ui
{
VyrazW::VyrazW(std::string expression, std::string premenna, QWidget * parent)
    : model::Vyraz(expression, premenna)
{
    setAutoFillBackground(true);
    setAlignment(Qt::AlignHCenter);
    setText(tr((expression+"="+premenna).c_str()));
    QFont f = font();
    f.setPixelSize(22);
    setFont(f);
    QPalette p = palette();
    p.setColor(QPalette::Background, QColor(15, 70, 90));
    setPalette(p);
    this->setParent(parent);
    show();
}
}
}