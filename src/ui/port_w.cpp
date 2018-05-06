#include "ui/port_w.h"
#include <QPalette>
#include <QColor>
#include <QToolTip>
#include <QPen>
#include <QPainter>
#include "main_window.h"

namespace icp
{
namespace ui
{

PortW::PortW(std::string nazov_bloku, int cislo, model::PortType type , QWidget * parent)
    : model::Port(nazov_bloku, cislo, type)
{
    setMaximumWidth(MainWindow::GRID_SQUARE_SIZE);
    setMaximumHeight(MAX_HEIGHT);
    setMouseTracking(true);
    this->setParent(parent);
    this->show();
}

void PortW::mouseMoveEvent(QMouseEvent * event)
{
    if (this->rect().contains(event->pos()))
    {
        QToolTip::showText(event->globalPos(), tr(to_string().c_str()));
    }
}

void PortW::paintEvent(QPaintEvent * event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    QPen pen;

    pen.setStyle(Qt::SolidLine);
    pen.setWidth(MAX_HEIGHT);

    if (get_port_type() == model::PortType::input)
    {
        pen.setBrush(QColor(50, 240, 220));
    }
    else if (get_port_type() == model::PortType::output)
    {
        pen.setBrush(QColor(227, 140, 255));
    }

    painter.setPen(pen);

    painter.drawLine(0, 0, 40, 0);
}

}
}